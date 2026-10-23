// Copyright 2026 ArcMod. Per-profile Chromium history queries and deletion notifications.
#import "arc/api/arc_internal.h"

#include <optional>
#include <set>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "base/strings/string_number_conversions.h"
#include "base/task/cancelable_task_tracker.h"
#include "chrome/browser/history/history_service_factory.h"
#include "chrome/browser/history/top_sites_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "components/history/core/browser/history_service.h"
#include "components/history/core/browser/history_service_observer.h"
#include "components/history/core/browser/history_types.h"
#include "components/history/core/browser/top_sites.h"
#include "components/prefs/pref_service.h"

namespace {
// Arc's history date getters return the Unix epoch for a null native Time,
// rather than nil (unlike the generic ArcNSDate helper).
NSDate* HistoryDate(base::Time time) {
  return [NSDate dateWithTimeIntervalSince1970:time.InSecondsFSinceUnixEpoch()];
}
base::Time HistoryTime(NSDate* date) {
  return base::Time::FromSecondsSinceUnixEpoch(date ? date.timeIntervalSince1970 : 0);
}
void OnMain(dispatch_block_t work) {
  if ([NSThread isMainThread]) {
    work();
  } else {
    dispatch_async(dispatch_get_main_queue(), work);
  }
}
}  // namespace

ARC_EXPORT @interface ArcHistoryQueryOptions : NSObject
@property(nonatomic, strong) NSString* searchText;
@property(nonatomic, strong) NSDate* beginTime;
@property(nonatomic, strong) NSDate* endTime;
@property(nonatomic) int maxResults;
@property(nonatomic) NSInteger duplicateHandlingPolicy;
@property(nonatomic) NSInteger matchingAlgorithm;
- (history::QueryOptions)nativeQueryOptions;
@end

@implementation ArcHistoryQueryOptions {
  std::u16string _searchText;
  history::QueryOptions _query;
}
- (instancetype)init {
  if ((self = [super init])) {
    // The original constructor sets a rolling one-day range and a 100-row limit.
    _query.end_time = base::Time::Now();
    _query.begin_time = _query.end_time - base::Days(1);
    _query.max_count = 100;
  }
  return self;
}
- (NSString*)searchText { return ArcNSString(_searchText); }
- (void)setSearchText:(NSString*)text { _searchText = ArcUTF16(text); }
- (NSDate*)beginTime { return HistoryDate(_query.begin_time); }
- (void)setBeginTime:(NSDate*)date { _query.begin_time = HistoryTime(date); }
- (NSDate*)endTime { return HistoryDate(_query.end_time); }
- (void)setEndTime:(NSDate*)date { _query.end_time = HistoryTime(date); }
- (int)maxResults { return _query.max_count; }
- (void)setMaxResults:(int)value { _query.max_count = value; }
- (NSInteger)duplicateHandlingPolicy { return _query.duplicate_policy; }
- (void)setDuplicateHandlingPolicy:(NSInteger)value {
  if (value >= 0 && value < 3) {
    _query.duplicate_policy = static_cast<history::QueryOptions::DuplicateHandling>(value);
  }
}
- (NSInteger)matchingAlgorithm {
  return _query.matching_algorithm ? static_cast<NSInteger>(*_query.matching_algorithm) : 0;
}
- (void)setMatchingAlgorithm:(NSInteger)value {
  if (value == 0 || value == 1) {
    _query.matching_algorithm = static_cast<query_parser::MatchingAlgorithm>(value);
  }
}
- (history::QueryOptions)nativeQueryOptions { return _query; }
@end

ARC_EXPORT @interface ArcHistoryItem : NSObject
- (instancetype)initWithURLRow:(const history::URLRow&)row visitTime:(std::optional<base::Time>)time;
@property(nonatomic, readonly, copy) NSString* historyID;
@property(nonatomic, readonly) NSURL* url;
@property(nonatomic, readonly, copy) NSString* urlSpec;
@property(nonatomic, readonly, copy) NSString* title;
@property(nonatomic, readonly) int visitCount;
@property(nonatomic, readonly) int typedCount;
@property(nonatomic, readonly) NSDate* lastVisitTime;
@property(nonatomic, readonly) NSDate* visitTime;
@end

@implementation ArcHistoryItem {
  history::URLRow _row;
  std::optional<base::Time> _visitTime;
}
- (instancetype)initWithURLRow:(const history::URLRow&)row visitTime:(std::optional<base::Time>)time {
  if ((self = [super init])) {
    _row = row;
    _visitTime = time;
  }
  return self;
}
- (NSString*)historyID { return ArcNSString(base::NumberToString(_row.id())); }
- (NSURL*)url { return ArcNSURL(_row.url()); }
- (NSString*)urlSpec { return ArcNSString(_row.url().spec()); }
- (NSString*)title { return ArcNSString(_row.title()); }
- (int)visitCount { return _row.visit_count(); }
- (int)typedCount { return _row.typed_count(); }
- (NSDate*)lastVisitTime { return HistoryDate(_row.last_visit()); }
- (NSDate*)visitTime { return _visitTime ? HistoryDate(*_visitTime) : nil; }
@end

ARC_EXPORT @interface ArcMostVisitedURL : NSObject
- (instancetype)initWithNativeMostVisitedURL:(const history::MostVisitedURL&)site;
@property(nonatomic, readonly) NSURL* url;
@property(nonatomic, readonly, copy) NSString* urlSpec;
@property(nonatomic, readonly, copy) NSString* title;
@end

@implementation ArcMostVisitedURL {
  GURL _url;
  std::u16string _title;
}
- (instancetype)initWithNativeMostVisitedURL:(const history::MostVisitedURL&)site {
  if ((self = [super init])) {
    _url = site.url;
    _title = site.title;
  }
  return self;
}
- (NSURL*)url { return ArcNSURL(_url); }
- (NSString*)urlSpec { return ArcNSString(_url.spec()); }
- (NSString*)title { return ArcNSString(_title); }
@end

ARC_EXPORT @interface ArcHistoryDeletion : NSObject
- (instancetype)initWithNativeDeletion:(const history::DeletionInfo&)info;
@property(nonatomic, readonly) BOOL isAllHistory;
@property(nonatomic, readonly) BOOL isFromExpiration;
@property(nonatomic, readonly) BOOL hasTimeRange;
@property(nonatomic, readonly) NSDate* timeRangeStart;
@property(nonatomic, readonly) NSDate* timeRangeEnd;
@property(nonatomic, readonly) NSArray* deletedItems;
@property(nonatomic, readonly, copy) NSSet* faviconUrls;
@property(nonatomic, readonly, copy) NSSet* restrictUrls;
@property(nonatomic, readonly, copy) NSArray* deletedVisitIds;
@end

@implementation ArcHistoryDeletion {
  BOOL _allHistory;
  BOOL _expiration;
  BOOL _hasRange;
  base::Time _begin;
  base::Time _end;
  history::URLRows _rows;
  std::set<GURL> _favicons;
  std::set<GURL> _restrictUrls;
  std::set<history::VisitID> _visitIds;
  NSArray* _deletedItems;
}
- (instancetype)initWithNativeDeletion:(const history::DeletionInfo&)info {
  if ((self = [super init])) {
    _allHistory = info.IsAllHistory();
    _expiration = info.is_from_expiration();
    _hasRange = info.time_range().IsValid();
    if (_hasRange) {
      _begin = info.time_range().begin();
      _end = info.time_range().end();
    }
    if (info.restrict_urls()) { _restrictUrls = *info.restrict_urls(); }
    if (!_allHistory) {
      _rows = info.deleted_rows();
      _favicons = info.favicon_urls();
      _visitIds = info.deleted_visit_ids();
    }
  }
  return self;
}
- (BOOL)isAllHistory { return _allHistory; }
- (BOOL)isFromExpiration { return _expiration; }
- (BOOL)hasTimeRange { return _hasRange; }
- (NSDate*)timeRangeStart { return HistoryDate(_begin); }
- (NSDate*)timeRangeEnd { return HistoryDate(_end); }
- (NSArray*)deletedItems {
  if (!_deletedItems) {
    NSMutableArray* items = [NSMutableArray arrayWithCapacity:_rows.size()];
    for (const auto& row : _rows) {
      [items addObject:[[ArcHistoryItem alloc] initWithURLRow:row visitTime:std::nullopt]];
    }
    _deletedItems = [items copy];
  }
  return _deletedItems;
}
- (NSSet*)faviconUrls {
  NSMutableSet* urls = [NSMutableSet setWithCapacity:_favicons.size()];
  for (const auto& url : _favicons) {
    if (NSURL* value = ArcNSURL(url)) { [urls addObject:value]; }
  }
  return urls;
}
- (NSSet*)restrictUrls {
  NSMutableSet* urls = [NSMutableSet setWithCapacity:_restrictUrls.size()];
  for (const auto& url : _restrictUrls) {
    if (NSURL* value = ArcNSURL(url)) { [urls addObject:value]; }
  }
  return urls;
}
- (NSArray*)deletedVisitIds {
  NSMutableArray* ids = [NSMutableArray arrayWithCapacity:_visitIds.size()];
  for (history::VisitID visit : _visitIds) { [ids addObject:@(visit)]; }
  return [ids copy];
}
@end

ARC_EXPORT @interface ArcVisitCategory : NSObject
- (instancetype)initCategoryId:(NSString*)categoryId weight:(int)weight;
@property(nonatomic, readonly, copy) NSString* categoryId;
@property(nonatomic, readonly) int weight;
@end
@implementation ArcVisitCategory {
  NSString* _categoryId;
  int _weight;
}
- (instancetype)initCategoryId:(NSString*)categoryId weight:(int)weight {
  if ((self = [super init])) {
    _categoryId = [categoryId copy] ?: @"";
    _weight = weight;
  }
  return self;
}
- (NSString*)categoryId { return _categoryId; }
- (int)weight { return _weight; }
@end

@protocol ArcProfileHistoryDelegate <NSObject>
@optional
- (void)browserContext:(ArcBrowserContext*)owner didDeleteHistory:(ArcHistoryDeletion*)deletion;
@end

namespace {
// Keep the observer alive across app callbacks: a delegate can release its
// owner or change services reentrantly. No native objects escape as raw pointers.
class HistoryState final : public history::HistoryServiceObserver,
                           public std::enable_shared_from_this<HistoryState> {
 public:
  HistoryState(history::HistoryService* service, ArcBrowserContext* owner)
      : service_(service->AsWeakPtr()), owner_(owner) {
    observation_.Observe(service);
  }
  ~HistoryState() override = default;
  history::HistoryService* service() const { return service_.get(); }
  ArcBrowserContext* owner() const { return owner_; }
  void SetDelegate(id<ArcProfileHistoryDelegate> delegate) { delegate_ = delegate; }
  base::CancelableTaskTracker* tracker() { return &tracker_; }
  void OnHistoryDeletions(history::HistoryService* service,
                          const history::DeletionInfo& info) override {
    auto keep_alive = shared_from_this();
    id<ArcProfileHistoryDelegate> delegate = delegate_;
    ArcBrowserContext* owner = owner_;
    if (owner && [delegate respondsToSelector:@selector(browserContext:didDeleteHistory:)]) {
      ArcHistoryDeletion* deletion = [[ArcHistoryDeletion alloc] initWithNativeDeletion:info];
      [delegate browserContext:owner didDeleteHistory:deletion];
    }
  }
  void HistoryServiceBeingDeleted(history::HistoryService* service) override {
    observation_.Reset();
    service_.reset();
    tracker_.TryCancelAll();
  }
 private:
  base::WeakPtr<history::HistoryService> service_;
  __weak ArcBrowserContext* owner_;
  __weak id<ArcProfileHistoryDelegate> delegate_;
  base::CancelableTaskTracker tracker_;
  base::ScopedObservation<history::HistoryService, history::HistoryServiceObserver> observation_{this};
};

// Native 0x431699c / 0x4316d20: 0 success, 1 invalid URL, 2 deletion disabled.
using HistoryMutationCompletion = void (^)(NSInteger);
NSInteger DeletionPolicy(Profile* profile) {
  if (!profile) { return 1; }
  PrefService* prefs = profile->GetPrefs();
  return prefs->FindPreference("history.deleting_enabled") &&
                 !prefs->GetBoolean("history.deleting_enabled") ? 2 : 0;
}
std::optional<std::vector<GURL>> HistoryURLs(NSArray* values) {
  std::vector<GURL> urls;
  urls.reserve(values.count);
  for (id value in values) {
    GURL url = ArcGURL(value);
    if (!url.is_valid()) { return std::nullopt; }
    urls.push_back(std::move(url));
  }
  return urls;
}
}  // namespace

ARC_EXPORT @interface ArcBrowserContextHistory : NSObject
@property(nonatomic, weak) ArcBrowserContext* owner;
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
- (std::shared_ptr<HistoryState>)nativeHistoryState;
- (void)expireURLs:(NSArray*)urls begin:(base::Time)begin end:(base::Time)end
        completion:(HistoryMutationCompletion)completion;
@end

@implementation ArcBrowserContextHistory {
  __weak ArcBrowserContext* _owner;
  __weak id<ArcProfileHistoryDelegate> _delegate;
  std::shared_ptr<HistoryState> _state;
}
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context {
  if ((self = [super init])) { _owner = context; }
  return self;
}
- (instancetype)initWithBrowserContextHistory:(const void*)unused
                           forBrowserContext:(ArcBrowserContext*)context {
  return [self initWithArcBrowserContext:context];
}
- (ArcBrowserContext*)owner { return _owner; }
- (void)setOwner:(ArcBrowserContext*)owner {
  OnMain(^{
    if (self->_owner != owner) {
      self->_state.reset();
      self->_owner = owner;
    }
  });
}
- (std::shared_ptr<HistoryState>)nativeHistoryState {
  if (!_state) {
    Profile* profile = ArcProfileFrom(_owner);
    history::HistoryService* service = profile ? HistoryServiceFactory::GetForProfile(
        profile, ServiceAccessType::EXPLICIT_ACCESS) : nullptr;
    if (service) {
      _state = std::make_shared<HistoryState>(service, _owner);
      _state->SetDelegate(_delegate);
    }
  }
  return _state;
}
- (void)setDelegate:(id<ArcProfileHistoryDelegate>)delegate {
  // Arc's nil branch leaves the weak target unchanged.
  if (!delegate) { return; }
  OnMain(^{
    (void)[self owner];
    self->_delegate = delegate;
    if (auto state = [self nativeHistoryState]) { state->SetDelegate(delegate); }
  });
}
- (void)searchHistory:(ArcHistoryQueryOptions*)options completion:(void (^)(NSArray*))completion {
  OnMain(^{
    auto state = [self nativeHistoryState];
    if (!state || !state->service()) { return; }
    // nil options is a default native query, not a constructed Arc descriptor.
    history::QueryOptions native = options ? [options nativeQueryOptions] : history::QueryOptions();
    std::weak_ptr<HistoryState> weak = state;
    state->service()->QueryHistory(ArcUTF16(options.searchText), native,
        base::BindOnce(^(history::QueryResults results) {
          auto live = weak.lock();
          if (!live || !live->service() || !live->owner() || !completion) { return; }
          NSMutableArray* items = [NSMutableArray arrayWithCapacity:results.size()];
          for (const auto& result : results) {
            [items addObject:[[ArcHistoryItem alloc] initWithURLRow:result
                                                        visitTime:result.visit_time()]];
          }
          completion([items copy]);
        }), state->tracker());
  });
}
- (void)getTopSitesFromHistory:(void (^)(NSArray*))completion {
  OnMain(^{
    Profile* profile = ArcProfileFrom(self->_owner);
    scoped_refptr<history::TopSites> sites = profile ? TopSitesFactory::GetForProfile(profile) : nullptr;
    if (!sites) {
      if (completion && profile) { completion(@[]); }
      return;
    }
    __weak ArcBrowserContext* weakOwner = self->_owner;
    sites->GetMostVisitedURLs(base::BindOnce(^(const history::MostVisitedURLList& list) {
      if (!weakOwner || !completion) { return; }
      NSMutableArray* items = [NSMutableArray arrayWithCapacity:list.size()];
      for (const auto& site : list) {
        [items addObject:[[ArcMostVisitedURL alloc] initWithNativeMostVisitedURL:site]];
      }
      completion([items copy]);
    }));
  });
}
- (void)addUrlToHistory:(NSURL*)value title:(NSString*)title {
  OnMain(^{
    auto state = [self nativeHistoryState];
    GURL url = ArcGURL(value);
    if (!state || !state->service() || !url.is_valid()) { return; }
    // The recovered bridge uses the extension AddURL path, then supplies title.
    state->service()->AddPage(url, base::Time::Now(), history::SOURCE_BROWSED);
    state->service()->SetPageTitle(url, ArcUTF16(title));
  });
}
- (void)deleteUrlsFromHistory:(NSArray*)values completion:(HistoryMutationCompletion)completion {
  OnMain(^{
    auto state = [self nativeHistoryState];
    if (!state || !state->service()) { return; }
    NSInteger result = DeletionPolicy(ArcProfileFrom(self->_owner));
    auto urls = result == 0 ? HistoryURLs(values) : std::nullopt;
    if (result == 0 && !urls) { result = 1; }
    if (result == 0) { state->service()->DeleteURLs(*urls); }
    // Arc returns after queueing deletion; the observer reports persisted changes.
    if (completion) { completion(result); }
  });
}
- (void)expireURLs:(NSArray*)values begin:(base::Time)begin end:(base::Time)end
        completion:(HistoryMutationCompletion)completion {
  auto state = [self nativeHistoryState];
  if (!state || !state->service()) { return; }
  NSInteger result = DeletionPolicy(ArcProfileFrom(_owner));
  auto urls = result == 0 ? HistoryURLs(values) : std::nullopt;
  if (result == 0 && !urls) { result = 1; }
  if (result != 0) {
    if (completion) { completion(result); }
    return;
  }
  std::set<GURL> restricted(urls->begin(), urls->end());
  std::weak_ptr<HistoryState> weak = state;
  state->service()->ExpireHistoryBetween(restricted, std::nullopt, begin, end, true,
      base::BindOnce(^{
        auto live = weak.lock();
        if (live && live->service() && live->owner() && completion) { completion(0); }
      }), state->tracker());
}
- (void)expireHistoryBetween:(NSArray*)urls forBeginTime:(NSDate*)begin forEndTime:(NSDate*)end
                 completion:(HistoryMutationCompletion)completion {
  OnMain(^{ [self expireURLs:urls begin:HistoryTime(begin) end:HistoryTime(end) completion:completion]; });
}
- (void)expireHistory:(NSArray*)urls completion:(HistoryMutationCompletion)completion {
  OnMain(^{ [self expireURLs:urls begin:base::Time() end:base::Time::Max() completion:completion]; });
}
@end
