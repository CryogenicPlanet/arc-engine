// Copyright 2026 ArcMod. Infobar and renderer-hang UI lifetimes.
#import "arc/browser/ui_values.h"
#include "arc/browser/ui_hooks.h"

#include <map>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "chrome/browser/hang_monitor/hang_crash_dump.h"
#include "components/infobars/content/content_infobar_manager.h"
#include "components/infobars/core/confirm_infobar_delegate.h"
#include "components/infobars/core/infobar.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/render_process_host_observer.h"
#include "content/public/browser/render_widget_host.h"
#include "content/public/browser/render_widget_host_observer.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "content/public/common/result_codes.h"

@interface ArcWebContents (UILifecycleInternal)
- (id)uiDelegate;
@end
@protocol HostLifecycleUI <NSObject>
- (void)webContents:(ArcWebContents*)contents willShowConfirmInfobar:(ArcConfirmInfobar*)infobar;
- (void)webContents:(ArcWebContents*)contents willCloseConfirmInfobar:(ArcConfirmInfobar*)infobar;
- (BOOL)webContents:(ArcWebContents*)contents showHungRendererDialog:(ArcHungRendererDialog*)dialog;
- (void)webContentsHideHungRendererDialog:(ArcWebContents*)contents;
@end

namespace {
id<HostLifecycleUI> Delegate(ArcWebContents* owner) {
  return [owner respondsToSelector:@selector(uiDelegate)] ? [owner uiDelegate] : nil;
}
struct LiveInfoBar {
  base::WeakPtr<infobars::InfoBar> bar;
};
struct InfoBarEntry {
  std::shared_ptr<LiveInfoBar> live;
  ArcConfirmInfobar* __strong wrapper;
};

class HostInfoBarBridge : public content::WebContentsObserver,
                          public content::WebContentsUserData<HostInfoBarBridge>,
                          public infobars::InfoBarManager::Observer {
 public:
  ~HostInfoBarBridge() override { Reset(); }
  void Initialize() {
    auto* manager = infobars::ContentInfoBarManager::FromWebContents(web_contents());
    if (!manager || observation_.IsObserving()) { return; }
    observation_.Observe(manager);
    owner_ = [ArcWebContents wrapperForWebContents:web_contents()];
    // Creation runs before the app sets its UI delegate. Replay on the next UI
    // turn, when the canonical wrapper has finished registering its identity.
    content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
        base::BindOnce(&HostInfoBarBridge::Replay, weak_factory_.GetWeakPtr()));
  }
  void OnInfoBarAdded(infobars::InfoBar* bar) override {
    if (!bar->delegate()->AsConfirmInfoBarDelegate()) { return; }
    const uintptr_t key = reinterpret_cast<uintptr_t>(bar);
    if (entries_.contains(key)) { return; }
    auto live = std::make_shared<LiveInfoBar>();
    live->bar = bar->AsWeakPtr();
    arc::UIConfirmInfobarData data;
    data.message = [live]() {
      const auto* delegate = live->bar ? live->bar->delegate()->AsConfirmInfoBarDelegate() : nullptr;
      return delegate ? delegate->GetMessageText() : std::u16string();
    };
    data.accepted = [live]() {
      // Original 0x443b434 calls Accept and discards its result. Dismissal is
      // driven by the owning native producer, not inferred from Views UI.
      if (live->bar) {
        auto* delegate = live->bar->delegate()->AsConfirmInfoBarDelegate();
        if (delegate) { delegate->Accept(); }
      }
    };
    ArcConfirmInfobar* wrapper = [[ArcConfirmInfobar alloc] initWithConfirmInfobar:&data];
    entries_.emplace(key, InfoBarEntry{live, wrapper});
    ArcWebContents* owner = owner_;
    id<HostLifecycleUI> delegate = Delegate(owner);
    if ([delegate respondsToSelector:@selector(webContents:willShowConfirmInfobar:)]) {
      [delegate webContents:owner willShowConfirmInfobar:wrapper];
    }
  }
  void OnInfoBarRemoved(infobars::InfoBar* bar, bool) override {
    const auto found = entries_.find(reinterpret_cast<uintptr_t>(bar));
    if (found == entries_.end()) { return; }
    auto entry = std::move(found->second);
    entries_.erase(found);
    ArcWebContents* owner = owner_;
    id<HostLifecycleUI> delegate = Delegate(owner);
    if ([delegate respondsToSelector:@selector(webContents:willCloseConfirmInfobar:)]) {
      [delegate webContents:owner willCloseConfirmInfobar:entry.wrapper];
    }
    // The local shared owner survives reentrant teardown during the callback.
    entry.live->bar.reset();
  }
  void OnInfoBarReplaced(infobars::InfoBar* old_bar, infobars::InfoBar* new_bar) override {
    auto weak = weak_factory_.GetWeakPtr();
    auto replacement = new_bar->AsWeakPtr();
    OnInfoBarRemoved(old_bar, false);
    if (weak && replacement) { weak->OnInfoBarAdded(replacement.get()); }
  }
  void OnManagerWillBeDestroyed(infobars::InfoBarManager*) override { Reset(); }
  void WebContentsDestroyed() override { Reset(); }
 private:
  friend class content::WebContentsUserData<HostInfoBarBridge>;
  explicit HostInfoBarBridge(content::WebContents* contents)
      : content::WebContentsObserver(contents), content::WebContentsUserData<HostInfoBarBridge>(*contents) {}
  void Replay() {
    if (!observation_.IsObserving()) { return; }
    std::vector<base::WeakPtr<infobars::InfoBar>> bars;
    for (const auto& bar : observation_.GetSource()->infobars()) { bars.push_back(bar->AsWeakPtr()); }
    auto weak = weak_factory_.GetWeakPtr();
    for (auto& bar : bars) {
      if (!weak) { return; }
      if (bar) { weak->OnInfoBarAdded(bar.get()); }
    }
  }
  void Reset() {
    weak_factory_.InvalidateWeakPtrs();
    observation_.Reset();
    for (auto& [key, entry] : entries_) { entry.live->bar.reset(); }
    entries_.clear();
  }
  __weak ArcWebContents* owner_ = nil;
  std::map<uintptr_t, InfoBarEntry> entries_;
  base::ScopedObservation<infobars::InfoBarManager, infobars::InfoBarManager::Observer> observation_{this};
  base::WeakPtrFactory<HostInfoBarBridge> weak_factory_{this};
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(HostInfoBarBridge);

class HostHungRendererBridge : public content::WebContentsObserver,
    public content::WebContentsUserData<HostHungRendererBridge>,
    public content::RenderWidgetHostObserver,
    public content::RenderProcessHostObserver {
 public:
  ~HostHungRendererBridge() override { Clear(); }
  bool Show(content::RenderWidgetHost* widget, base::RepeatingClosure restarter) {
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:web_contents()];
    id<HostLifecycleUI> delegate = Delegate(owner);
    if (![delegate respondsToSelector:@selector(webContents:showHungRendererDialog:)]) { return false; }
    Clear();
    owner_ = owner;
    widget_observation_.Observe(widget);
    process_observation_.Observe(widget->GetProcess());
    restarter_ = std::move(restarter);
    auto weak = weak_factory_.GetWeakPtr();
    const uint64_t generation = generation_;
    arc::UIHungRendererDialogData data;
    data.wait = [weak, generation]() {
      if (!weak || weak->generation_ != generation) { return; }
      auto restarter = weak->restarter_;
      if (restarter) { restarter.Run(); }
    };
    data.crash_process = [weak, generation]() {
      if (!weak || weak->generation_ != generation || !weak->process_observation_.IsObserving()) { return; }
      auto* process = weak->process_observation_.GetSource();
      if (process->IsInitializedAndNotDead()) {
        CrashDumpHungChildProcess(process->GetProcess().Handle());
        process->Shutdown(content::RESULT_CODE_HUNG);
      }
    };
    dialog_ = [[ArcHungRendererDialog alloc] initWithHungRendererDialog:&data];
    shown_ = true;
    const bool handled = [delegate webContents:owner showHungRendererDialog:dialog_];
    // Reentrant teardown consumes the request: native fallback would use stale
    // source/widget pointers supplied before the app callback.
    if (!weak || weak->generation_ != generation) { return true; }
    if (!handled) { weak->Clear(); }
    return handled;
  }
  bool Hide(content::RenderWidgetHost* widget) {
    if (!widget_observation_.IsObservingSource(widget)) { return false; }
    Hide();
    return true;
  }
  void Hide() {
    const bool notify = shown_;
    // Native teardown must never manufacture a wrapper for dying contents.
    ArcWebContents* owner = owner_;
    id<HostLifecycleUI> delegate = Delegate(owner);
    Clear();
    if (notify && [delegate respondsToSelector:@selector(webContentsHideHungRendererDialog:)]) {
      [delegate webContentsHideHungRendererDialog:owner];
    }
  }
  void RenderWidgetHostDestroyed(content::RenderWidgetHost*) override {
    widget_observation_.Reset();
    Hide();
  }
  void RenderProcessExited(content::RenderProcessHost*, const content::ChildProcessTerminationInfo&) override { Hide(); }
  void RenderProcessHostDestroyed(content::RenderProcessHost*) override {
    process_observation_.Reset();
    Hide();
  }
  void WebContentsDestroyed() override { Hide(); }
 private:
  friend class content::WebContentsUserData<HostHungRendererBridge>;
  explicit HostHungRendererBridge(content::WebContents* contents)
      : content::WebContentsObserver(contents), content::WebContentsUserData<HostHungRendererBridge>(*contents) {}
  void Clear() {
    ++generation_;
    shown_ = false;
    owner_ = nil;
    dialog_ = nil;
    restarter_.Reset();
    widget_observation_.Reset();
    process_observation_.Reset();
  }
  __weak ArcWebContents* owner_ = nil;
  ArcHungRendererDialog* __strong dialog_;
  base::RepeatingClosure restarter_;
  uint64_t generation_ = 0;
  bool shown_ = false;
  base::ScopedObservation<content::RenderWidgetHost, content::RenderWidgetHostObserver> widget_observation_{this};
  base::ScopedObservation<content::RenderProcessHost, content::RenderProcessHostObserver> process_observation_{this};
  base::WeakPtrFactory<HostHungRendererBridge> weak_factory_{this};
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(HostHungRendererBridge);
}  // namespace

void ArcUIWebContentsCreated(content::WebContents* contents) {
  if (!contents) { return; }
  HostInfoBarBridge::CreateForWebContents(contents);
  HostInfoBarBridge::FromWebContents(contents)->Initialize();
}
namespace arc {
bool ShowHungRendererDialog(content::WebContents* contents, content::RenderWidgetHost* widget,
                            base::RepeatingClosure restarter) {
  if (!contents || !widget) { return false; }
  HostHungRendererBridge::CreateForWebContents(contents);
  return HostHungRendererBridge::FromWebContents(contents)->Show(widget, std::move(restarter));
}
bool HideHungRendererDialog(content::WebContents* contents, content::RenderWidgetHost* widget) {
  auto* bridge = contents ? HostHungRendererBridge::FromWebContents(contents) : nullptr;
  return bridge && bridge->Hide(widget);
}
}  // namespace arc
