// Copyright 2026 ArcMod. Native downloads and the profile's download snapshot/deltas.
#import "arc/api/arc_internal.h"

#include <set>

#include "base/auto_reset.h"
#include "base/functional/bind.h"
#include "base/memory/raw_ptr.h"
#include "chrome/browser/arcmod/arcmod_hooks.h"
#include "base/scoped_multi_source_observation.h"
#include "base/scoped_observation.h"
#include "base/supports_user_data.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/download/download_core_service.h"
#include "chrome/browser/download/download_core_service_factory.h"
#include "chrome/browser/download/download_item_model.h"
#include "components/download/public/common/download_item.h"
#include "components/download/public/common/download_url_parameters.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "content/public/browser/download_manager.h"
#include "content/public/browser/download_item_utils.h"

ARC_EXPORT @interface ArcDownloadItem : NSObject
+ (instancetype)wrapperForDownload:(download::DownloadItem*)item __attribute__((objc_direct));
- (download::DownloadItem*)nativeDownload __attribute__((objc_direct));
@end

namespace arc_profile {
void AfterInitialSearchUpdate(Profile* profile, base::OnceClosure callback);
}

namespace {
constinit unsigned gDownloadCallbackDepth = 0;
struct DownloadHolder final : base::SupportsUserData::Data {
  ArcDownloadItem* __strong wrapper;
};
constexpr char kDownloadWrapperKey[] = "ArcDownloadItem";

class DownloadLifetime final : public download::DownloadItem::Observer {
 public:
  explicit DownloadLifetime(download::DownloadItem* item) : item_(item) {
    item_->AddObserver(this);
  }
  ~DownloadLifetime() override {
    if (item_) {
      item_->RemoveObserver(this);
    }
  }
  void OnDownloadDestroyed(download::DownloadItem* item) override { item_ = nullptr; }
  download::DownloadItem* item() const { return item_; }
 private:
  raw_ptr<download::DownloadItem> item_;
};
}  // namespace

@implementation ArcDownloadItem {
  std::unique_ptr<DownloadLifetime> _lifetime;
  std::unique_ptr<DownloadItemModel> _nativeModel;
  ArcWebContents* _webContents;
}
+ (instancetype)wrapperForDownload:(download::DownloadItem*)item {
  if (!item) {
    return nil;
  }
  auto* holder = static_cast<DownloadHolder*>(item->GetUserData(kDownloadWrapperKey));
  if (!holder) {
    auto owned = std::make_unique<DownloadHolder>();
    owned->wrapper = [[ArcDownloadItem alloc] init];
    owned->wrapper->_lifetime = std::make_unique<DownloadLifetime>(item);
    owned->wrapper->_nativeModel = std::make_unique<DownloadItemModel>(item);
    holder = owned.get();
    item->SetUserData(kDownloadWrapperKey, std::move(owned));
  }
  return holder->wrapper;
}
- (download::DownloadItem*)nativeDownload { return _lifetime ? _lifetime->item() : nullptr; }
- (int64_t)completedBytes { return _nativeModel && _nativeModel->GetDownloadItem() ? _nativeModel->GetCompletedBytes() : 0; }
- (int64_t)totalBytes { return _nativeModel && _nativeModel->GetDownloadItem() ? _nativeModel->GetTotalBytes() : 0; }
- (int64_t)currentSpeed { return _lifetime && _lifetime->item() ? _lifetime->item()->CurrentSpeed() : 0; }
- (NSString*)targetPath {
  return _lifetime && _lifetime->item() ? ArcNSString(_lifetime->item()->GetTargetFilePath().value()) : @"";
}
- (NSString*)sourceURL {
  return _lifetime && _lifetime->item() ? ArcNSString(_lifetime->item()->GetTabUrl().spec()) : @"";
}
- (NSString*)id { return _lifetime && _lifetime->item() ? ArcNSString(_lifetime->item()->GetGuid()) : @""; }
- (NSDate*)startTime {
  return _lifetime && _lifetime->item() ? ArcNSDate(_lifetime->item()->GetStartTime()) : nil;
}
- (ArcWebContents*)webContents {
  if (!_webContents && _lifetime && _lifetime->item()) {
    _webContents = [ArcWebContents wrapperForWebContents:
        content::DownloadItemUtils::GetWebContents(_lifetime->item())];
  }
  return _webContents;
}
- (NSInteger)currentState {
  return _lifetime && _lifetime->item() ? static_cast<NSInteger>(_lifetime->item()->GetState()) + 1 : 0;
}
- (int64_t)remainingTimeInSeconds {
  base::TimeDelta remaining;
  return _lifetime && _lifetime->item() && _lifetime->item()->TimeRemaining(&remaining)
             ? remaining.InSeconds() : -1;
}
- (BOOL)isPaused { return _lifetime && _lifetime->item() && _lifetime->item()->IsPaused(); }
- (BOOL)isDone { return _lifetime && _lifetime->item() && _lifetime->item()->IsDone(); }
- (BOOL)canResume { return _lifetime && _lifetime->item() && _lifetime->item()->CanResume(); }
- (void)pause {
  if (gDownloadCallbackDepth) {
    __weak ArcDownloadItem* weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{ [weakSelf pause]; });
    return;
  }
  if (_lifetime && _lifetime->item()) _lifetime->item()->Pause();
}
- (void)cancel:(BOOL)userCancel {
  if (gDownloadCallbackDepth) {
    __weak ArcDownloadItem* weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{ [weakSelf cancel:userCancel]; });
    return;
  }
  if (_lifetime && _lifetime->item()) { _lifetime->item()->Cancel(userCancel); }
}
- (void)resume {
  if (gDownloadCallbackDepth) {
    __weak ArcDownloadItem* weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{ [weakSelf resume]; });
    return;
  }
  if (_lifetime && _lifetime->item()) _lifetime->item()->Resume(true);
}
- (void)remove {
  if (gDownloadCallbackDepth) {
    __weak ArcDownloadItem* weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{ [weakSelf remove]; });
    return;
  }
  if (_lifetime && _lifetime->item()) _lifetime->item()->Remove();
}
@end

@protocol ArcProfileDownloadsDelegate <NSObject>
@optional
- (void)browserContext:(ArcBrowserContext*)context downloadManagerInitializedWithItems:(NSArray*)items;
- (void)browserContext:(ArcBrowserContext*)context onDownloadCreated:(ArcDownloadItem*)item;
- (void)browserContext:(ArcBrowserContext*)context onDownloadUpdated:(ArcDownloadItem*)item;
- (void)browserContext:(ArcBrowserContext*)context onDownloadRemoved:(ArcDownloadItem*)item;
- (void)browserContext:(ArcBrowserContext*)context onDeterminingFilename:(ArcDownloadItem*)item
       defaultFilename:(NSString*)filename completionTimeout:(double)timeout
            completion:(void (^)(NSString*))completion;
@end

ARC_EXPORT @interface ArcBrowserContextDownloads : NSObject
@property(nonatomic, weak) ArcBrowserContext* owner;
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
@end

namespace {
struct FilenameTarget final : base::SupportsUserData::Data {
  __weak ArcBrowserContextDownloads* service;
  __weak ArcBrowserContext* owner;
  __weak id<ArcProfileDownloadsDelegate> delegate;
};
constexpr char kFilenameTargetKey[] = "ArcProfileDownloadFilenameTarget";
class FilenameDecision {
 public:
  explicit FilenameDecision(base::OnceCallback<void(const base::FilePath&)> done)
      : done_(std::move(done)) {}
  void Resolve(NSString* filename) {
    if (done_) std::move(done_).Run(base::FilePath(ArcUTF8(filename)));
  }
 private:
  base::OnceCallback<void(const base::FilePath&)> done_;
};
bool DetermineFilename(Profile* profile, download::DownloadItem* item,
                       const base::FilePath& basename,
                       base::OnceCallback<void(const base::FilePath&)> done) {
  auto* target = static_cast<FilenameTarget*>(profile->GetUserData(kFilenameTargetKey));
  id<ArcProfileDownloadsDelegate> delegate = target ? target->delegate : nil;
  ArcBrowserContext* owner = target ? target->owner : nil;
  if (!target || !target->service || !owner ||
      ![delegate respondsToSelector:@selector(browserContext:onDeterminingFilename:defaultFilename:completionTimeout:completion:)])
    return false;
  auto decision = std::make_shared<FilenameDecision>(std::move(done));
  // Both paths run on the UI queue; the shared decision consumes the callback
  // once even when the app completes after the native timeout or calls twice.
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 15 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
    decision->Resolve(nil);
  });
  base::WeakPtr<Profile> weakProfile = profile->GetWeakPtr();
  ArcDownloadItem* wrapper = [ArcDownloadItem wrapperForDownload:item];
  NSString* filename = ArcNSString(basename.value());
  // Arc's native DelegateWrapper::InvokeAsync posts this event with delay zero.
  // Keep the app callback outside the target determiner's native stack, and
  // resolve the current weak bridge only when the UI task runs.
  dispatch_async(dispatch_get_main_queue(), ^{
    if (!weakProfile || ![wrapper nativeDownload]) {
      decision->Resolve(nil);
      return;
    }
    auto* current = static_cast<FilenameTarget*>(weakProfile->GetUserData(kFilenameTargetKey));
    id<ArcProfileDownloadsDelegate> currentDelegate = current ? current->delegate : nil;
    ArcBrowserContext* currentOwner = current ? current->owner : nil;
    if (!current || !current->service || !currentOwner ||
        ![currentDelegate respondsToSelector:@selector(browserContext:onDeterminingFilename:defaultFilename:completionTimeout:completion:)]) {
      decision->Resolve(nil);
      return;
    }
    base::AutoReset<unsigned> in_callback(&gDownloadCallbackDepth, gDownloadCallbackDepth + 1);
    [currentDelegate browserContext:currentOwner onDeterminingFilename:wrapper
                   defaultFilename:filename completionTimeout:15.0
                        completion:^(NSString* selected) {
      if ([NSThread isMainThread]) decision->Resolve(selected);
      else dispatch_async(dispatch_get_main_queue(), ^{ decision->Resolve(selected); });
    }];
  });
  return true;
}
// The native observer owns no ObjC delegate or context. Profile shutdown detaches
// the manager; individual download destruction detaches each item observation.
class DownloadsObserver final : public content::DownloadManager::Observer,
                                public download::DownloadItem::Observer,
                                public std::enable_shared_from_this<DownloadsObserver> {
 public:
  DownloadsObserver(content::DownloadManager* manager, ArcBrowserContext* owner,
                    id<ArcProfileDownloadsDelegate> delegate)
      : manager_(manager), owner_(owner), delegate_(delegate) {
    manager_->AddObserver(this);
    download::SimpleDownloadManager::DownloadVector items;
    manager_->GetAllDownloads(&items);
    for (const auto& item : items) {
      items_.AddObservation(item.get());
    }
  }
  ~DownloadsObserver() override {
    if (manager_) { manager_->RemoveObserver(this); }
  }
  void SetDelegate(id<ArcProfileDownloadsDelegate> delegate) { delegate_ = delegate; }
  void SendSnapshot() {
    ArcBrowserContext* owner = owner_;
    if (!manager_ || !owner || !manager_->IsManagerInitialized() || snapshot_sent_ ||
        snapshot_pending_ || !delegate_) return;
    Profile* profile = ArcProfileFrom(owner);
    if (!profile) return;
    snapshot_pending_ = true;
    arc_profile::AfterInitialSearchUpdate(profile, base::BindOnce([](std::weak_ptr<DownloadsObserver> weak) {
      auto alive = weak.lock();
      if (!alive) return;
      alive->snapshot_pending_ = false;
      alive->SendReadySnapshot();
    }, weak_from_this()));
  }
  void SendReadySnapshot() {
    auto keep_alive = shared_from_this();
    base::AutoReset<unsigned> in_callback(&gDownloadCallbackDepth, gDownloadCallbackDepth + 1);
    id<ArcProfileDownloadsDelegate> delegate = delegate_;
    ArcBrowserContext* owner = owner_;
    if (!manager_ || !owner || !manager_->IsManagerInitialized() || snapshot_sent_ || !delegate) {
      return;
    }
    snapshot_sent_ = true;
    download::SimpleDownloadManager::DownloadVector items;
    manager_->GetAllDownloads(&items);
    NSMutableArray* wrappers = [NSMutableArray array];
    for (const auto& item : items) {
      [wrappers addObject:[ArcDownloadItem wrapperForDownload:item.get()]];
    }
    if ([delegate respondsToSelector:@selector(browserContext:downloadManagerInitializedWithItems:)]) {
      [delegate browserContext:owner downloadManagerInitializedWithItems:[wrappers copy]];
    }
  }
  void OnManagerInitialized() override { SendSnapshot(); }
  void ManagerGoingDown(content::DownloadManager* manager) override {
    manager_->RemoveObserver(this);
    manager_ = nullptr;
    items_.RemoveAllObservations();
  }
  void OnDownloadCreated(content::DownloadManager* manager, download::DownloadItem* item) override {
    auto keep_alive = shared_from_this();
    base::AutoReset<unsigned> in_callback(&gDownloadCallbackDepth, gDownloadCallbackDepth + 1);
    if (!items_.IsObservingSource(item)) { items_.AddObservation(item); }
    // History loading is represented by the initial snapshot.
    if (!snapshot_sent_) { return; }
    pending_created_.insert(item->GetGuid());
    OnDownloadUpdated(item);
  }
  void OnDownloadUpdated(download::DownloadItem* item) override {
    auto keep_alive = shared_from_this();
    base::AutoReset<unsigned> in_callback(&gDownloadCallbackDepth, gDownloadCallbackDepth + 1);
    if (!snapshot_sent_) { return; }
    if (pending_created_.contains(item->GetGuid())) {
      if (item->GetTargetFilePath().empty() && !item->IsDone()) return;
      pending_created_.erase(item->GetGuid());
      id<ArcProfileDownloadsDelegate> delegate = delegate_;
      if ([delegate respondsToSelector:@selector(browserContext:onDownloadCreated:)])
        [delegate browserContext:owner_ onDownloadCreated:[ArcDownloadItem wrapperForDownload:item]];
      return;
    }
    id<ArcProfileDownloadsDelegate> delegate = delegate_;
    if ([delegate respondsToSelector:@selector(browserContext:onDownloadUpdated:)]) {
      [delegate browserContext:owner_ onDownloadUpdated:[ArcDownloadItem wrapperForDownload:item]];
    }
  }
  void OnDownloadRemoved(download::DownloadItem* item) override {
    auto keep_alive = shared_from_this();
    base::AutoReset<unsigned> in_callback(&gDownloadCallbackDepth, gDownloadCallbackDepth + 1);
    pending_created_.erase(item->GetGuid());
    if (!snapshot_sent_) { return; }
    id<ArcProfileDownloadsDelegate> delegate = delegate_;
    if ([delegate respondsToSelector:@selector(browserContext:onDownloadRemoved:)]) {
      [delegate browserContext:owner_ onDownloadRemoved:[ArcDownloadItem wrapperForDownload:item]];
    }
  }
  void OnDownloadDestroyed(download::DownloadItem* item) override {
    pending_created_.erase(item->GetGuid());
    items_.RemoveObservation(item);
  }
 private:
  raw_ptr<content::DownloadManager> manager_;
  __weak ArcBrowserContext* owner_;
  __weak id<ArcProfileDownloadsDelegate> delegate_;
  bool snapshot_sent_ = false;
  bool snapshot_pending_ = false;
  std::set<std::string> pending_created_;
  base::ScopedMultiSourceObservation<download::DownloadItem, download::DownloadItem::Observer> items_{this};
};
}  // namespace

@implementation ArcBrowserContextDownloads {
  __weak ArcBrowserContext* _owner;
  __weak id<ArcProfileDownloadsDelegate> _delegate;
  std::shared_ptr<DownloadsObserver> _observer;
}
@synthesize owner = _owner;
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context {
  if ((self = [super init])) { _owner = context; }
  return self;
}
- (instancetype)initWithBrowserContextDownloads:(const void*)unused
                             forBrowserContext:(ArcBrowserContext*)context {
  return [self initWithArcBrowserContext:context];
}
- (ArcBrowserContext*)owner { return _owner; }
- (void)setOwner:(ArcBrowserContext*)owner {
  if (_owner != owner) {
    if (Profile* profile = ArcProfileFrom(_owner)) {
      auto* target = static_cast<FilenameTarget*>(profile->GetUserData(kFilenameTargetKey));
      if (target && target->service == self) profile->RemoveUserData(kFilenameTargetKey);
    }
    _observer.reset();
    _owner = owner;
  }
}
- (void)setDelegate:(id<ArcProfileDownloadsDelegate>)delegate {
  ArcBrowserContext* owner = [self owner];
  // The original engine's nil branch leaves the weak native target unchanged.
  if (!delegate) { return; }
  _delegate = delegate;
  if (_observer) {
    if (Profile* profile = ArcProfileFrom(owner)) {
      if (auto* target = static_cast<FilenameTarget*>(profile->GetUserData(kFilenameTargetKey)))
        target->delegate = delegate;
    }
    _observer->SetDelegate(delegate);
    return;
  }
  Profile* profile = ArcProfileFrom(owner);
  if (!profile) { return; }
  auto target = std::make_unique<FilenameTarget>();
  target->service = self;
  target->owner = owner;
  target->delegate = delegate;
  profile->SetUserData(kFilenameTargetKey, std::move(target));
  arcmod::GetDownloadFilenameHook() = base::BindRepeating(&DetermineFilename);
  _observer = std::make_shared<DownloadsObserver>(profile->GetDownloadManager(), owner, delegate);
  DownloadCoreServiceFactory::GetForBrowserContext(profile)->InitializeHistory(
      DownloadCoreService::DownloadHistoryLoadTrigger::kDownloadsPage);
  // Registration never calls the app synchronously, even if the manager has
  // already initialized. A weak capture also cancels delivery after teardown.
  __weak ArcBrowserContextDownloads* weakSelf = self;
  dispatch_async(dispatch_get_main_queue(), ^{
    ArcBrowserContextDownloads* strongSelf = weakSelf;
    if (strongSelf && strongSelf->_observer) { strongSelf->_observer->SendSnapshot(); }
  });
}
- (void)startDownload:(NSURL*)url error:(NSError* __autoreleasing*)error {
  __block NSError* failure;
  void (^start)(void) = ^{
    Profile* profile = ArcProfileFrom(self->_owner);
    content::DownloadManager* manager = profile ? profile->GetDownloadManager() : nullptr;
    if (!manager) {
      failure = [NSError errorWithDomain:@"company.thebrowser.Arc" code:10
          userInfo:@{NSLocalizedDescriptionKey: profile ? @"Download manager is not available"
                                                        : @"This object is detached"}];
      return;
    }
    auto parameters = std::make_unique<download::DownloadUrlParameters>(ArcGURL(url),
        net::DefineNetworkTrafficAnnotation("arc_profile_download", R"(
          semantics {
            sender: "Arc profile downloads"
            description: "Downloads a URL explicitly requested by the host application."
            trigger: "The user asks Arc to download a URL."
            data: "The requested URL and the profile's cookies."
            destination: WEBSITE
          }
          policy {
            cookies_allowed: YES
            cookies_store: "user"
            setting: "Explicit user download request."
            policy_exception_justification: "Downloads use Chromium's normal download policies."
          })"));
    // Original 0x836bb20 writes download source 6 (INTERNAL_API).
    parameters->set_download_source(download::DownloadSource::INTERNAL_API);
    manager->DownloadUrl(std::move(parameters));
  };
  if ([NSThread isMainThread]) start();
  else dispatch_sync(dispatch_get_main_queue(), start);
  if (error && failure) *error = failure;
}
@end
