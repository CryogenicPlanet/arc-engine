// Copyright 2026 ArcMod. Bridge actual Chromium histogram samples to Arc's app.
#include "arc/api/arc_internal.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "base/containers/span.h"
#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "base/metrics/histogram_base.h"
#include "base/metrics/statistics_recorder.h"
#include "base/no_destructor.h"
#include "base/numerics/byte_conversions.h"
#include "base/task/single_thread_task_runner.h"
#include "base/time/time.h"
#include "chrome/browser/metrics/desktop_session_duration/desktop_session_duration_tracker.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"

ARC_EXPORT @interface ArcHistogramSamples : NSObject
- (instancetype)initWithPeriodicHistogramSample:(const void*)sample;
@end
ARC_EXPORT @interface ArcAdhocHistogramSamples : NSObject
- (instancetype)initWithAdhocHistogramSample:(const void*)sample;
@end
@protocol ApplicationHistogramDelegate <NSObject>
- (void)periodicHistogramSamplesAvailable:(ArcHistogramSamples*)samples;
- (void)adhocHistogramSamplesAvailable:(ArcAdhocHistogramSamples*)samples;
@end

namespace {
// Native histograms_observer.cc registers these names and preserves Sample32.
// Indices are native payload order, independently verified against all37 DTO slots.
constexpr std::array<std::string_view, 37> kPeriodicHistograms = {
    "Memory.Total.PrivateMemoryFootprint",
    "Memory.Browser.PrivateMemoryFootprint",
    "PerformanceMonitor.AverageCPU10.Total.AllTabsHidden_NoVideoCaptureOrAudio",
    "PerformanceMonitor.AverageCPU10.Total",
    "PerformanceMonitor.AverageCPU10.BrowserProcess",
    "PerformanceMonitor.IdleWakeups2.Total.AllTabsHidden_NoVideoCaptureOrAudio",
    "PerformanceMonitor.IdleWakeups2.Total",
    "Power.BatteryDischargeRateRelative5.AllTabsHidden_NoVideoCaptureOrAudio.Periodic",
    "Power.BatteryDischargeRateRelative5.Navigation.Periodic",
    "Power.BatteryDischargeRateRelative5",
    "Memory.PressureWindowDuration2.CriticalToModerate",
    "Memory.PressureWindowDuration2.CriticalToNone",
    "Memory.PressureWindowDuration2.ModerateToCritical",
    "Memory.PressureWindowDuration2.ModerateToNone",
    "PerformanceMonitor.AverageCPU10.GPUProcess",
    "PerformanceMonitor.AverageCPU10.NetworkProcess",
    "PerformanceMonitor.AverageCPU10.RendererExtensionEventProcess",
    "PerformanceMonitor.AverageCPU10.RendererExtensionPersistentProcess",
    "PerformanceMonitor.AverageCPU10.RendererProcess",
    "PerformanceMonitor.AverageCPU10.Total.FullscreenVideo",
    "PerformanceMonitor.AverageCPU10.Total.Interaction",
    "PerformanceMonitor.AverageCPU10.Total.Navigation",
    "PerformanceMonitor.AverageCPU10.Total.Passive",
    "Memory.Renderer.PrivateMemoryFootprint",
    "Memory.Gpu.PrivateMemoryFootprint",
    "Memory.NetworkService.PrivateMemoryFootprint",
    "Memory.Extension.PrivateMemoryFootprint",
    "PerformanceMonitor.AverageCPU10.DiaAiServiceProcess",
    "Memory.DiaAiService.PrivateMemoryFootprint",
    "Memory.AudioService.PrivateMemoryFootprint",
    "Memory.Utility.PrivateMemoryFootprint",
    "Memory.Total.RendererPrivateMemoryFootprint",
    "Memory.Total.SharedMemoryFootprint",
    "Memory.Total.PrivateSwapFootprint",
    "Memory.Total.ResidentSet",
    "Memory.Total.GpuMappedMemory",
    "Memory.Total.TileMemory",
};
constexpr std::array<std::string_view, 5> kAdhocHistograms = {
    "Arc.LowMemoryPolicy.ReclaimTargetMb",
    "Arc.LowMemoryPolicy.ReclaimedMemoryMb",
    "Arc.LowMemoryPolicy.AttemptThatDidntDecreaseMemory",
    "Arc.AccessibilityDelegate.TimeToKillSwitchInS",
    "Arc.AccessibilityDelegate.AccessibilitySessionsPerHour",
};

uint32_t BrowserState() {
  if (!metrics::DesktopSessionDurationTracker::IsInitialized()) return 0;
  const auto* tracker = metrics::DesktopSessionDurationTracker::Get();
  return uint32_t{tracker->is_visible()} | (uint32_t{tracker->in_session()} << 1) |
         (uint32_t{tracker->is_audio_playing()} << 2);
}
struct PeriodicSample {
  int32_t value;
  uint32_t browser_state;
};
struct AdhocSample {
  uint32_t type;
  int32_t value;
};
static_assert(sizeof(AdhocSample) == 8);
struct AdhocInput {
  uint64_t ignored_header = 0;
  uintptr_t data_address;
  uint64_t count;
};
static_assert(offsetof(AdhocInput, data_address) == 8);
static_assert(offsetof(AdhocInput, count) == 16);

class ApplicationHistogramRecorder final {
 public:
  void SetDelegate(id<ApplicationHistogramDelegate> delegate) {
    delegate_ = delegate;
    // Explicit nil removes the callback. An expired weak capture leaves its
    // native closure installed: flush must still clear the accumulated batch.
    callback_installed_ = delegate != nil;
  }
  void Start() {
    DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
    if (started_) return;
    started_ = true;
    for (size_t index = 0; index < kPeriodicHistograms.size(); ++index) {
      observers_.push_back(std::make_unique<
          base::StatisticsRecorder::ScopedHistogramSampleObserver>(
          kPeriodicHistograms[index],
          base::BindRepeating(&ApplicationHistogramRecorder::OnPeriodic,
                              weak_factory_.GetWeakPtr(), index)));
    }
    for (size_t index = 0; index < kAdhocHistograms.size(); ++index) {
      observers_.push_back(std::make_unique<
          base::StatisticsRecorder::ScopedHistogramSampleObserver>(
          kAdhocHistograms[index],
          base::BindRepeating(&ApplicationHistogramRecorder::OnAdhoc,
                              weak_factory_.GetWeakPtr(), uint32_t(index))));
    }
  }

 private:
  void OnPeriodic(size_t index, std::string_view, uint64_t,
                  base::HistogramBase::Sample32 sample) {
    // Capture before an early flush can call back into the app and change state.
    const PeriodicSample incoming{sample, BrowserState()};
    if (periodic_[index]) Flush();
    periodic_[index] = incoming;
    periodic_dirty_ = true;
    Schedule();
  }
  void OnAdhoc(uint32_t type, std::string_view, uint64_t,
               base::HistogramBase::Sample32 sample) {
    adhoc_.push_back({type, sample});
    Schedule();
  }
  void Schedule() {
    if (scheduled_) return;
    scheduled_ = true;
    content::GetUIThreadTaskRunner({})->PostDelayedTask(
        FROM_HERE,
        base::BindOnce(&ApplicationHistogramRecorder::OnBatchDeadline,
                       weak_factory_.GetWeakPtr()),
        base::Seconds(5));
  }
  void OnBatchDeadline() {
    Flush();
    // A duplicate-field early flush does not cancel this pending deadline.
    scheduled_ = false;
  }
  void Flush() {
    if (callback_installed_ && periodic_dirty_) {
      id<ApplicationHistogramDelegate> delegate = delegate_;
      if ([delegate respondsToSelector:@selector(periodicHistogramSamplesAvailable:)]) {
        // This input adapter supplies only bytes read by the already verified
        // DTO constructor. Its ignored first8 bytes need no native vtable.
        std::array<uint8_t, 0x1c8> payload{};
        auto bytes = base::span(payload);
        for (size_t index = 0; index < periodic_.size(); ++index) {
          if (!periodic_[index]) continue;
          const size_t offset = 8 + index * 12;
          bytes.subspan(offset).first<4>().copy_from(
              base::I32ToNativeEndian(periodic_[index]->value));
          bytes.subspan(offset + 4).first<4>().copy_from(
              base::U32ToNativeEndian(periodic_[index]->browser_state));
          bytes[offset + 8] = 1;
        }
        ArcHistogramSamples* samples = [[ArcHistogramSamples alloc]
            initWithPeriodicHistogramSample:payload.data()];
        [delegate periodicHistogramSamplesAvailable:samples];
      }
      // Native clears AFTER delivery, including fields added reentrantly.
      for (auto& sample : periodic_) sample.reset();
      periodic_dirty_ = false;
    }
    // Re-read the current callback after periodic delivery: the app can replace it.
    if (callback_installed_ && !adhoc_.empty()) {
      id<ApplicationHistogramDelegate> delegate = delegate_;
      if ([delegate respondsToSelector:@selector(adhocHistogramSamplesAvailable:)]) {
        AdhocInput payload{0, reinterpret_cast<uintptr_t>(adhoc_.data()), adhoc_.size()};
        ArcAdhocHistogramSamples* samples = [[ArcAdhocHistogramSamples alloc]
            initWithAdhocHistogramSample:&payload];
        [delegate adhocHistogramSamplesAvailable:samples];
      }
      adhoc_.clear();
    }
  }
  __weak id<ApplicationHistogramDelegate> delegate_ = nil;
  bool callback_installed_ = false;
  bool started_ = false;
  bool scheduled_ = false;
  bool periodic_dirty_ = false;
  std::array<std::optional<PeriodicSample>, 37> periodic_;
  std::vector<AdhocSample> adhoc_;
  std::vector<std::unique_ptr<
      base::StatisticsRecorder::ScopedHistogramSampleObserver>> observers_;
  base::WeakPtrFactory<ApplicationHistogramRecorder> weak_factory_{this};
};
ApplicationHistogramRecorder& HistogramRecorder() {
  static base::NoDestructor<ApplicationHistogramRecorder> recorder;
  return *recorder;
}

}  // namespace

// The host must call this once on the UI sequence from PostBrowserStart,
// independently of registration/replacement/lifetime of the weak app delegate.
void ArcAppInitializeHistogramObservers() {
  HistogramRecorder().Start();
}

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end
@implementation ArcBrowserApplication (AppHistogramSamples)
- (void)setHistogramSamplesDelegate:(id<ApplicationHistogramDelegate>)delegate {
  HistogramRecorder().SetDelegate(delegate);
}
@end
