// Copyright 2026 ArcMod. Import-source discovery and boundary value objects.
#include "arc/api/arc_internal.h"
#include "arc/browser/extensions_values.h"

#include <array>
#include <memory>
#include <optional>
#include <vector>

#include "base/command_line.h"
#include "base/check.h"
#include "base/functional/bind.h"
#include "base/no_destructor.h"
#include "base/task/thread_pool.h"
#include "third_party/sqlite/sqlite3.h"
#include "base/task/sequenced_task_runner.h"

// This record's two native int32 fields are part of the public pointer encoding.
struct ImportSource {
  int source_id;
  int type;
};

namespace {
struct ImportProfileSnapshot {
  int source_id = -1;
  int native_type = -1;
  std::string name;
  std::string profile_name;
  std::string source_path;
  std::string app_path;
  int categories = 0;
};

struct SourceDescriptor {
  int source_id;
  int native_type;
  const char* bundle;
  const char* relative_path;
  const char* name;
};

// The IDs, native types, and bundle IDs are the ten ordered records at native
// 0xdf6aae8; profile detector paths are from its macOS discovery implementation.
constexpr std::array<SourceDescriptor, 10> kSources = {{
    {0, 1, "com.google.Chrome", "Google/Chrome", "Chrome"},
    {1, 1, "com.google.Chrome.canary", "Google/Chrome Canary", "Chrome Canary"},
    {2, 8, "com.brave.Browser", "BraveSoftware/Brave-Browser", "Brave"},
    {3, 7, "com.microsoft.edgemac", "Microsoft Edge", "Microsoft Edge"},
    {4, 9, "com.vivaldi.Vivaldi", "Vivaldi", "Vivaldi"},
    {5, 10, "com.operasoftware.Opera", "com.operasoftware.Opera", "Opera"},
    {6, 11, "com.operasoftware.OperaGX", "com.operasoftware.OperaGX", "Opera GX"},
    {7, 12, "company.thebrowser.Browser", "Arc/User Data", "Arc"},
    {8, 3, "com.apple.Safari", "", "Safari"},
    {9, 2, "org.mozilla.firefox", "Firefox", "Firefox"},
}};

long long PublicImportType(int native_type) {
  // Native 0x4403e9c uses this table indexed by native_type + 1 and the
  // validity bitmask 0x7f1d. Profile type maps the same values.
  constexpr std::array<unsigned char, 15> kTypes = {
      0, 0, 2, 3, 4, 0, 0, 0, 6, 7, 8, 9, 10, 11, 12};
  int index = native_type + 1;
  CHECK(index >= 0 && index < static_cast<int>(kTypes.size()) &&
        (0x7f1d & (1 << index)));
  return kTypes[index];
}

NSString* CanonicalPath(NSString* path) {
  return [[path stringByStandardizingPath] stringByResolvingSymlinksInPath];
}

bool PathIsWithin(NSString* path, NSString* root) {
  return [path isEqualToString:root] ||
         [path hasPrefix:[root stringByAppendingString:@"/"]];
}

bool SafeImportPath(NSString* path) {
  if (!path.length) return false;
  NSString* canonical = CanonicalPath(path);
  // Lead exception #133436 allows source existence/metadata reads, including
  // Arc's metadata. No importer or source writes are performed by this scanner.
  return !PathIsWithin(canonical, @"/Applications/Arc.app");
}

NSString* ImportLibraryPath() {
  std::string override_path = base::CommandLine::ForCurrentProcess()
                                  ->GetSwitchValueASCII("arcmod-import-library-path");
  return override_path.empty()
             ? [NSHomeDirectory() stringByAppendingPathComponent:@"Library"]
             : CanonicalPath(ArcNSString(override_path));
}

bool ImportPathExists(NSString* path) {
  return SafeImportPath(path) &&
         [[NSFileManager defaultManager] fileExistsAtPath:path];
}

NSDictionary* ReadImportDictionary(NSString* path) {
  if (!SafeImportPath(path)) return nil;
  NSData* data = [NSData dataWithContentsOfFile:path];
  if (!data) return nil;
  id value = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
  return [value isKindOfClass:[NSDictionary class]] ? value : nil;
}

NSDictionary* ImportDictionary(id value) {
  return [value isKindOfClass:[NSDictionary class]] ? value : nil;
}

NSString* ImportString(id value) {
  return [value isKindOfClass:[NSString class]] ? value : @"";
}

NSString* ImportAppPath(const SourceDescriptor& descriptor) {
  // NSWorkspace supplies installed-app path metadata; never open the app bundle.
  NSURL* url = [[NSWorkspace sharedWorkspace]
      URLForApplicationWithBundleIdentifier:ArcNSString(descriptor.bundle)];
  return url.path ?: @"";
}

std::vector<arc::ExtensionInfo> DiscoverImportExtensions(ImportProfileSnapshot profile);

bool HasImportableCreditCards(NSString* path) {
  if (!ImportPathExists(path)) return false;
  // READONLY alone can still create/update a live database's WAL shared-memory
  // file. SQLite's readonly_shm URI option makes discovery metadata-only there
  // too; a source without a usable read-only WAL index is left unsupported.
  NSString* uri = [[[NSURL fileURLWithPath:path] absoluteString]
      stringByAppendingString:@"?mode=ro&readonly_shm=1"];
  sqlite3* opened = nullptr;
  const int status = sqlite3_open_v2(ArcUTF8(uri).c_str(), &opened,
      SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_URI, nullptr);
  std::unique_ptr<sqlite3, decltype(&sqlite3_close)> database(opened, &sqlite3_close);
  if (status != SQLITE_OK) return false;
  sqlite3_stmt* prepared = nullptr;
  const int result = sqlite3_prepare_v2(database.get(),
      "SELECT 1 FROM credit_cards LIMIT 1", -1, &prepared, nullptr);
  std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> query(prepared, &sqlite3_finalize);
  return result == SQLITE_OK && sqlite3_step(query.get()) == SQLITE_ROW;
}

int ImportCategories(NSString* profile_path, int native_type) {
  int result = 0;
  if (ImportPathExists([profile_path stringByAppendingPathComponent:@"History"]))
    result |= 1537;
  if (ImportPathExists([profile_path stringByAppendingPathComponent:@"Bookmarks"]))
    result |= 2;
  if (ImportPathExists([profile_path stringByAppendingPathComponent:@"Cookies"]) ||
      ImportPathExists([profile_path stringByAppendingPathComponent:@"Network/Cookies"]))
    result |= 4;
  if (ImportPathExists([profile_path stringByAppendingPathComponent:@"Login Data"]))
    result |= 8;
  if (HasImportableCreditCards([profile_path stringByAppendingPathComponent:@"Web Data"]))
    result |= 256;
  if (ImportPathExists([profile_path stringByAppendingPathComponent:@"Current Session"]) ||
      ImportPathExists([profile_path stringByAppendingPathComponent:@"Last Session"]) ||
      ImportPathExists([profile_path stringByAppendingPathComponent:@"Sessions"]))
    result |= 2048;
  if (ReadImportDictionary([profile_path stringByAppendingPathComponent:@"Secure Preferences"])) {
    ImportProfileSnapshot profile;
    profile.source_path = ArcUTF8(profile_path);
    profile.native_type = native_type;
    if (!DiscoverImportExtensions(std::move(profile)).empty()) result |= 128;
  }
  return result;
}

std::vector<ImportProfileSnapshot> DiscoverImportProfiles(int requested_id) {
  @autoreleasepool {
    std::vector<ImportProfileSnapshot> profiles;
    NSString* library = ImportLibraryPath();
    NSString* application_support =
        [library stringByAppendingPathComponent:@"Application Support"];
    for (const auto& descriptor : kSources) {
      if (requested_id >= 0 && requested_id != descriptor.source_id) continue;
      if (descriptor.native_type == 2) continue;  // Separate Firefox backend.
      if (descriptor.native_type == 3) {
        NSString* path = [library stringByAppendingPathComponent:@"Safari"];
        if (ImportPathExists(path)) {
          ImportProfileSnapshot profile;
          profile.source_id = descriptor.source_id;
          profile.native_type = descriptor.native_type;
          profile.name = descriptor.name;
          profile.source_path = ArcUTF8(path);
          profile.app_path = ArcUTF8(ImportAppPath(descriptor));
          profiles.push_back(std::move(profile));
        }
        continue;
      }
      NSString* root = [application_support
          stringByAppendingPathComponent:ArcNSString(descriptor.relative_path)];
      if (!ImportPathExists(root)) continue;
      NSDictionary* local_state =
          ReadImportDictionary([root stringByAppendingPathComponent:@"Local State"]);
      NSDictionary* cache = ImportDictionary(ImportDictionary(local_state[@"profile"])[@"info_cache"]);
      NSArray* keys = cache ? [[cache allKeys] sortedArrayUsingSelector:@selector(compare:)]
                           : @[ @"Default" ];
      for (id key in keys) {
        NSString* directory = ImportString(key);
        if (!directory.length || ![directory isEqualToString:directory.lastPathComponent] ||
            [directory isEqualToString:@"."] || [directory isEqualToString:@".."]) continue;
        NSString* path = [root stringByAppendingPathComponent:directory];
        if (!PathIsWithin(CanonicalPath(path), CanonicalPath(root)) ||
            !ImportPathExists(path)) continue;
        NSDictionary* metadata = ImportDictionary(cache[key]);
        NSString* profile_name = ImportString(metadata[@"name"]);
        if (!profile_name.length) profile_name = ImportString(metadata[@"shortcut_name"]);
        if (!profile_name.length) profile_name = ImportString(metadata[@"user_name"]);
        ImportProfileSnapshot profile;
        profile.source_id = descriptor.source_id;
        profile.native_type = descriptor.native_type;
        profile.name = descriptor.name;
        profile.profile_name = ArcUTF8(profile_name);
        profile.source_path = ArcUTF8(path);
        profile.app_path = ArcUTF8(ImportAppPath(descriptor));
        profile.categories = ImportCategories(path, descriptor.native_type);
        profiles.push_back(std::move(profile));
      }
    }
    return profiles;
  }
}

std::vector<ImportSource> DiscoverImportSources() {
  @autoreleasepool {
    std::vector<ImportSource> sources;
    for (const auto& descriptor : kSources) {
      if (ImportAppPath(descriptor).length)
        sources.push_back({descriptor.source_id, descriptor.native_type});
    }
    return sources;
  }
}

std::vector<arc::ExtensionInfo> DiscoverImportExtensions(ImportProfileSnapshot profile) {
  @autoreleasepool {
    std::vector<arc::ExtensionInfo> extensions;
    if (profile.native_type == 3 || profile.native_type == 13) return extensions;
    NSString* root = ArcNSString(profile.source_path);
    NSDictionary* preferences =
        ReadImportDictionary([root stringByAppendingPathComponent:@"Preferences"]);
    NSDictionary* secure =
        ReadImportDictionary([root stringByAppendingPathComponent:@"Secure Preferences"]);
    NSString* settings_key = (profile.native_type == 10 || profile.native_type == 11)
                                 ? @"opsettings" : @"settings";
    NSDictionary* settings = ImportDictionary(ImportDictionary(secure[@"extensions"])[settings_key]);
    if (!settings) settings = ImportDictionary(ImportDictionary(preferences[@"extensions"])[settings_key]);
    for (NSString* extension_id in [[settings allKeys] sortedArrayUsingSelector:@selector(compare:)]) {
      NSDictionary* entry = ImportDictionary(settings[extension_id]);
      NSDictionary* manifest = ImportDictionary(entry[@"manifest"]);
      id default_install = entry[@"was_installed_by_default"];
      id from_webstore = entry[@"from_webstore"];
      id disable_reasons = entry[@"disable_reasons"];
      // Native 0x43208d0 selects user-installed Chrome Web Store extensions,
      // with no disable reasons, and a normal extension manifest (type 1).
      if (!manifest || ![default_install isKindOfClass:[NSNumber class]] ||
          CFGetTypeID((__bridge CFTypeRef)default_install) != CFBooleanGetTypeID() ||
          [default_install boolValue] ||
          ![from_webstore isKindOfClass:[NSNumber class]] ||
          CFGetTypeID((__bridge CFTypeRef)from_webstore) != CFBooleanGetTypeID() ||
          ![from_webstore boolValue] ||
          (disable_reasons && (![disable_reasons isKindOfClass:[NSNumber class]] ||
              CFGetTypeID((__bridge CFTypeRef)disable_reasons) != CFNumberGetTypeID() ||
              CFNumberIsFloatType((__bridge CFNumberRef)disable_reasons) ||
              [disable_reasons longLongValue] != 0)) ||
          manifest[@"theme"] || manifest[@"app"] ||
          !ImportString(manifest[@"version"]).length) continue;
      arc::ExtensionInfo info;
      info.id = ArcUTF8(extension_id);
      info.name = ArcUTF8(ImportString(manifest[@"name"]));
      info.description = ArcUTF8(ImportString(manifest[@"description"]));
      info.version = ArcUTF8(ImportString(manifest[@"version"]));
      info.url = "https://chrome.google.com/webstore/detail/" + info.id;
      info.enabled = true;
      info.from_webstore = true;
      info.has_action = manifest[@"action"] || manifest[@"browser_action"] || manifest[@"page_action"];
      info.omnibox_keyword = ArcUTF8(ImportString(ImportDictionary(manifest[@"omnibox"])[@"keyword"]));
      id permissions = manifest[@"permissions"];
      if ([permissions isKindOfClass:[NSArray class]]) {
        for (id permission in permissions) {
          if ([permission isKindOfClass:[NSString class]])
            info.permissions.push_back(ArcUTF16(permission));
        }
      }
      extensions.push_back(std::move(info));
    }
    return extensions;
  }
}

struct ImportSequence {
  std::vector<ImportProfileSnapshot> profiles;
};
std::optional<ImportSequence>& CurrentImportSequence() {
  static base::NoDestructor<std::optional<ImportSequence>> sequence;
  return *sequence;
}
}  // namespace

ARC_EXPORT @interface ArcImportSource : NSObject
- (instancetype)initWithImportSource:(const ImportSource*)source;
@property(nonatomic, readonly) long long sourceID;
@property(nonatomic, readonly) long long type;
@end
@implementation ArcImportSource
@synthesize sourceID = _sourceID, type = _type;
- (instancetype)initWithImportSource:(const ImportSource*)source {
  if ((self = [super init])) {
    _sourceID = source->source_id;
    _type = PublicImportType(source->type);
  }
  return self;
}
@end

ARC_EXPORT @interface ArcImportSourceProfile : NSObject {
  ImportProfileSnapshot _snapshot;
}
- (instancetype)initWithImportSourceProfile:(const void*)profile;
- (instancetype)initWithSafariExportedData:(NSString*)path profileName:(NSString*)profileName
                                    name:(NSString*)name categories:(long long)categories;
- (void)availableExtensions:(void (^)(NSArray<ArcExtensionInfo*>*))completion;
@property(nonatomic, readonly, copy) NSString* name;
@property(nonatomic, readonly, copy) NSString* profileName;
@property(nonatomic, readonly, copy) NSString* sourcePath;
@property(nonatomic, readonly, copy) NSString* appPath;
@property(nonatomic, readonly) long long type;
@property(nonatomic, readonly) long long supportedCategories;
@property(nonatomic, readonly) long long constraints;
@end
@implementation ArcImportSourceProfile
- (instancetype)initWithImportSourceProfile:(const void*)profile {
  if ((self = [super init])) {
    if (profile) _snapshot = *static_cast<const ImportProfileSnapshot*>(profile);
  }
  return self;
}
- (instancetype)initWithSafariExportedData:(NSString*)path profileName:(NSString*)profileName
                                    name:(NSString*)name categories:(long long)categories {
  if ((self = [super init])) {
    _snapshot.native_type = 13;
    _snapshot.source_path = ArcUTF8(path);
    _snapshot.profile_name = ArcUTF8(profileName);
    _snapshot.name = ArcUTF8(name);
    _snapshot.categories = static_cast<unsigned short>(categories);
  }
  return self;
}
- (NSString*)name { return ArcNSString(_snapshot.name); }
- (NSString*)profileName { return ArcNSString(_snapshot.profile_name); }
- (NSString*)sourcePath { return ArcNSString(_snapshot.source_path); }
- (NSString*)appPath { return ArcNSString(_snapshot.app_path); }
- (long long)type { return PublicImportType(_snapshot.native_type); }
- (long long)supportedCategories { return _snapshot.categories & 0xfff; }
- (long long)constraints { return _snapshot.native_type == 3; }
- (void)availableExtensions:(void (^)(NSArray<ArcExtensionInfo*>*))completion {
  if (!completion) return;
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock()},
      base::BindOnce(&DiscoverImportExtensions, _snapshot),
      base::BindOnce(^(std::vector<arc::ExtensionInfo> extensions) {
        NSMutableArray<ArcExtensionInfo*>* result = [NSMutableArray array];
        for (const auto& extension : extensions)
          [result addObject:[[ArcExtensionInfo alloc] initWithExtensionInfo:&extension]];
        completion(result);
      }));
}
@end

@class ArcBookmarkTreeNode;
ARC_EXPORT @interface ArcBookmarkImportTargets : NSObject
- (instancetype)initWithBookmarkBarBookmarksTargetNode:(ArcBookmarkTreeNode*)bar
                         withOtherBookmarksTargetNode:(ArcBookmarkTreeNode*)other;
@property(nonatomic, strong) ArcBookmarkTreeNode* targetNodeForBookmarkBarBookmarks;
@property(nonatomic, strong) ArcBookmarkTreeNode* targetNodeForOtherBookmarks;
@end
@implementation ArcBookmarkImportTargets
@synthesize targetNodeForBookmarkBarBookmarks = _targetNodeForBookmarkBarBookmarks;
@synthesize targetNodeForOtherBookmarks = _targetNodeForOtherBookmarks;
- (instancetype)initWithBookmarkBarBookmarksTargetNode:(ArcBookmarkTreeNode*)bar
                         withOtherBookmarksTargetNode:(ArcBookmarkTreeNode*)other {
  if ((self = [super init])) {
    _targetNodeForBookmarkBarBookmarks = bar;
    _targetNodeForOtherBookmarks = other;
  }
  return self;
}
@end

namespace {
void CompleteImportProfiles(void (^completion)(NSArray*),
                            std::vector<ImportProfileSnapshot> profiles) {
  if (CurrentImportSequence()) CurrentImportSequence()->profiles = profiles;
  NSMutableArray* result = [NSMutableArray array];
  for (const auto& profile : profiles)
    [result addObject:[[ArcImportSourceProfile alloc] initWithImportSourceProfile:&profile]];
  completion(result);
}
}

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end
@implementation ArcBrowserApplication (AppImports)
- (void)availableImportSourceProfiles:(void (^)(NSArray*))completion {
  if (!completion) return;
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock()}, base::BindOnce(&DiscoverImportProfiles, -1),
      base::BindOnce(&CompleteImportProfiles, completion));
}
- (void)availableImportSources:(void (^)(NSArray*))completion {
  if (!completion) return;
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock()}, base::BindOnce(&DiscoverImportSources),
      base::BindOnce(^(std::vector<ImportSource> sources) {
        NSMutableArray* result = [NSMutableArray array];
        for (const auto& source : sources)
          [result addObject:[[ArcImportSource alloc] initWithImportSource:&source]];
        completion(result);
      }));
}
- (void)loadImportSourceProfiles:(long long)sourceID completion:(void (^)(NSArray*))completion {
  if (!completion) return;
  if (sourceID < 0 || sourceID >= static_cast<long long>(kSources.size())) {
    // Keep completion asynchronous for an unknown source, just like discovery.
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE, base::BindOnce(^{ completion(@[]); }));
    return;
  }
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock()},
      base::BindOnce(&DiscoverImportProfiles, static_cast<int>(sourceID)),
      base::BindOnce(&CompleteImportProfiles, completion));
}
- (void)beginImportSequence {
  if (!CurrentImportSequence()) CurrentImportSequence().emplace();
}
- (void)endImportSequence { CurrentImportSequence().reset(); }
@end
