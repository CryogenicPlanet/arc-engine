// Copyright 2026 ArcMod. Per-profile extras: import, favicon writes, metrics, search-engine/auto-open/user-color
// preferences and the annotated-visit value classes.
#import "arc/api/arc_internal.h"
#include "arc/api/arc_chromium_import.h"

#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "base/files/file_enumerator.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "base/supports_user_data.h"
#include "base/task/cancelable_task_tracker.h"
#include "base/task/sequenced_task_runner.h"
#include "base/task/thread_pool.h"
#include "base/time/time.h"
#include "base/values.h"
#include "chrome/browser/affiliations/affiliation_service_factory.h"
#include "chrome/browser/autocomplete/autocomplete_classifier_factory.h"
#include "chrome/browser/bookmarks/bookmark_model_factory.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/download/download_prefs.h"
#include "chrome/browser/favicon/favicon_service_factory.h"
#include "chrome/browser/importer/external_process_importer_host.h"
#include "chrome/browser/importer/importer_progress_observer.h"
#include "chrome/browser/importer/profile_writer.h"
#include "chrome/browser/password_manager/factories/account_password_store_factory.h"
#include "chrome/browser/password_manager/factories/profile_password_store_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_attributes_entry.h"
#include "chrome/browser/profiles/profile_attributes_storage.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/themes/theme_service.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/profiles/profile_colors_util.h"
#include "chrome/grit/generated_resources.h"
#include "components/bookmarks/browser/bookmark_model.h"
#include "components/bookmarks/browser/bookmark_node.h"
#include "components/bookmarks/browser/bookmark_utils.h"
#include "components/favicon/core/favicon_service.h"
#include "components/favicon_base/favicon_types.h"
#include "components/history/core/browser/history_types.h"
#include "components/omnibox/browser/autocomplete_classifier.h"
#include "components/omnibox/browser/autocomplete_match.h"
#include "components/password_manager/core/browser/import/import_results.h"
#include "components/password_manager/core/browser/import/password_importer.h"
#include "components/password_manager/core/browser/password_store/password_store_interface.h"
#include "components/password_manager/core/browser/ui/saved_passwords_presenter.h"
#include "components/search_engines/template_url.h"
#include "components/search_engines/template_url_data.h"
#include "components/search_engines/template_url_service.h"
#include "components/site_engagement/content/site_engagement_service.h"
#include "components/url_formatter/url_formatter.h"
#include "components/user_data_importer/common/imported_bookmark_entry.h"
#include "components/user_data_importer/common/importer_data_types.h"
#include "components/user_data_importer/common/importer_type.h"
#include "third_party/metrics_proto/omnibox_event.pb.h"
#include "third_party/sqlite/sqlite3.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/l10n/l10n_util_mac.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/text_elider.h"

namespace {

constexpr NSInteger kErrorCode = 1;

NSError* MakeError(NSString* message) {
  return [NSError errorWithDomain:@"company.thebrowser.Arc"
                             code:kErrorCode
                         userInfo:@{NSLocalizedDescriptionKey : message}];
}

BOOL Fail(NSError** error, NSString* message) {
  if (error) {
    *error = MakeError(message);
  }
  return NO;
}

void AsyncMain(void (^block)(void)) {
  dispatch_async(dispatch_get_main_queue(), block);
}

// Icon types cross the boundary as a bitmask of favicon_base::IconType values 1...5 (bit i - 1), like the
// favicon readers in arc_profile_services.mm.
favicon_base::IconTypeSet IconTypesFromMask(NSInteger mask) {
  favicon_base::IconTypeSet types;
  for (int i = 1; i <= 5; ++i) {
    if (mask & (1 << (i - 1))) {
      types.insert(static_cast<favicon_base::IconType>(i));
    }
  }
  return types;
}

favicon_base::IconType SingleIconType(NSInteger mask) {
  for (int i = 1; i <= 5; ++i) {
    if (mask & (1 << (i - 1))) {
      return static_cast<favicon_base::IconType>(i);
    }
  }
  return favicon_base::IconType::kFavicon;
}

struct FaviconTrackerHolder final : base::SupportsUserData::Data {
  base::CancelableTaskTracker tracker;
};
base::CancelableTaskTracker* FaviconTracker(Profile* profile) {
  constexpr char kKey[] = "ArcProfileExtrasFaviconTracker";
  auto* holder = static_cast<FaviconTrackerHolder*>(profile->GetUserData(kKey));
  if (!holder) {
    auto owned = std::make_unique<FaviconTrackerHolder>();
    holder = owned.get();
    profile->SetUserData(kKey, std::move(owned));
  }
  return &holder->tracker;
}

favicon::FaviconService* FaviconServiceFor(Profile* profile) {
  return profile ? FaviconServiceFactory::GetForProfile(profile, ServiceAccessType::EXPLICIT_ACCESS) : nullptr;
}

}  // namespace

// ---- ArcSiteEngagementDetails / ArcMetrics ------------------------------------------------------------

ARC_EXPORT @interface ArcSiteEngagementDetails : NSObject
- (instancetype)initWithURL:(const GURL&)url score:(double)score;
@property(nonatomic, readonly) NSURL* url;
@property(nonatomic, readonly) double score;
@end

@implementation ArcSiteEngagementDetails {
  NSURL* _url;
  double _score;
}
- (instancetype)initWithURL:(const GURL&)url score:(double)score {
  if ((self = [super init])) {
    _url = ArcNSURL(url);
    _score = score;
  }
  return self;
}
- (NSURL*)url { return _url; }
- (double)score { return _score; }
@end

ARC_EXPORT @interface ArcMetrics : NSObject
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
- (NSArray<ArcSiteEngagementDetails*>*)getAllSiteEngagementDetails;
@end

@implementation ArcMetrics {
  __weak ArcBrowserContext* _owner;
}
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context {
  if ((self = [super init])) {
    _owner = context;
  }
  return self;
}
- (NSArray<ArcSiteEngagementDetails*>*)getAllSiteEngagementDetails {
  NSMutableArray* result = [NSMutableArray array];
  Profile* profile = ArcProfileFrom(_owner);
  auto* service = profile ? site_engagement::SiteEngagementService::Get(profile) : nullptr;
  if (!service) {
    return result;
  }
  for (const auto& details : service->GetAllDetails()) {
    [result addObject:[[ArcSiteEngagementDetails alloc] initWithURL:details.origin score:details.total_score]];
  }
  return result;
}
@end

// ---- ArcAnnotatedVisit --------------------------------------------------------------------------------
// A plain value record (Arc's is a flat native struct). The producers are ArcBrowserContextHistory's
// -getVisitsSince:completion: and -getVisitsForURL:completion:; the consumers -addVisit(s):completion:.

ARC_EXPORT @interface ArcAnnotatedVisit : NSObject
@property(nonatomic, strong) NSURL* url;
@property(nonatomic, strong) NSString* title;
@property(nonatomic, strong) NSDate* visitTime;
@property(nonatomic) double visitDuration;
@property(nonatomic) NSInteger transition;
@property(nonatomic, copy) NSArray* redirectChain;
@property(nonatomic) BOOL isRedirectChainStartIncomplete;
@property(nonatomic) BOOL isRedirectChainEndIncomplete;
@property(nonatomic) NSInteger referringVisitId;
@property(nonatomic) NSInteger openerVisitId;
@property(nonatomic) int windowId;
@property(nonatomic) int tabId;
@property(nonatomic) NSInteger taskId;
@property(nonatomic) NSInteger rootTaskId;
@property(nonatomic) NSInteger parentTaskId;
@property(nonatomic) int httpResponseCode;
@property(nonatomic) NSInteger browserType;
@property(nonatomic, strong) NSString* pageLanguage;
@property(nonatomic) NSInteger passwordState;
@property(nonatomic) BOOL hasUrlKeyedImage;
@property(nonatomic, copy) NSArray* categories;
@property(nonatomic, copy) NSArray* relatedSearches;
@property(nonatomic, strong) NSURL* faviconUrl;
@property(nonatomic, strong) NSURL* referrerUrl;
@property(nonatomic) NSInteger clusterId;
@property(nonatomic, getter=isHidden) BOOL hidden;
@property(nonatomic, strong) NSString* originatorCacheGuid;
@property(nonatomic) NSInteger originatorVisitId;
@end

@implementation ArcAnnotatedVisit {
  NSDate* _visitTime;
  NSArray* _redirectChain;
  NSArray* _categories;
  NSArray* _relatedSearches;
}
@synthesize url, title, visitDuration, transition, isRedirectChainStartIncomplete, isRedirectChainEndIncomplete,
    referringVisitId, openerVisitId, windowId, tabId, taskId, rootTaskId, parentTaskId, httpResponseCode,
    browserType, pageLanguage, passwordState, hasUrlKeyedImage, faviconUrl, referrerUrl, clusterId, hidden,
    originatorCacheGuid, originatorVisitId;
- (instancetype)init {
  if ((self = [super init])) {
    taskId = rootTaskId = parentTaskId = -1;
  }
  return self;
}
// An unset visit time reads as the current time, like Arc's empty record.
- (NSDate*)visitTime { return _visitTime ?: [NSDate date]; }
- (void)setVisitTime:(NSDate*)time { _visitTime = time; }
- (NSArray*)redirectChain { return _redirectChain ?: @[]; }
- (void)setRedirectChain:(NSArray*)chain { _redirectChain = [chain copy]; }
- (NSArray*)categories { return _categories ?: @[]; }
- (void)setCategories:(NSArray*)categories { _categories = [categories copy]; }
- (NSArray*)relatedSearches { return _relatedSearches ?: @[]; }
- (void)setRelatedSearches:(NSArray*)searches { _relatedSearches = [searches copy]; }
@end

// ---- Import -------------------------------------------------------------------------------------------------

@protocol ArcExtrasImportDelegate <NSObject>
@optional
- (void)didImportInitialize:(id)context;
- (void)didImportStart:(id)context;
- (void)didImportCategoryStart:(id)context withCategory:(long long)category;
- (void)didImportCategoryEnd:(id)context withCategory:(long long)category withError:(NSError*)error;
- (void)didImportEnd:(id)context withError:(NSError*)error;
@end

// Public surface of the bookmark node, import profile and target classes (arc_profile_bookmarks.mm and
// arc_app_imports.mm); used through protocols so this file needs none of their internals.
@protocol ArcExtrasBookmarkNode <NSObject>
@property(nonatomic, readonly) NSString* nodeID;
@end
@protocol ArcExtrasImportSourceProfile <NSObject>
@property(nonatomic, readonly, copy) NSString* name;
@property(nonatomic, readonly, copy) NSString* profileName;
@property(nonatomic, readonly, copy) NSString* sourcePath;
@property(nonatomic, readonly, copy) NSString* appPath;
@property(nonatomic, readonly) long long type;
@end
@protocol ArcExtrasBookmarkImportTargets <NSObject>
@property(nonatomic, strong) id<ArcExtrasBookmarkNode> targetNodeForBookmarkBarBookmarks;
@property(nonatomic, strong) id<ArcExtrasBookmarkNode> targetNodeForOtherBookmarks;
@end

namespace {

// Arc's import categories (ArcImportSourceProfile.supportedCategories, masked to 0xfff).
constexpr long long kCategoryHistory = 1 << 0;
constexpr long long kCategoryBookmarks = 1 << 1;
constexpr long long kCategoryCookies = 1 << 2;
constexpr long long kCategoryPasswords = 1 << 3;
constexpr long long kCategoryExtensions = 1 << 7;
constexpr long long kCategoryCreditCards = 1 << 8;
// Bits 9 and 10 are reported together with history by the source scanner (mask 1537) and carry no data of
// their own here.
constexpr long long kCategoryHistoryCompanions = (1 << 9) | (1 << 10);
constexpr long long kCategoryMask = 0xfff;

// ArcImportSourceProfile.type values (arc_app_imports.mm kTypes).
constexpr long long kTypeChrome = 2;
constexpr long long kTypeFirefox = 3;
constexpr long long kTypeSafari = 4;
// A folder Safari exported (File → Export Browsing Data), as Arc's app unpacks it.
constexpr long long kTypeSafariExport = 12;

// The export's bookmarks are an HTML file in its folder; Safari names it Bookmarks.html.
base::FilePath FindSafariExportBookmarks(const base::FilePath& directory) {
  std::vector<base::FilePath> found;
  base::FileEnumerator files(directory, /*recursive=*/false, base::FileEnumerator::FILES);
  for (base::FilePath path = files.Next(); !path.empty(); path = files.Next()) {
    if (!path.MatchesExtension(FILE_PATH_LITERAL(".html"))) {
      continue;
    }
    if (base::EqualsCaseInsensitiveASCII(path.BaseName().value(), "Bookmarks.html")) {
      return path;
    }
    found.push_back(path);
  }
  std::sort(found.begin(), found.end());
  return found.empty() ? base::FilePath() : found.front();
}

bool IsChromiumFamily(long long type) {
  // Chrome, Edge, Brave, Vivaldi, Opera, Opera GX, Arc.
  return type == kTypeChrome || (type >= 6 && type <= 11);
}

struct ItemMapping {
  long long category;
  user_data_importer::ImportItem item;
};
constexpr ItemMapping kItemMappings[] = {
    {kCategoryHistory, user_data_importer::HISTORY},
    {kCategoryBookmarks, user_data_importer::FAVORITES},
    {kCategoryPasswords, user_data_importer::PASSWORDS},
};

// Bookmarks go below the app-chosen target folders instead of Chromium's default "Imported" placement.
class TargetedProfileWriter final : public ProfileWriter {
 public:
  TargetedProfileWriter(Profile* profile, std::optional<int64_t> bar_id, std::optional<int64_t> other_id)
      : ProfileWriter(profile), profile_(profile->GetWeakPtr()), bar_id_(bar_id), other_id_(other_id) {}

  bool BookmarkModelIsLoaded() const override {
    auto* model = Model();
    return model && model->loaded();
  }

  void AddBookmarks(const std::vector<user_data_importer::ImportedBookmarkEntry>& bookmarks,
                    const std::u16string& top_level_folder_name) override {
    if (!bar_id_ && !other_id_) {
      ProfileWriter::AddBookmarks(bookmarks, top_level_folder_name);
      return;
    }
    auto* model = Model();
    if (!model || !model->loaded()) {
      return;
    }
    const bookmarks::BookmarkNode* bar = Target(model, bar_id_);
    const bookmarks::BookmarkNode* other = Target(model, other_id_);
    for (const auto& entry : bookmarks) {
      const bookmarks::BookmarkNode* parent = entry.in_toolbar ? (bar ? bar : other) : (other ? other : bar);
      if (!parent) {
        continue;
      }
      for (const std::u16string& name : entry.path) {
        parent = Folder(model, parent, name);
      }
      if (entry.is_folder) {
        Folder(model, parent, entry.title);
      } else if (entry.url.is_valid()) {
        model->AddURL(parent, parent->children().size(), entry.title, entry.url, nullptr, entry.creation_time);
      }
    }
  }

 private:
  ~TargetedProfileWriter() override = default;
  bookmarks::BookmarkModel* Model() const {
    return profile_ ? BookmarkModelFactory::GetForBrowserContext(profile_.get()) : nullptr;
  }
  static const bookmarks::BookmarkNode* Target(bookmarks::BookmarkModel* model, std::optional<int64_t> id) {
    return id ? bookmarks::GetBookmarkNodeByID(model, *id) : nullptr;
  }
  static const bookmarks::BookmarkNode* Folder(bookmarks::BookmarkModel* model,
                                              const bookmarks::BookmarkNode* parent,
                                              const std::u16string& title) {
    for (const auto& child : parent->children()) {
      if (child->is_folder() && child->GetTitle() == title) {
        return child.get();
      }
    }
    return model->AddFolder(parent, parent->children().size(), title);
  }

  base::WeakPtr<Profile> profile_;
  std::optional<int64_t> bar_id_;
  std::optional<int64_t> other_id_;
};

// Data read from a Chromium-family profile on disk.
struct ChromiumFamilyData {
  std::vector<user_data_importer::ImportedBookmarkEntry> bookmarks;
  bool bookmarks_ok = false;
  history::URLRows history;
  bool history_ok = false;
  arc::ChromiumSecrets secrets;
};

base::Time ChromeTime(int64_t microseconds) {
  return base::Time::FromDeltaSinceWindowsEpoch(base::Microseconds(microseconds));
}

void CollectBookmarks(const base::DictValue& node, bool in_toolbar, std::vector<std::u16string>* path,
                      std::vector<user_data_importer::ImportedBookmarkEntry>* out) {
  const std::string* type = node.FindString("type");
  if (!type) {
    return;
  }
  const std::string* name = node.FindString("name");
  user_data_importer::ImportedBookmarkEntry entry;
  entry.in_toolbar = in_toolbar;
  entry.title = name ? base::UTF8ToUTF16(*name) : std::u16string();
  entry.path = *path;
  if (const std::string* added = node.FindString("date_added")) {
    int64_t value = 0;
    if (base::StringToInt64(*added, &value) && value > 0) {
      entry.creation_time = ChromeTime(value);
    }
  }
  if (*type == "url") {
    const std::string* url = node.FindString("url");
    entry.url = url ? GURL(*url) : GURL();
    if (entry.url.is_valid()) {
      out->push_back(std::move(entry));
    }
  } else if (*type == "folder") {
    entry.is_folder = true;
    const std::u16string title = entry.title;
    out->push_back(std::move(entry));
    if (const base::ListValue* children = node.FindList("children")) {
      path->push_back(title);
      for (const base::Value& child : *children) {
        if (const base::DictValue* dict = child.GetIfDict()) {
          CollectBookmarks(*dict, in_toolbar, path, out);
        }
      }
      path->pop_back();
    }
  }
}

bool ReadChromiumBookmarks(const base::FilePath& dir, ChromiumFamilyData* data) {
  std::string contents;
  if (!base::ReadFileToString(dir.AppendASCII("Bookmarks"), &contents)) {
    return false;
  }
  std::optional<base::DictValue> root = base::JSONReader::ReadDict(contents, base::JSON_PARSE_RFC);
  const base::DictValue* roots = root ? root->FindDict("roots") : nullptr;
  if (!roots) {
    return false;
  }
  constexpr std::pair<const char*, bool> kRoots[] = {{"bookmark_bar", true}, {"other", false}, {"synced", false}};
  for (const auto& [key, in_toolbar] : kRoots) {
    const base::DictValue* top = roots->FindDict(key);
    const base::ListValue* children = top ? top->FindList("children") : nullptr;
    if (!children) {
      continue;
    }
    std::vector<std::u16string> path;
    for (const base::Value& child : *children) {
      if (const base::DictValue* dict = child.GetIfDict()) {
        CollectBookmarks(*dict, in_toolbar, &path, &data->bookmarks);
      }
    }
  }
  return true;
}

bool ReadChromiumHistory(const base::FilePath& dir, ChromiumFamilyData* data) {
  // The source browser may hold the live database open; read a private copy.
  base::FilePath copy;
  if (!base::CreateTemporaryFile(&copy)) {
    return false;
  }
  bool ok = false;
  if (base::CopyFile(dir.AppendASCII("History"), copy)) {
    sqlite3* opened = nullptr;
    if (sqlite3_open_v2(copy.value().c_str(), &opened, SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX, nullptr) ==
        SQLITE_OK) {
      sqlite3_stmt* statement = nullptr;
      if (sqlite3_prepare_v2(opened,
                             "SELECT url, title, visit_count, typed_count, last_visit_time FROM urls "
                             "WHERE hidden = 0 AND visit_count > 0",
                             -1, &statement, nullptr) == SQLITE_OK) {
        ok = true;
        while (sqlite3_step(statement) == SQLITE_ROW) {
          const char* url = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
          const char* title = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
          GURL gurl(url ? url : "");
          if (!gurl.is_valid()) {
            continue;
          }
          history::URLRow row(gurl);
          row.set_title(base::UTF8ToUTF16(title ? title : ""));
          row.set_visit_count(sqlite3_column_int(statement, 2));
          row.set_typed_count(sqlite3_column_int(statement, 3));
          row.set_last_visit(ChromeTime(sqlite3_column_int64(statement, 4)));
          data->history.push_back(std::move(row));
        }
        sqlite3_finalize(statement);
      }
      sqlite3_close(opened);
    }
  }
  base::DeleteFile(copy);
  return ok;
}

ChromiumFamilyData ReadChromiumFamily(base::FilePath dir,
                                      bool want_history,
                                      bool want_bookmarks,
                                      std::string keychain_service,
                                      bool want_cookies,
                                      bool want_passwords,
                                      bool want_cards) {
  ChromiumFamilyData data;
  data.secrets = arc::ReadChromiumSecrets(dir, keychain_service, want_cookies, want_passwords, want_cards);
  if (want_bookmarks) {
    data.bookmarks_ok = ReadChromiumBookmarks(dir, &data);
  }
  if (want_history) {
    data.history_ok = ReadChromiumHistory(dir, &data);
  }
  return data;
}

// One import run. Delegate callbacks follow ArcImportDelegate; everything runs on the main thread. The run
// keeps itself alive until it ends.
class ImportSession final : public importer::ImporterProgressObserver,
                            public std::enable_shared_from_this<ImportSession> {
 public:
  ImportSession(Profile* profile, id delegate, long long categories,
                id<ArcExtrasImportSourceProfile> source, scoped_refptr<ProfileWriter> writer,
                std::vector<std::string> allowed_extensions, bool skip_existing_cookies)
      : profile_(profile->GetWeakPtr()),
        delegate_(delegate),
        categories_(categories),
        source_(source),
        writer_(std::move(writer)),
        allowed_extensions_(std::move(allowed_extensions)),
        skip_existing_cookies_(skip_existing_cookies) {}
  ~ImportSession() override = default;

  void Start() {
    keepalive_ = shared_from_this();
    if ([delegate_ respondsToSelector:@selector(didImportInitialize:)]) {
      [delegate_ didImportInitialize:ArcImportSourceProfileCopy(source_)];
    }
    if (source_.type == kTypeFirefox || source_.type == kTypeSafari) {
      StartChromiumHost();
    } else if (source_.type == kTypeSafariExport) {
      StartSafariExport();
    } else {
      StartChromiumFamily();
    }
  }

  // importer::ImporterProgressObserver:
  void ImportStarted() override {
    // Chromium's importer host reports this twice: when it's asked to start and when its utility process begins.
    if (started_) {
      return;
    }
    NotifyStart();
    ReportUnsupported(SupportedMask());
  }
  void ImportItemStarted(user_data_importer::ImportItem item) override {
    if (long long category = CategoryFor(item)) {
      NotifyCategoryStart(category);
    }
  }
  void ImportItemEnded(user_data_importer::ImportItem item) override {
    if (long long category = CategoryFor(item)) {
      NotifyCategoryEnd(category, nil);
    }
  }
  // A cancelled import (a locked Firefox profile, a crashed utility process) can end before it starts, or with
  // categories still open.
  void ImportEnded() override {
    NSError* error = started_ && !open_categories_ ? nil : MakeError(@"The import was interrupted.");
    NotifyStart();
    for (long long bit = 1; bit <= kCategoryMask; bit <<= 1) {
      if (open_categories_ & bit) {
        NotifyCategoryEnd(bit, error);
      }
    }
    Finish(error);
  }

 private:
  static long long CategoryFor(user_data_importer::ImportItem item) {
    for (const auto& mapping : kItemMappings) {
      if (mapping.item == item) {
        return mapping.category;
      }
    }
    return 0;
  }
  // Categories Chromium's importer host (Firefox, Safari) can import.
  static long long HostMask() { return kCategoryHistory | kCategoryBookmarks | kCategoryPasswords; }
  // What this session's source can import: a Safari export's bookmarks file goes through the host's bookmarks
  // file importer; its other data isn't imported yet.
  long long SupportedMask() const {
    if (source_.type == kTypeSafariExport) {
      return safari_export_bookmarks_.empty() ? 0 : kCategoryBookmarks;
    }
    return HostMask();
  }

  // Safari export: find the bookmarks file off the main thread, then import it through the host.
  void StartSafariExport() {
    base::ThreadPool::PostTaskAndReplyWithResult(
        FROM_HERE, {base::MayBlock(), base::TaskPriority::USER_VISIBLE},
        base::BindOnce(&FindSafariExportBookmarks, base::FilePath(ArcUTF8(source_.sourcePath))),
        base::BindOnce(
            [](std::weak_ptr<ImportSession> weak, base::FilePath bookmarks) {
              if (auto session = weak.lock()) {
                session->safari_export_bookmarks_ = std::move(bookmarks);
                session->StartChromiumHost();
              }
            },
            weak_from_this()));
  }
  // What Arc's Chromium importer reads from Chrome, Edge, Brave, Vivaldi, Opera and Arc profiles.
  static long long FamilyMask() {
    return kCategoryHistory | kCategoryBookmarks | kCategoryCookies | kCategoryPasswords | kCategoryExtensions |
           kCategoryCreditCards;
  }

  // Once per import.
  void NotifyStart() {
    if (started_) {
      return;
    }
    started_ = true;
    if ([delegate_ respondsToSelector:@selector(didImportStart:)]) {
      [delegate_ didImportStart:ArcImportSourceProfileCopy(source_)];
    }
  }
  void NotifyCategoryStart(long long category) {
    open_categories_ |= category;
    if ([delegate_ respondsToSelector:@selector(didImportCategoryStart:withCategory:)]) {
      [delegate_ didImportCategoryStart:ArcImportSourceProfileCopy(source_) withCategory:category];
    }
  }
  void NotifyCategoryEnd(long long category, NSError* error) {
    open_categories_ &= ~category;
    if ([delegate_ respondsToSelector:@selector(didImportCategoryEnd:withCategory:withError:)]) {
      [delegate_ didImportCategoryEnd:ArcImportSourceProfileCopy(source_) withCategory:category withError:error];
    }
  }
  // Requested categories outside |supported| (cookies, cards, extensions, sessions, ...) end with an error.
  void ReportUnsupported(long long supported) {
    const long long mask = categories_ & ~(supported | kCategoryHistoryCompanions);
    for (long long bit = 1; bit <= kCategoryMask; bit <<= 1) {
      if (mask & bit) {
        NotifyCategoryStart(bit);
        NotifyCategoryEnd(bit, MakeError(@"This category cannot be imported from this source."));
      }
    }
  }
  void Finish(NSError* error) {
    if ([delegate_ respondsToSelector:@selector(didImportEnd:withError:)]) {
      [delegate_ didImportEnd:ArcImportSourceProfileCopy(source_) withError:error];
    }
    // Drop the self reference after the caller (possibly the importer host) has unwound.
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE, base::BindOnce([](std::shared_ptr<ImportSession>) {}, std::move(keepalive_)));
  }

  // Firefox and Safari: Chromium's importer host (utility process).
  void StartChromiumHost() {
    Profile* profile = profile_.get();
    if (!profile) {
      NotifyStart();
      Finish(MakeError(@"The profile is gone."));
      return;
    }
    user_data_importer::SourceProfile source;
    source.importer_name = ArcUTF16(source_.name);
    source.profile = ArcUTF16(source_.profileName);
    source.app_path = base::FilePath(ArcUTF8(source_.appPath));
    source.locale = g_browser_process->GetApplicationLocale();
    if (source_.type == kTypeFirefox) {
      source.importer_type = user_data_importer::TYPE_FIREFOX;
      source.source_path = base::FilePath(ArcUTF8(source_.sourcePath));
    } else if (source_.type == kTypeSafariExport) {
      source.importer_type = user_data_importer::TYPE_BOOKMARKS_FILE;
      source.source_path = safari_export_bookmarks_;
    } else {
      source.importer_type = user_data_importer::TYPE_SAFARI;
      // Chromium's Safari importer reads from the user library directory itself.
      source.source_path = base::FilePath(ArcUTF8(source_.sourcePath)).DirName();
    }
    uint16_t items = 0;
    for (const auto& mapping : kItemMappings) {
      if (categories_ & mapping.category & SupportedMask()) {
        items |= mapping.item;
      }
    }
    source.services_supported = items;
    if (!items) {
      NotifyStart();
      ReportUnsupported(SupportedMask());
      Finish(nil);
      return;
    }
    // The host deletes itself after ImportEnded. Headless: no lock dialog; a locked Firefox profile cancels.
    auto* host = new ExternalProcessImporterHost();
    host->set_headless();
    host->set_observer(this);
    host->StartImportSettings(source, profile, items, writer_.get());
  }

  // Chrome, Brave, Edge, Vivaldi, Opera, Arc: bookmarks and history read from the profile directory.
  void StartChromiumFamily() {
    NotifyStart();
    ReportUnsupported(FamilyMask());
    const long long wanted = categories_ & FamilyMask();
    if (!wanted) {
      Finish(nil);
      return;
    }
    for (long long bit = 1; bit <= kCategoryMask; bit <<= 1) {
      if (wanted & bit) {
        NotifyCategoryStart(bit);
      }
    }
    base::ThreadPool::PostTaskAndReplyWithResult(
        FROM_HERE, {base::MayBlock(), base::TaskPriority::USER_VISIBLE},
        base::BindOnce(&ReadChromiumFamily, base::FilePath(ArcUTF8(source_.sourcePath)),
                       (wanted & kCategoryHistory) != 0, (wanted & kCategoryBookmarks) != 0,
                       arc::ChromiumKeychainService(source_.type), (wanted & kCategoryCookies) != 0,
                       (wanted & kCategoryPasswords) != 0, (wanted & kCategoryCreditCards) != 0),
        base::BindOnce(
            [](std::weak_ptr<ImportSession> weak, long long wanted, ChromiumFamilyData data) {
              if (auto session = weak.lock()) {
                session->FinishChromiumFamily(wanted, std::move(data));
              }
            },
            weak_from_this(), wanted));
  }

  void FinishChromiumFamily(long long wanted, ChromiumFamilyData data) {
    const bool alive = profile_ != nullptr;
    if (wanted & kCategoryHistory) {
      if (alive && data.history_ok) {
        writer_->AddHistoryPage(data.history, history::SOURCE_OS_MIGRATION_IMPORTED);
        NotifyCategoryEnd(kCategoryHistory, nil);
      } else {
        NotifyCategoryEnd(kCategoryHistory, MakeError(@"The history could not be read."));
      }
    }
    if (wanted & kCategoryBookmarks) {
      if (alive && data.bookmarks_ok && writer_->BookmarkModelIsLoaded()) {
        writer_->AddBookmarks(data.bookmarks, u"Imported");
        NotifyCategoryEnd(kCategoryBookmarks, nil);
      } else {
        NotifyCategoryEnd(kCategoryBookmarks, MakeError(@"The bookmarks could not be imported."));
      }
    }
    if (wanted & kCategoryPasswords) {
      if (alive && data.secrets.passwords_ok) {
        for (const password_manager::PasswordForm& form : data.secrets.passwords) {
          writer_->AddPasswordForm(form);
        }
        NotifyCategoryEnd(kCategoryPasswords, nil);
      } else {
        NotifyCategoryEnd(kCategoryPasswords, MakeError(@"The passwords could not be read."));
      }
    }
    if (wanted & kCategoryCreditCards) {
      if (alive && data.secrets.cards_ok) {
        arc::WriteImportedCards(profile_.get(), data.secrets.cards);
        NotifyCategoryEnd(kCategoryCreditCards, nil);
      } else {
        NotifyCategoryEnd(kCategoryCreditCards, MakeError(@"The cards could not be read."));
      }
    }
    if (wanted & kCategoryExtensions) {
      // Installs continue from the Chrome Web Store after the import ends; the app hears about each one from
      // the extensions delegate.
      const bool scheduled = alive && arc::InstallImportedExtensions(profile_.get(), allowed_extensions_);
      NotifyCategoryEnd(kCategoryExtensions, scheduled ? nil : MakeError(@"The extensions could not be installed."));
    }
    if (!(wanted & kCategoryCookies)) {
      Finish(nil);
      return;
    }
    if (!alive || !data.secrets.cookies_ok) {
      NotifyCategoryEnd(kCategoryCookies, MakeError(@"The cookies could not be read."));
      Finish(nil);
      return;
    }
    arc::WriteImportedCookies(
        profile_.get(), std::move(data.secrets.cookies), skip_existing_cookies_,
        base::BindOnce(
            [](std::weak_ptr<ImportSession> weak, bool ok) {
              if (auto session = weak.lock()) {
                session->NotifyCategoryEnd(kCategoryCookies,
                                           ok ? nil : MakeError(@"Some cookies could not be imported."));
                session->Finish(nil);
              }
            },
            weak_from_this()));
  }

  base::WeakPtr<Profile> profile_;
  __weak id delegate_;
  const long long categories_;
  id<ArcExtrasImportSourceProfile> source_;
  scoped_refptr<ProfileWriter> writer_;
  const std::vector<std::string> allowed_extensions_;
  const bool skip_existing_cookies_;
  // Safari export: its bookmarks HTML file (empty until found, or if there's none).
  base::FilePath safari_export_bookmarks_;
  bool started_ = false;
  long long open_categories_ = 0;
  std::shared_ptr<ImportSession> keepalive_;
};

// Imports one CSV file of passwords into the profile store, then deletes itself.
class PasswordFileImport {
 public:
  static void Run(Profile* profile, const base::FilePath& path) {
    // Owns itself until the import result arrives.
    (new PasswordFileImport(profile))->Start(path);
  }

 private:
  explicit PasswordFileImport(Profile* profile)
      : presenter_(AffiliationServiceFactory::GetForProfile(profile),
                   ProfilePasswordStoreFactory::GetForProfile(profile, ServiceAccessType::EXPLICIT_ACCESS),
                   AccountPasswordStoreFactory::GetForProfile(profile, ServiceAccessType::EXPLICIT_ACCESS)),
        importer_(presenter_) {}

  void Start(const base::FilePath& path) {
    presenter_.Init(base::BindOnce(&PasswordFileImport::Import, base::Unretained(this), path));
  }
  void Import(base::FilePath path) {
    importer_.Import(path, password_manager::PasswordForm::Store::kProfileStore,
                     base::BindOnce(&PasswordFileImport::Done, base::Unretained(this)));
  }
  void Done(const password_manager::ImportResults&) { delete this; }

  password_manager::SavedPasswordsPresenter presenter_;
  password_manager::PasswordImporter importer_;
};

std::optional<int64_t> NodeIdOf(id<ArcExtrasBookmarkNode> node) {
  int64_t id = 0;
  return node && base::StringToInt64(ArcUTF8(node.nodeID), &id) ? std::optional<int64_t>(id) : std::nullopt;
}

}  // namespace

@interface ArcBrowserContext (ProfileExtras)
- (id)metrics;
@end

@implementation ArcBrowserContext (ProfileExtras)

- (id)metrics {
  return [self serviceNamed:@"ArcMetrics"];
}

- (BOOL)import:(id)sourceProfile
        withCategories:(long long)categories
 withAllowedExtensions:(NSArray*)allowedExtensions
withBookmarkImportTargets:(id)targets
   skipExistingCookies:(BOOL)skipExistingCookies
          withDelegate:(id)delegate
                 error:(NSError**)error {
  Profile* profile = ArcProfileFrom(self);
  if (!profile) {
    return Fail(error, @"The profile is gone.");
  }
  if (![sourceProfile respondsToSelector:@selector(sourcePath)] ||
      ![sourceProfile respondsToSelector:@selector(type)]) {
    return Fail(error, @"Invalid import source.");
  }
  id<ArcExtrasImportSourceProfile> source = sourceProfile;
  if (source.type != kTypeFirefox && source.type != kTypeSafari && source.type != kTypeSafariExport &&
      !IsChromiumFamily(source.type)) {
    return Fail(error, @"This import source is not supported.");
  }
  // Bookmark targets must be existing folders.
  std::optional<int64_t> bar_id;
  std::optional<int64_t> other_id;
  if ([targets respondsToSelector:@selector(targetNodeForBookmarkBarBookmarks)]) {
    id<ArcExtrasBookmarkImportTargets> import_targets = targets;
    auto* model = BookmarkModelFactory::GetForBrowserContext(profile);
    bar_id = NodeIdOf(import_targets.targetNodeForBookmarkBarBookmarks);
    other_id = NodeIdOf(import_targets.targetNodeForOtherBookmarks);
    for (const std::optional<int64_t>& id : {bar_id, other_id}) {
      if (!id) {
        continue;
      }
      const bookmarks::BookmarkNode* native =
          model && model->loaded() ? bookmarks::GetBookmarkNodeByID(model, *id) : nullptr;
      if (!native || !native->is_folder()) {
        return Fail(error, @"Invalid bookmark import target.");
      }
    }
  }
  auto writer = base::MakeRefCounted<TargetedProfileWriter>(profile, bar_id, other_id);
  std::vector<std::string> extension_ids;
  for (id extension in allowedExtensions) {
    if ([extension isKindOfClass:[NSString class]]) {
      extension_ids.push_back(ArcUTF8(extension));
    }
  }
  std::make_shared<ImportSession>(profile, delegate, categories & kCategoryMask, source, std::move(writer),
                                  std::move(extension_ids), skipExistingCookies)
      ->Start();
  return YES;
}

- (void)importPasswords:(NSString*)path {
  Profile* profile = ArcProfileFrom(self);
  if (!profile || !path.length) {
    return;
  }
  PasswordFileImport::Run(profile, base::FilePath(ArcUTF8(path)));
}

- (void)faviconTempFileForPage:(NSURL*)page
                     iconTypes:(NSInteger)types
                  sizeInPixels:(NSInteger)size
                fallbackToHost:(BOOL)fallback
                    completion:(void (^)(NSURL*))completion {
  if (!completion) {
    return;
  }
  Profile* profile = ArcProfileFrom(self);
  auto* service = FaviconServiceFor(profile);
  if (!service) {
    AsyncMain(^{ completion(nil); });
    return;
  }
  service->GetRawFaviconForPageURL(
      ArcGURL(page), IconTypesFromMask(types), static_cast<int>(size), fallback,
      base::BindOnce(^(const favicon_base::FaviconRawBitmapResult& result) {
        if (!result.is_valid()) {
          completion(nil);
          return;
        }
        NSData* data = [NSData dataWithBytes:result.bitmap_data->front() length:result.bitmap_data->size()];
        NSString* name = [[NSUUID UUID].UUIDString stringByAppendingPathExtension:@"png"];
        NSURL* file = [NSURL fileURLWithPath:[NSTemporaryDirectory() stringByAppendingPathComponent:name]];
        completion([data writeToURL:file options:NSDataWritingAtomic error:nil] ? file : nil);
      }),
      FaviconTracker(profile));
}

- (void)setFavicon:(NSImage*)image
           forPage:(NSURL*)page
           iconURL:(NSURL*)iconURL
          iconType:(NSInteger)iconType
        completion:(void (^)(BOOL))completion {
  Profile* profile = ArcProfileFrom(self);
  auto* service = FaviconServiceFor(profile);
  const GURL pageURL = ArcGURL(page);
  const BOOL ok = service && image && pageURL.is_valid();
  if (ok) {
    service->SetFavicons({pageURL}, ArcGURL(iconURL), SingleIconType(iconType), gfx::Image(image));
  }
  if (completion) {
    AsyncMain(^{ completion(ok); });
  }
}

- (void)setFaviconData:(NSData*)data
               forPage:(NSURL*)page
               iconURL:(NSURL*)iconURL
              iconType:(NSInteger)iconType
            completion:(void (^)(BOOL))completion {
  NSImage* image = data.length ? [[NSImage alloc] initWithData:data] : nil;
  [self setFavicon:image forPage:page iconURL:iconURL iconType:iconType completion:completion];
}

@end

// ---- Preferences extras -------------------------------------------------------------------------------------

ARC_EXPORT @interface ArcSearchEngine : NSObject
- (instancetype)initWithTemplateURLData:(const TemplateURLData&)data;
- (const TemplateURLData&)templateURLData;
@end

ARC_EXPORT @interface ArcPasteAndGoClassification : NSObject
- (instancetype)initWithPasteAndGoClassification:(const void*)data;
@end

ARC_EXPORT @interface ArcBrowserContextPreferences : NSObject
@property(nonatomic, weak) ArcBrowserContext* owner;
@end

namespace {

// Runs |action| now when |service| is loaded, else after it loads.
void WhenLoaded(TemplateURLService* service, base::OnceClosure action) {
  if (service->loaded()) {
    std::move(action).Run();
    return;
  }
  auto subscription = std::make_shared<base::CallbackListSubscription>();
  *subscription = service->RegisterOnLoadedCallback(base::BindOnce(
      [](std::shared_ptr<base::CallbackListSubscription>, base::OnceClosure action) { std::move(action).Run(); },
      subscription, std::move(action)));
  service->Load();
}

TemplateURLService* SearchServiceFor(ArcBrowserContext* owner) {
  Profile* profile = ArcProfileFrom(owner);
  return profile ? TemplateURLServiceFactory::GetForProfile(profile) : nullptr;
}

}  // namespace

@implementation ArcBrowserContextPreferences (ProfileExtras)

- (BOOL)addSearchEngine:(ArcSearchEngine*)engine error:(NSError**)error {
  TemplateURLService* service = SearchServiceFor(self.owner);
  if (!service || ![engine isKindOfClass:[ArcSearchEngine class]]) {
    return Fail(error, @"The search engine service is unavailable.");
  }
  TemplateURLData data = [engine templateURLData];
  if (data.short_name().empty() || data.keyword().empty() || data.url().empty()) {
    return Fail(error, @"A search engine needs a name, keyword and URL.");
  }
  data.date_created = base::Time::Now();
  data.last_modified = data.date_created;
  data.safe_for_autoreplace = false;
  WhenLoaded(service, base::BindOnce(
                          [](base::WeakPtr<Profile> profile, TemplateURLData data) {
                            auto* service = profile ? TemplateURLServiceFactory::GetForProfile(profile.get()) : nullptr;
                            if (service && !service->GetTemplateURLForKeyword(data.keyword())) {
                              service->Add(std::make_unique<TemplateURL>(data));
                            }
                          },
                          ArcProfileFrom(self.owner)->GetWeakPtr(), data));
  return YES;
}

- (BOOL)removeSearchEngine:(ArcSearchEngine*)engine error:(NSError**)error {
  TemplateURLService* service = SearchServiceFor(self.owner);
  if (!service || ![engine isKindOfClass:[ArcSearchEngine class]]) {
    return Fail(error, @"The search engine service is unavailable.");
  }
  const std::u16string keyword = [engine templateURLData].keyword();
  if (service->loaded()) {
    const TemplateURL* native = service->GetTemplateURLForKeyword(keyword);
    if (!native) {
      return Fail(error, @"The search engine was not found.");
    }
    if (native == service->GetDefaultSearchProvider()) {
      return Fail(error, @"The default search engine cannot be removed.");
    }
  }
  WhenLoaded(service, base::BindOnce(
                          [](base::WeakPtr<Profile> profile, std::u16string keyword) {
                            auto* service = profile ? TemplateURLServiceFactory::GetForProfile(profile.get()) : nullptr;
                            const TemplateURL* native = service ? service->GetTemplateURLForKeyword(keyword) : nullptr;
                            if (native && native != service->GetDefaultSearchProvider()) {
                              service->Remove(native);
                            }
                          },
                          ArcProfileFrom(self.owner)->GetWeakPtr(), keyword));
  return YES;
}

- (BOOL)setIsActiveSearchEngine:(ArcSearchEngine*)engine isActive:(BOOL)isActive error:(NSError**)error {
  TemplateURLService* service = SearchServiceFor(self.owner);
  if (!service || ![engine isKindOfClass:[ArcSearchEngine class]]) {
    return Fail(error, @"The search engine service is unavailable.");
  }
  TemplateURL* native =
      service->loaded() ? service->GetTemplateURLForKeyword([engine templateURLData].keyword()) : nullptr;
  if (!native) {
    return Fail(error, @"The search engine was not found.");
  }
  service->SetIsActiveTemplateURL(native, isActive);
  return YES;
}

- (void)setUserColor:(ArcColor*)color {
  Profile* profile = ArcProfileFrom(self.owner);
  if (!profile) {
    return;
  }
  Profile* original = profile->GetOriginalProfile();
  const std::optional<SkColor> seed = color ? std::optional<SkColor>([color skColor]) : std::nullopt;
  if (auto* theme = ThemeServiceFactory::GetForProfile(original)) {
    theme->SetUserColor(seed);
  }
  // The profile's color seed is what ArcBrowserContextAttributes.userColor reports.
  if (auto* entry = g_browser_process->profile_manager()->GetProfileAttributesStorage()
                        .GetProfileAttributesWithPath(original->GetPath())) {
    if (seed) {
      ProfileThemeColors colors = GetProfileThemeColorsForAutogeneratedColor(*seed);
      colors.profile_color_seed = *seed;
      entry->SetProfileThemeColors(colors);
    } else {
      entry->SetProfileThemeColors(std::nullopt);
    }
  }
}

- (ArcPasteAndGoClassification*)classifyPasteAndGoInput:(NSString*)input
                                   maxDisplayCharacters:(NSUInteger)maxCharacters {
  Profile* profile = ArcProfileFrom(self.owner);
  auto* classifier = profile ? AutocompleteClassifierFactory::GetForProfile(profile) : nullptr;
  const std::u16string text = ArcUTF16(input);
  if (!classifier || text.empty()) {
    return nil;
  }
  AutocompleteMatch match;
  classifier->Classify(text, /*in_keyword_mode=*/false, /*allow_exact_keyword_match=*/false,
                       metrics::OmniboxEventProto::OTHER, &match, nullptr);
  if (!match.destination_url.is_valid()) {
    return nil;
  }
  NSString* label;
  if (AutocompleteMatch::IsSearchType(match.type)) {
    label = l10n_util::FixUpWindowsStyleLabel(
        l10n_util::GetStringFUTF16(IDS_PASTE_AND_SEARCH, gfx::TruncateString(text, maxCharacters, gfx::WORD_BREAK)));
  } else {
    label = l10n_util::FixUpWindowsStyleLabel(l10n_util::GetStringFUTF16(
        IDS_PASTE_AND_GO,
        gfx::TruncateString(url_formatter::FormatUrl(match.destination_url), maxCharacters, gfx::CHARACTER_BREAK)));
  }
  const std::pair<std::string, std::string> data{ArcUTF8(label), match.destination_url.spec()};
  return [[ArcPasteAndGoClassification alloc] initWithPasteAndGoClassification:&data];
}

- (BOOL)canAutoOpenFile:(NSURL*)file {
  Profile* profile = ArcProfileFrom(self.owner);
  if (!profile || !file.isFileURL) {
    return NO;
  }
  return DownloadPrefs::FromBrowserContext(profile)->IsAutoOpenEnabled(GURL(), base::FilePath(ArcUTF8(file.path)));
}

- (BOOL)setAutoOpenFile:(NSURL*)file autoOpen:(BOOL)autoOpen error:(NSError**)error {
  Profile* profile = ArcProfileFrom(self.owner);
  if (!profile || !file.isFileURL) {
    return Fail(error, @"Invalid file.");
  }
  auto* prefs = DownloadPrefs::FromBrowserContext(profile);
  const base::FilePath path(ArcUTF8(file.path));
  if (autoOpen) {
    if (!prefs->EnableAutoOpenByUserBasedOnExtension(path)) {
      return Fail(error, @"This file type cannot be opened automatically.");
    }
  } else {
    prefs->DisableAutoOpenByUserBasedOnExtension(path);
  }
  return YES;
}

@end
