// Copyright 2026 ArcMod. Native downloads and the profile's download snapshot/deltas.
#import "arc/api/arc_internal.h"

#include "base/memory/raw_ptr.h"
#include "base/scoped_multi_source_observation.h"
#include "base/scoped_observation.h"
#include "base/supports_user_data.h"
#include "chrome/browser/profiles/profile.h"
#include "components/download/public/common/download_item.h"
#include "content/public/browser/download_manager.h"

ARC_EXPORT @interface ArcDownloadItem : NSObject
+ (instancetype)wrapperForDownload:(download::DownloadItem*)item;
@end

namespace {
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
    holder = owned.get();
    item->SetUserData(kDownloadWrapperKey, std::move(owned));
  }
  return holder->wrapper;
}
- (int64_t)completedBytes { return _lifetime && _lifetime->item() ? _lifetime->item()->GetReceivedBytes() : 0; }
- (int64_t)totalBytes { return _lifetime && _lifetime->item() ? _lifetime->item()->GetTotalBytes() : 0; }
- (int64_t)currentSpeed { return _lifetime && _lifetime->item() ? _lifetime->item()->CurrentSpeed() : 0; }
- (NSString*)targetPath {
  return _lifetime && _lifetime->item() ? ArcNSString(_lifetime->item()->GetTargetFilePath().value()) : @"";
}
- (NSString*)sourceURL {
  return _lifetime && _lifetime->item() ? ArcNSString(_lifetime->item()->GetURL().spec()) : @"";
}
- (NSString*)id { return _lifetime && _lifetime->item() ? ArcNSString(_lifetime->item()->GetGuid()) : @""; }
- (NSDate*)startTime {
  return _lifetime && _lifetime->item() ? ArcNSDate(_lifetime->item()->GetStartTime()) : nil;
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
- (void)pause { if (_lifetime && _lifetime->item()) { _lifetime->item()->Pause(); } }
- (void)cancel:(BOOL)userCancel {
  if (_lifetime && _lifetime->item()) { _lifetime->item()->Cancel(userCancel); }
}
- (void)resume { if (_lifetime && _lifetime->item()) { _lifetime->item()->Resume(true); } }
- (void)remove { if (_lifetime && _lifetime->item()) { _lifetime->item()->Remove(); } }
@end

@protocol ArcProfileDownloadsDelegate <NSObject>
@optional
- (void)browserContext:(ArcBrowserContext*)context downloadManagerInitializedWithItems:(NSArray*)items;
- (void)browserContext:(ArcBrowserContext*)context onDownloadCreated:(ArcDownloadItem*)item;
- (void)browserContext:(ArcBrowserContext*)context onDownloadUpdated:(ArcDownloadItem*)item;
- (void)browserContext:(ArcBrowserContext*)context onDownloadRemoved:(ArcDownloadItem*)item;
@end

ARC_EXPORT @interface ArcBrowserContextDownloads : NSObject
@property(nonatomic, weak) ArcBrowserContext* owner;
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
@end

namespace {
// The native observer owns no ObjC delegate or context. Profile shutdown detaches
// the manager; individual download destruction detaches each item observation.
class DownloadsObserver final : public content::DownloadManager::Observer,
                                public download::DownloadItem::Observer {
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
    if (!items_.IsObservingSource(item)) { items_.AddObservation(item); }
    // History loading is represented by the initial snapshot.
    if (!snapshot_sent_) { return; }
    id<ArcProfileDownloadsDelegate> delegate = delegate_;
    if ([delegate respondsToSelector:@selector(browserContext:onDownloadCreated:)]) {
      [delegate browserContext:owner_ onDownloadCreated:[ArcDownloadItem wrapperForDownload:item]];
    }
  }
  void OnDownloadUpdated(download::DownloadItem* item) override {
    if (!snapshot_sent_) { return; }
    id<ArcProfileDownloadsDelegate> delegate = delegate_;
    if ([delegate respondsToSelector:@selector(browserContext:onDownloadUpdated:)]) {
      [delegate browserContext:owner_ onDownloadUpdated:[ArcDownloadItem wrapperForDownload:item]];
    }
  }
  void OnDownloadRemoved(download::DownloadItem* item) override {
    if (!snapshot_sent_) { return; }
    id<ArcProfileDownloadsDelegate> delegate = delegate_;
    if ([delegate respondsToSelector:@selector(browserContext:onDownloadRemoved:)]) {
      [delegate browserContext:owner_ onDownloadRemoved:[ArcDownloadItem wrapperForDownload:item]];
    }
  }
  void OnDownloadDestroyed(download::DownloadItem* item) override {
    items_.RemoveObservation(item);
  }
 private:
  raw_ptr<content::DownloadManager> manager_;
  __weak ArcBrowserContext* owner_;
  __weak id<ArcProfileDownloadsDelegate> delegate_;
  bool snapshot_sent_ = false;
  base::ScopedMultiSourceObservation<download::DownloadItem, download::DownloadItem::Observer> items_{this};
};
}  // namespace

@implementation ArcBrowserContextDownloads {
  __weak ArcBrowserContext* _owner;
  __weak id<ArcProfileDownloadsDelegate> _delegate;
  std::unique_ptr<DownloadsObserver> _observer;
}
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
    _observer->SetDelegate(delegate);
    return;
  }
  Profile* profile = ArcProfileFrom(owner);
  if (!profile) { return; }
  _observer = std::make_unique<DownloadsObserver>(profile->GetDownloadManager(), owner, delegate);
  // Registration never calls the app synchronously, even if the manager has
  // already initialized. A weak capture also cancels delivery after teardown.
  __weak ArcBrowserContextDownloads* weakSelf = self;
  dispatch_async(dispatch_get_main_queue(), ^{
    ArcBrowserContextDownloads* strongSelf = weakSelf;
    if (strongSelf && strongSelf->_observer) { strongSelf->_observer->SendSnapshot(); }
  });
}
@end
