// Copyright 2026 ArcMod. Native profile bookmarks with immutable value snapshots.
#import "arc/api/arc_internal.h"

#include <optional>
#include <set>
#include <string>
#include <utility>

#include "base/functional/bind.h"
#include "base/location.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "base/strings/string_number_conversions.h"
#include "base/supports_user_data.h"
#include "base/uuid.h"
#include "chrome/browser/bookmarks/bookmark_model_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "components/bookmarks/browser/bookmark_client.h"
#include "components/bookmarks/browser/bookmark_model.h"
#include "components/bookmarks/browser/bookmark_model_load_waiter.h"
#include "components/bookmarks/browser/bookmark_model_observer.h"
#include "components/bookmarks/browser/bookmark_node.h"
#include "components/bookmarks/browser/bookmark_utils.h"
#include "components/bookmarks/common/bookmark_metrics.h"

namespace {
template <typename Work>
auto BookmarkMain(Work work) -> decltype(work()) {
  if ([NSThread isMainThread]) { return work(); }
  __block decltype(work()) result{};
  dispatch_sync(dispatch_get_main_queue(), ^{ result = work(); });
  return result;
}
void BookmarkMainVoid(dispatch_block_t work) {
  if ([NSThread isMainThread]) { work(); }
  else { dispatch_sync(dispatch_get_main_queue(), work); }
}

class BookmarkState;
struct BookmarkSnapshot {
  NSInteger type = 0;
  bool folder = false;
  bool url_node = false;
  std::string id;
  std::string uuid;
  std::string parent_id;
  int index = -1;
  GURL url;
  std::u16string title;
  GURL favicon;
  base::Time added;
  base::Time modified;
};
BookmarkSnapshot Snapshot(const bookmarks::BookmarkNode* node) {
  BookmarkSnapshot value;
  if (!node) { return value; }
  value.type = static_cast<NSInteger>(node->type()) + 1;
  value.folder = node->is_folder();
  value.url_node = node->is_url();
  value.id = base::NumberToString(node->id());
  value.uuid = node->uuid().AsLowercaseString();
  if (const auto* parent = node->parent()) {
    value.parent_id = base::NumberToString(parent->id());
    if (auto index = parent->GetIndexOf(node)) { value.index = static_cast<int>(*index); }
  }
  if (node->is_url()) { value.url = node->url(); }
  value.title = node->GetTitle();
  if (const GURL* icon = node->icon_url()) { value.favicon = *icon; }
  value.added = node->date_added();
  value.modified = node->date_folder_modified();
  return value;
}
}  // namespace

ARC_EXPORT @interface ArcBookmarkTreeNode : NSObject
- (instancetype)initWithBookmarkState:(const std::shared_ptr<BookmarkState>&)state
                                 node:(const bookmarks::BookmarkNode*)node;
- (BookmarkSnapshot)bookmarkSnapshot;
- (const bookmarks::BookmarkNode*)nativeNodeForState:(const std::shared_ptr<BookmarkState>&)state;
@property(nonatomic, readonly) NSInteger nodeType;
@property(nonatomic, readonly) BOOL isFolder;
@property(nonatomic, readonly) BOOL isUrl;
@property(nonatomic, readonly) NSString* nodeID;
@property(nonatomic, readonly) NSString* uuid;
@property(nonatomic, readonly) NSString* parentNodeID;
@property(nonatomic, readonly) int index;
@property(nonatomic, readonly) NSURL* url;
@property(nonatomic, readonly) NSString* title;
@property(nonatomic, readonly) NSURL* faviconUrl;
@property(nonatomic, readonly) NSDate* dateAdded;
@property(nonatomic, readonly) NSDate* dateFolderModified;
@property(nonatomic, readonly) BOOL isValid;
@property(nonatomic, readonly) NSArray* liveChildren;
@end

// These sends were recovered from original ArcBookmarkModelObserverBridge's
// vtable at 0xda87ad0 (callbacks 0x43f83a4..0x43f878c). No context argument is sent.
@protocol ArcProfileBookmarkObserver <NSObject>
@optional
- (void)bookmarkModelLoaded;
- (void)bookmarkNodeMovedFromParent:(ArcBookmarkTreeNode*)oldParent oldIndex:(NSUInteger)oldIndex
                         toParent:(ArcBookmarkTreeNode*)newParent newIndex:(NSUInteger)newIndex;
- (void)bookmarkNodeAddedToParent:(ArcBookmarkTreeNode*)parent atIndex:(NSUInteger)index
                    addedByUser:(BOOL)addedByUser;
- (void)bookmarkNodeRemovedFromParent:(ArcBookmarkTreeNode*)parent oldIndex:(NSUInteger)index
                                node:(ArcBookmarkTreeNode*)node noLongerBookmarkedURLs:(NSSet*)urls;
- (void)bookmarkNodeChanged:(ArcBookmarkTreeNode*)node;
- (void)bookmarkMetaInfoChanged:(ArcBookmarkTreeNode*)node;
- (void)bookmarkNodeFaviconChanged:(ArcBookmarkTreeNode*)node;
- (void)bookmarkNodeChildrenReordered:(ArcBookmarkTreeNode*)node;
@end

namespace {
class BookmarkState final : public bookmarks::BookmarkModelObserver,
                            public std::enable_shared_from_this<BookmarkState> {
 public:
  explicit BookmarkState(bookmarks::BookmarkModel* model) : model_(model->AsWeakPtr()) {
    removing_ = [NSMutableDictionary dictionary];
    observers_ = [NSMapTable weakToStrongObjectsMapTable];
    observation_.Observe(model);
  }
  bookmarks::BookmarkModel* model() const { return model_.get(); }
  bool ready() const { return model_ && model_->loaded(); }
  const bookmarks::BookmarkNode* Resolve(int64_t id) const {
    return ready() ? bookmarks::GetBookmarkNodeByID(model_.get(), id) : nullptr;
  }
  ArcBookmarkTreeNode* Wrap(const bookmarks::BookmarkNode* node) {
    if (!node) { return nil; }
    // Original 0x42b01d4..0x42b01f8 creates a fresh value Impl for each
    // liveChildren result. Cached fields remain frozen while native operations
    // resolve the model node through a weak state reference and stable identity.
    return [[ArcBookmarkTreeNode alloc] initWithBookmarkState:shared_from_this() node:node];
  }
  NSArray* Children(const bookmarks::BookmarkNode* node) {
    NSMutableArray* children = [NSMutableArray array];
    if (node) {
      for (const auto& child : node->children()) {
        // The original liveChildren implementation filters the node's own visibility.
        if (child->IsVisible()) { [children addObject:Wrap(child.get())]; }
      }
    }
    return [children copy];
  }
  void AddObserver(ArcBrowserContext* context, id<ArcProfileBookmarkObserver> observer) {
    if (!context || !observer) return;
    NSHashTable* targets = [observers_ objectForKey:context];
    if (!targets) {
      targets = [NSHashTable weakObjectsHashTable];
      [observers_ setObject:targets forKey:context];
    }
    [targets addObject:observer];
  }
  void RemoveObserver(ArcBrowserContext* context, id<ArcProfileBookmarkObserver> observer) {
    [[observers_ objectForKey:context] removeObject:observer];
  }
  NSArray* Observers() {
    NSMutableArray* targets = [NSMutableArray array];
    for (ArcBrowserContext* context in observers_.keyEnumerator) {
      if (ArcProfileFrom(context))
        [targets addObjectsFromArray:[[observers_ objectForKey:context] allObjects]];
    }
    return targets;
  }
  void BookmarkModelLoaded(bool ids_reassigned) override {
    auto alive = shared_from_this();
    for (id<ArcProfileBookmarkObserver> observer in Observers()) {
      if ([observer respondsToSelector:@selector(bookmarkModelLoaded)]) { [observer bookmarkModelLoaded]; }
    }
  }
  void BookmarkModelBeingDeleted() override {
    auto alive = shared_from_this();
    [removing_ removeAllObjects];
    observation_.Reset();
    model_.reset();
  }
  void BookmarkNodeMoved(const bookmarks::BookmarkNode* old_parent, size_t old_index,
                        const bookmarks::BookmarkNode* new_parent, size_t new_index) override {
    auto alive = shared_from_this();
    ArcBookmarkTreeNode* old_wrapper = Wrap(old_parent);
    ArcBookmarkTreeNode* new_wrapper = Wrap(new_parent);
    for (id<ArcProfileBookmarkObserver> observer in Observers()) {
      if ([observer respondsToSelector:@selector(bookmarkNodeMovedFromParent:oldIndex:toParent:newIndex:)]) {
        [observer bookmarkNodeMovedFromParent:old_wrapper oldIndex:old_index
                                    toParent:new_wrapper newIndex:new_index];
      }
    }
  }
  void BookmarkNodeAdded(const bookmarks::BookmarkNode* parent, size_t index, bool added_by_user) override {
    auto alive = shared_from_this();
    ArcBookmarkTreeNode* wrapper = Wrap(parent);
    for (id<ArcProfileBookmarkObserver> observer in Observers()) {
      if ([observer respondsToSelector:@selector(bookmarkNodeAddedToParent:atIndex:addedByUser:)]) {
        [observer bookmarkNodeAddedToParent:wrapper atIndex:index addedByUser:added_by_user];
      }
    }
  }
  void OnWillRemoveBookmarks(const bookmarks::BookmarkNode* parent, size_t index,
                            const bookmarks::BookmarkNode* node, const base::Location& location) override {
    // Preserve the pre-removal value payload without permanently detaching
    // retained snapshots. isValid follows current model membership; whether an
    // undo can restore validity is intentionally left to native model identity.
    removing_[@(node->id())] = Wrap(node);
  }
  void BookmarkNodeRemoved(const bookmarks::BookmarkNode* parent, size_t index,
                           const bookmarks::BookmarkNode* node, const std::set<GURL>& urls,
                           const base::Location& location) override {
    auto alive = shared_from_this();
    ArcBookmarkTreeNode* parent_wrapper = Wrap(parent);
    ArcBookmarkTreeNode* removed_wrapper = removing_[@(node->id())];
    if (!removed_wrapper) {
      removed_wrapper = [[ArcBookmarkTreeNode alloc] initWithBookmarkState:alive node:node];
    }
    [removing_ removeObjectForKey:@(node->id())];
    NSMutableSet* strings = [NSMutableSet setWithCapacity:urls.size()];
    for (const GURL& url : urls) { [strings addObject:ArcNSString(url.spec())]; }
    for (id<ArcProfileBookmarkObserver> observer in Observers()) {
      if ([observer respondsToSelector:
          @selector(bookmarkNodeRemovedFromParent:oldIndex:node:noLongerBookmarkedURLs:)]) {
        [observer bookmarkNodeRemovedFromParent:parent_wrapper oldIndex:index node:removed_wrapper
                         noLongerBookmarkedURLs:strings];
      }
    }
  }
  void BookmarkNodeChanged(const bookmarks::BookmarkNode* node) override {
    NotifyNode(node, @selector(bookmarkNodeChanged:));
  }
  void BookmarkMetaInfoChanged(const bookmarks::BookmarkNode* node) override {
    NotifyNode(node, @selector(bookmarkMetaInfoChanged:));
  }
  void BookmarkNodeFaviconChanged(const bookmarks::BookmarkNode* node) override {
    NotifyNode(node, @selector(bookmarkNodeFaviconChanged:));
  }
  void BookmarkNodeChildrenReordered(const bookmarks::BookmarkNode* node) override {
    NotifyNode(node, @selector(bookmarkNodeChildrenReordered:));
  }
  void BookmarkAllUserNodesRemoved(const std::set<GURL>& removed_urls,
                                   const base::Location& location) override {
    // Arc's recovered ObjC bridge has no bulk-removal callback selector.
  }
 private:
  void NotifyNode(const bookmarks::BookmarkNode* node, SEL selector) {
    auto alive = shared_from_this();
    ArcBookmarkTreeNode* wrapper = Wrap(node);
    for (id<ArcProfileBookmarkObserver> observer in Observers()) {
      if (![observer respondsToSelector:selector]) { continue; }
      if (selector == @selector(bookmarkNodeChanged:)) { [observer bookmarkNodeChanged:wrapper]; }
      else if (selector == @selector(bookmarkMetaInfoChanged:)) { [observer bookmarkMetaInfoChanged:wrapper]; }
      else if (selector == @selector(bookmarkNodeFaviconChanged:)) { [observer bookmarkNodeFaviconChanged:wrapper]; }
      else { [observer bookmarkNodeChildrenReordered:wrapper]; }
    }
  }
  base::WeakPtr<bookmarks::BookmarkModel> model_;
  NSMutableDictionary<NSNumber*, ArcBookmarkTreeNode*>* __strong removing_;
  NSMapTable<ArcBrowserContext*, NSHashTable<id<ArcProfileBookmarkObserver>>*>* __strong observers_;
  base::ScopedObservation<bookmarks::BookmarkModel, bookmarks::BookmarkModelObserver> observation_{this};
};

struct BookmarkStateHolder final : base::SupportsUserData::Data {
  explicit BookmarkStateHolder(std::shared_ptr<BookmarkState> value) : state(std::move(value)) {}
  std::shared_ptr<BookmarkState> state;
};
constexpr char kBookmarkState[] = "ArcProfileBookmarkState";
std::shared_ptr<BookmarkState> StateFor(ArcBrowserContext* context) {
  Profile* profile = ArcProfileFrom(context);
  if (!profile) { return {}; }
  // BookmarkModelFactory redirects incognito to the regular profile. Share the
  // model state with that same owner; each lookup still returns a value snapshot.
  profile = profile->GetOriginalProfile();
  auto* holder = static_cast<BookmarkStateHolder*>(profile->GetUserData(kBookmarkState));
  if (!holder) {
    auto* model = BookmarkModelFactory::GetForBrowserContext(profile);
    if (!model) { return {}; }
    auto owned = std::make_unique<BookmarkStateHolder>(std::make_shared<BookmarkState>(model));
    holder = owned.get();
    profile->SetUserData(kBookmarkState, std::move(owned));
  }
  return holder->state;
}
const bookmarks::BookmarkNode* NodeFor(ArcBookmarkTreeNode* wrapper,
                                      const std::shared_ptr<BookmarkState>& state) {
  return [wrapper isKindOfClass:[ArcBookmarkTreeNode class]] ? [wrapper nativeNodeForState:state] : nullptr;
}
bool Editable(bookmarks::BookmarkModel* model, const bookmarks::BookmarkNode* node) {
  return model && node && !model->is_permanent_node(node) && !model->client()->IsNodeManaged(node);
}
bool InsertionParent(bookmarks::BookmarkModel* model, const bookmarks::BookmarkNode* node, NSInteger index) {
  return model && node && node->is_folder() && !model->is_root_node(node) &&
      !model->client()->IsNodeManaged(node) && index >= 0 &&
      static_cast<size_t>(index) <= node->children().size();
}
std::optional<base::Uuid> BookmarkUUID(NSString* string) {
  if (!string) { return std::nullopt; }
  base::Uuid uuid = base::Uuid::ParseCaseInsensitive(ArcUTF8(string));
  return uuid.is_valid() ? std::make_optional(uuid) : std::nullopt;
}
bool UUIDAvailable(bookmarks::BookmarkModel* model, const bookmarks::BookmarkNode* parent,
                   const std::optional<base::Uuid>& uuid) {
  if (!uuid) { return true; }
  bool account = (model->account_bookmark_bar_node() && parent->HasAncestor(model->account_bookmark_bar_node())) ||
      (model->account_other_node() && parent->HasAncestor(model->account_other_node())) ||
      (model->account_mobile_node() && parent->HasAncestor(model->account_mobile_node()));
  auto type = account ? bookmarks::BookmarkModel::NodeTypeForUuidLookup::kAccountNodes :
      bookmarks::BookmarkModel::NodeTypeForUuidLookup::kLocalOrSyncableNodes;
  return !model->GetNodeByUuid(*uuid, type);
}
}  // namespace

@implementation ArcBookmarkTreeNode {
  std::weak_ptr<BookmarkState> _state;
  int64_t _id;
  BOOL _hasNode;
  BookmarkSnapshot _snapshot;
}
- (instancetype)initWithBookmarkState:(const std::shared_ptr<BookmarkState>&)state
                                 node:(const bookmarks::BookmarkNode*)node {
  if ((self = [super init])) {
    _state = state;
    _id = node ? node->id() : 0;
    _hasNode = node != nullptr;
    _snapshot = Snapshot(node);
  }
  return self;
}
- (const bookmarks::BookmarkNode*)nativeNodeForState:(const std::shared_ptr<BookmarkState>&)state {
  if (!_hasNode || !state || _state.lock() != state) { return nullptr; }
  const auto* node = state->Resolve(_id);
  // Original 0x42affa0 also checks UUID and node ID when resolving live nodes.
  return node && node->uuid().AsLowercaseString() == _snapshot.uuid ? node : nullptr;
}
- (BookmarkSnapshot)bookmarkSnapshot {
  // Original scalar getters read the value Impl without refreshing its fields.
  return _snapshot;
}
- (NSInteger)nodeType { return [self bookmarkSnapshot].type; }
- (BOOL)isFolder { return [self bookmarkSnapshot].folder; }
- (BOOL)isUrl { return [self bookmarkSnapshot].url_node; }
- (NSString*)nodeID { return ArcNSString([self bookmarkSnapshot].id); }
- (NSString*)uuid { return ArcNSString([self bookmarkSnapshot].uuid); }
- (NSString*)parentNodeID {
  auto snapshot = [self bookmarkSnapshot];
  return snapshot.parent_id.empty() ? nil : ArcNSString(snapshot.parent_id);
}
- (int)index { return [self bookmarkSnapshot].index; }
- (NSURL*)url { return ArcNSURL([self bookmarkSnapshot].url); }
- (NSString*)title {
  auto snapshot = [self bookmarkSnapshot];
  return snapshot.title.empty() ? nil : ArcNSString(snapshot.title);
}
- (NSURL*)faviconUrl { return ArcNSURL([self bookmarkSnapshot].favicon); }
- (NSDate*)dateAdded {
  return [NSDate dateWithTimeIntervalSince1970:[self bookmarkSnapshot].added.InSecondsFSinceUnixEpoch()];
}
- (NSDate*)dateFolderModified {
  BookmarkSnapshot snapshot = [self bookmarkSnapshot];
  return snapshot.folder ? [NSDate dateWithTimeIntervalSince1970:snapshot.modified.InSecondsFSinceUnixEpoch()] : nil;
}
- (BOOL)isValid {
  return BookmarkMain([&]() -> BOOL {
    auto state = _state.lock();
    return [self nativeNodeForState:state] != nullptr;
  });
}
- (NSArray*)liveChildren {
  return BookmarkMain([&]() -> NSArray* {
    auto state = _state.lock();
    return state ? state->Children([self nativeNodeForState:state]) : @[];
  });
}
@end

@implementation ArcBrowserContext (ProfileBookmarks)
- (ArcBookmarkTreeNode*)getBookmarksTree {
  return BookmarkMain([&]() -> ArcBookmarkTreeNode* {
    auto state = StateFor(self);
    return state && state->ready() ? state->Wrap(state->model()->root_node()) : nil;
  });
}
- (void)getImportedBookmarksTree:(void (^)(ArcBookmarkTreeNode*))completion {
  BookmarkMainVoid(^{
    auto state = StateFor(self);
    if (!state || !state->model()) {
      if (completion) { completion([[ArcBookmarkTreeNode alloc] init]); }
      return;
    }
    std::weak_ptr<BookmarkState> weak = state;
    auto deliver = ^{
      auto live = weak.lock();
      if (!live || !live->ready() || !completion) { return; }
      const bookmarks::BookmarkNode* node = live->model()->bookmark_bar_node();
      // Original 0x42c9d58 unwraps the sole imported folder/entry, if present.
      if (node && node->children().size() == 1) { node = node->children().front().get(); }
      completion(live->Wrap(node));
    };
    if (state->ready()) { deliver(); }
    else { bookmarks::ScheduleCallbackOnBookmarkModelLoad(*state->model(), base::BindOnce(deliver)); }
  });
}
- (void)clearBookmarks {
  BookmarkMainVoid(^{
    auto state = StateFor(self);
    if (state && state->ready()) { state->model()->RemoveAllUserBookmarks(FROM_HERE); }
  });
}
- (BOOL)addBookmark:(NSString*)title withUrl:(NSURL*)value withParent:(ArcBookmarkTreeNode*)parent
 withIndexInParent:(NSInteger)index withUuid:(NSString*)uuidString {
  return BookmarkMain([&]() -> BOOL {
    auto state = StateFor(self);
    if (!state || !state->ready()) { return NO; }
    auto* model = state->model();
    const auto* native_parent = NodeFor(parent, state);
    GURL url = ArcGURL(value);
    auto uuid = BookmarkUUID(uuidString);
    if (!url.is_valid() || !InsertionParent(model, native_parent, index) ||
        !UUIDAvailable(model, native_parent, uuid)) { return NO; }
    return model->AddURL(native_parent, static_cast<size_t>(index), ArcUTF16(title), url,
                         nullptr, std::nullopt, uuid, false) != nullptr;
  });
}
- (BOOL)addBookmarkFolder:(NSString*)title withParent:(ArcBookmarkTreeNode*)parent
       withIndexInParent:(NSInteger)index withUuid:(NSString*)uuidString {
  return BookmarkMain([&]() -> BOOL {
    auto state = StateFor(self);
    if (!state || !state->ready()) { return NO; }
    auto* model = state->model();
    const auto* native_parent = NodeFor(parent, state);
    auto uuid = BookmarkUUID(uuidString);
    if (!InsertionParent(model, native_parent, index) || !UUIDAvailable(model, native_parent, uuid)) { return NO; }
    return model->AddFolder(native_parent, static_cast<size_t>(index), ArcUTF16(title),
                            nullptr, std::nullopt, uuid) != nullptr;
  });
}
- (BOOL)renameBookmark:(ArcBookmarkTreeNode*)bookmark withTitle:(NSString*)title {
  return BookmarkMain([&]() -> BOOL {
    auto state = StateFor(self);
    if (!state || !state->ready()) { return NO; }
    auto* model = state->model();
    const auto* node = NodeFor(bookmark, state);
    if (!Editable(model, node)) { return NO; }
    model->SetTitle(node, ArcUTF16(title), bookmarks::metrics::BookmarkEditSource::kUser);
    return YES;
  });
}
- (BOOL)editBookmark:(ArcBookmarkTreeNode*)bookmark withNewURL:(NSURL*)value {
  return BookmarkMain([&]() -> BOOL {
    auto state = StateFor(self);
    if (!state || !state->ready()) { return NO; }
    auto* model = state->model();
    const auto* node = NodeFor(bookmark, state);
    GURL url = ArcGURL(value);
    if (!Editable(model, node) || !node->is_url() || !url.is_valid()) { return NO; }
    model->SetURL(node, url, bookmarks::metrics::BookmarkEditSource::kUser);
    return YES;
  });
}
- (BOOL)removeBookmark:(ArcBookmarkTreeNode*)bookmark withParent:(ArcBookmarkTreeNode*)parent {
  return BookmarkMain([&]() -> BOOL {
    auto state = StateFor(self);
    if (!state || !state->ready()) { return NO; }
    auto* model = state->model();
    const auto* node = NodeFor(bookmark, state);
    // The ObjC bridge requires a parent wrapper, but original 0x42ca6d4
    // removes by target node and does not consult the parent native value.
    if (!Editable(model, node) || ![parent isKindOfClass:[ArcBookmarkTreeNode class]]) { return NO; }
    model->Remove(node, bookmarks::metrics::BookmarkEditSource::kUser, FROM_HERE);
    return YES;
  });
}
- (BOOL)moveBookmark:(ArcBookmarkTreeNode*)bookmark newParent:(ArcBookmarkTreeNode*)parent
             atIndex:(NSInteger)index {
  return BookmarkMain([&]() -> BOOL {
    auto state = StateFor(self);
    if (!state || !state->ready()) { return NO; }
    auto* model = state->model();
    const auto* node = NodeFor(bookmark, state);
    const auto* native_parent = NodeFor(parent, state);
    if (!Editable(model, node) || !InsertionParent(model, native_parent, index) || native_parent->HasAncestor(node)) {
      return NO;
    }
    model->Move(node, native_parent, static_cast<size_t>(index));
    return YES;
  });
}
- (BOOL)isBookmarked:(NSURL*)value {
  return BookmarkMain([&]() -> BOOL {
    auto state = StateFor(self);
    GURL url = ArcGURL(value);
    return state && state->ready() && url.is_valid() && state->model()->IsBookmarked(url);
  });
}
- (void)addBookmarkModelObserver:(id<ArcProfileBookmarkObserver>)observer {
  BookmarkMainVoid(^{ if (auto state = StateFor(self)) { state->AddObserver(self, observer); } });
}
- (void)removeBookmarkModelObserver:(id<ArcProfileBookmarkObserver>)observer {
  BookmarkMainVoid(^{ if (auto state = StateFor(self)) { state->RemoveObserver(self, observer); } });
}
@end
