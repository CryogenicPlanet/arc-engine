// Copyright 2026 ArcMod. Application-level APIs and diagnostics classes.
#include "arc/api/arc_internal.h"

#import <objc/runtime.h>

#include <array>
#include <atomic>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "base/base64.h"
#include "components/os_crypt/common/os_crypt_switches.h"
#include "crypto/apple/fake_keychain_v2.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/functional/bind.h"
#include "base/no_destructor.h"
#include "base/path_service.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/task/single_thread_task_runner.h"
#include "base/time/time.h"
#include "base/trace_event/memory_allocator_dump.h"
#include "base/trace_event/memory_dump_manager.h"
#include "base/trace_event/memory_dump_provider.h"
#include "base/trace_event/process_memory_dump.h"
#include "base/values.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/enterprise/browser_management/browser_management_service.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "chrome/browser/performance_manager/public/user_tuning/battery_saver_mode_manager.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_commands_mac.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/dialogs/browser_dialogs.h"
#include "chrome/common/channel_info.h"
#include "chrome/common/chrome_paths.h"
#include "components/enterprise/browser/controller/browser_dm_token_storage.h"
#include "components/net_log/net_export_file_writer.h"
#include "components/os_crypt/common/keychain_password_mac.h"
#include "components/performance_manager/public/user_tuning/prefs.h"
#include "components/prefs/pref_service.h"
#include "components/version_info/version_info.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/storage_partition.h"
#include "crypto/apple/keychain_v2.h"
#include "net/log/net_log_capture_mode.h"
#include "third_party/boringssl/src/include/openssl/curve25519.h"
#include "third_party/boringssl/src/include/openssl/evp.h"
#include "ui/display/display.h"
#include "ui/display/screen.h"

namespace {

NSString* const kArcAppErrorDomain = @"company.thebrowser.Arc";
NSString* const kArcSpake2ErrorDomain = @"company.thebrowser.Arc.Spake2";

NSError* MakeError(NSString* domain, NSInteger code, NSString* description) {
  return [NSError errorWithDomain:domain
                             code:code
                         userInfo:description ? @{NSLocalizedDescriptionKey : description} : nil];
}

// nil for an empty string, like the engine's optional<string> conversions.
NSString* NonEmptyNSString(const std::string& value) {
  return value.empty() ? nil : ArcNSString(value);
}

using BatteryManager = performance_manager::user_tuning::BatterySaverModeManager;

// Free memory target on critical memory pressure, in bytes. Arc's engine keeps it in a global that its
// patched memory-pressure policy reads; nothing in stock Chromium consumes it.
std::atomic<uint64_t>& FreeMemoryTargetBytes() {
  static std::atomic<uint64_t> target{0};
  return target;
}

std::vector<std::string>& ArtifactAllowedScriptSources() {
  static base::NoDestructor<std::vector<std::string>> sources;
  return *sources;
}

// State of -[ArcPerformanceController simulateLowBatteryStateForDuration:].
struct LowBatterySimulation {
  uint64_t generation = 0;
  std::optional<int> previous_state;
};

LowBatterySimulation& Simulation() {
  static LowBatterySimulation simulation;
  return simulation;
}

}  // namespace

// ---- ArcBrowserApplication ---------------------------------------------------------------------

@interface ArcNetworkLoggingConfiguration : NSObject
@property(nonatomic, copy) NSString* filePath;
@property(nonatomic) double loggingDurationInSeconds;
@end

ARC_EXPORT @interface ArcNetworkActivityMonitor : NSObject
@property(nonatomic, readonly) BOOL isLogging;
- (BOOL)startLogging:(ArcNetworkLoggingConfiguration*)configuration
                with:(id)context
               error:(NSError**)error;
@end

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end

// The engine's debug property record: two std::string, passed by value (indirect).
namespace app_extras {
struct DebugProperty {
  std::string name;
  std::string value;
};
}  // namespace app_extras

@interface ArcBrowserApplicationDebugProperty : NSObject
- (instancetype)initWithDebugProperty:(app_extras::DebugProperty)property;
@end

static const void* const kNetworkActivityMonitorKey = &kNetworkActivityMonitorKey;

@implementation ArcBrowserApplication (AppExtras)

- (void)showTaskManager {
  chrome::ShowTaskManager(nullptr);
}

- (NSArray*)debugProperties {
  std::vector<app_extras::DebugProperty> properties;
  properties.push_back({"Chromium version", std::string(version_info::GetVersionNumber())});
  properties.push_back(
      {"Channel", chrome::GetChannelName(chrome::WithExtendedStable(false))});
  base::FilePath user_data_dir;
  if (base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir)) {
    properties.push_back({"User data directory", user_data_dir.AsUTF8Unsafe()});
  }
  properties.push_back(
      {"Command line", base::CommandLine::ForCurrentProcess()->GetCommandLineString()});
  NSMutableArray* result = [NSMutableArray arrayWithCapacity:properties.size()];
  for (app_extras::DebugProperty& property : properties) {
    [result addObject:[[ArcBrowserApplicationDebugProperty alloc]
                          initWithDebugProperty:std::move(property)]];
  }
  return [result copy];
}

// The OSCrypt key: the Keychain "Safe Storage" password run through PBKDF2-HMAC-SHA1 exactly like
// os_crypt's keychain key provider (salt "saltysalt", 1003 iterations, 16 bytes), base64 encoded.
// Under --use-mock-keychain (ArcMod) it uses the same fake keychain as os_crypt, never the real one.
- (NSString*)getBase64EncodedEncryptionKey {
  std::unique_ptr<crypto::apple::FakeKeychainV2> fake_keychain;
  crypto::apple::KeychainV2* keychain = &crypto::apple::KeychainV2::GetInstance();
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(os_crypt::switches::kUseMockKeychain)) {
    fake_keychain = std::make_unique<crypto::apple::FakeKeychainV2>("test-access-group");
    keychain = fake_keychain.get();
  }
  KeychainPassword keychain_password(*keychain);
  const std::string password = keychain_password.GetPassword();
  if (password.empty()) {
    return nil;
  }
  static constexpr char kSalt[] = "saltysalt";
  std::array<uint8_t, 16> key;
  if (!PKCS5_PBKDF2_HMAC_SHA1(password.data(), password.size(),
                              reinterpret_cast<const uint8_t*>(kSalt), sizeof(kSalt) - 1, 1003,
                              key.size(), key.data())) {
    return nil;
  }
  return NonEmptyNSString(base::Base64Encode(key));
}

- (NSString*)getEnterpriseDeviceID {
  policy::BrowserDMTokenStorage* storage = policy::BrowserDMTokenStorage::Get();
  return storage ? NonEmptyNSString(storage->RetrieveClientId()) : nil;
}

// chrome://inspect's browser target: the active-port file Chromium's DevTools HTTP handler writes into the
// user data directory while remote debugging is on holds the port and the /devtools/browser/<id> path.
- (NSString*)devToolsWebSocketDebugURL {
  base::FilePath user_data_dir;
  if (!base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir)) {
    return nil;
  }
  std::string contents;
  if (!base::ReadFileToString(user_data_dir.AppendASCII("DevToolsActivePort"), &contents)) {
    return nil;
  }
  const std::vector<std::string> lines = base::SplitString(
      contents, "\n", base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);
  int port = 0;
  if (lines.size() < 2 || !base::StringToInt(lines[0], &port) || port <= 0) {
    return nil;
  }
  return ArcNSString("ws://127.0.0.1:" + base::NumberToString(port) + lines[1]);
}

- (void)setArtifactAllowedScriptSources:(NSArray*)sources {
  std::vector<std::string> converted;
  converted.reserve(sources.count);
  for (id source in sources) {
    if ([source isKindOfClass:[NSString class]]) {
      converted.push_back(ArcUTF8(source));
    }
  }
  ArtifactAllowedScriptSources() = std::move(converted);
}

- (NSValue*)frameForDisplayId:(int64_t)displayId error:(NSError**)error {
  display::Screen* screen = display::Screen::Get();
  if (screen) {
    for (const display::Display& display : screen->GetAllDisplays()) {
      if (display.id() == displayId) {
        const gfx::Rect bounds = display.bounds();
        return [NSValue valueWithRect:NSMakeRect(bounds.x(), bounds.y(), bounds.width(),
                                                 bounds.height())];
      }
    }
  }
  if (error) {
    *error = [NSError errorWithDomain:kArcAppErrorDomain code:3 userInfo:nil];
  }
  return nil;
}

- (ArcNetworkActivityMonitor*)networkActivityMonitor {
  ArcNetworkActivityMonitor* monitor = objc_getAssociatedObject(self, kNetworkActivityMonitorKey);
  if (!monitor) {
    monitor = [[ArcNetworkActivityMonitor alloc] init];
    objc_setAssociatedObject(self, kNetworkActivityMonitorKey, monitor,
                             OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  return monitor;
}

- (void)setFreeMemoryTargetOnCriticalMemoryPressure:(NSUInteger)megabytes {
  FreeMemoryTargetBytes().store(static_cast<uint64_t>(megabytes) << 20);
}

@end

// ---- ArcBrowser --------------------------------------------------------------------------------

@implementation ArcBrowser (AppExtras)

- (void)toggleJavaScriptFromAppleEventsAllowed {
  if (BrowserWindowInterface* browser = [self browser]) {
    chrome::ToggleJavaScriptFromAppleEventsAllowed(browser);
  }
}

@end

// ---- ArcPerformanceController ------------------------------------------------------------------

ARC_EXPORT @interface ArcPerformanceController : NSObject
@end

@implementation ArcPerformanceController (AppExtras)

// Chromium has no low-battery injection point, so the simulation forces battery saver on for |duration|
// seconds through its pref, then restores the previous mode.
+ (void)simulateLowBatteryStateForDuration:(unsigned int)duration {
  PrefService* local_state = g_browser_process ? g_browser_process->local_state() : nullptr;
  if (!local_state) {
    return;
  }
  namespace prefs = performance_manager::user_tuning::prefs;
  LowBatterySimulation& simulation = Simulation();
  if (!simulation.previous_state) {
    simulation.previous_state = local_state->GetInteger(prefs::kBatterySaverModeState);
  }
  const uint64_t generation = ++simulation.generation;
  local_state->SetInteger(prefs::kBatterySaverModeState,
                          static_cast<int>(prefs::BatterySaverModeState::kEnabled));
  base::SingleThreadTaskRunner::GetCurrentDefault()->PostDelayedTask(
      FROM_HERE, base::BindOnce(
                     [](uint64_t expected_generation) {
                       LowBatterySimulation& simulation = Simulation();
                       PrefService* local_state =
                           g_browser_process ? g_browser_process->local_state() : nullptr;
                       // A newer simulation owns the restore.
                       if (simulation.generation != expected_generation) {
                         return;
                       }
                       if (local_state && simulation.previous_state) {
                         local_state->SetInteger(prefs::kBatterySaverModeState,
                                                 *simulation.previous_state);
                       }
                       simulation.previous_state.reset();
                     },
                     generation),
      base::Seconds(duration));
}

@end

// ---- ArcBreadcrumb -----------------------------------------------------------------------------

namespace app_extras {
struct Breadcrumb {
  std::string category;
  std::string message;
  std::unordered_map<std::string, std::string> data;
  std::optional<std::string> file_id;
  std::optional<int> line;
};
}  // namespace app_extras

ARC_EXPORT @interface ArcBreadcrumb : NSObject {
  app_extras::Breadcrumb _breadcrumb;
}
// A direct method has no runtime selector entry for the boundary recorder.
- (instancetype)initWithRecord:(app_extras::Breadcrumb)record __attribute__((objc_direct));
@property(nonatomic, readonly, copy) NSString* category;
@property(nonatomic, readonly, copy) NSString* message;
@property(nonatomic, readonly, copy) NSDictionary* data;
@property(nonatomic, readonly, copy) NSString* fileId;
@property(nonatomic, readonly) NSNumber* line;
@end

@implementation ArcBreadcrumb
- (instancetype)initWithRecord:(app_extras::Breadcrumb)record {
  if ((self = [super init])) {
    _breadcrumb = std::move(record);
  }
  return self;
}
- (NSString*)category { return ArcNSString(_breadcrumb.category); }
- (NSString*)message { return ArcNSString(_breadcrumb.message); }
- (NSDictionary*)data {
  NSMutableDictionary* data = [NSMutableDictionary dictionaryWithCapacity:_breadcrumb.data.size()];
  for (const auto& [key, value] : _breadcrumb.data) {
    data[ArcNSString(key)] = ArcNSString(value);
  }
  return data;
}
- (NSString*)fileId {
  return _breadcrumb.file_id ? ArcNSString(*_breadcrumb.file_id) : nil;
}
- (NSNumber*)line { return _breadcrumb.line ? @(*_breadcrumb.line) : nil; }
@end

// Creates the ArcBreadcrumb handed to the callback of -[ArcBrowserApplication setBreadcrumbCallback:].
id ArcCreateBreadcrumb(const std::string& category,
                       const std::string& message,
                       const std::unordered_map<std::string, std::string>& data,
                       const std::optional<std::string>& file_id,
                       std::optional<int> line);
id ArcCreateBreadcrumb(const std::string& category,
                       const std::string& message,
                       const std::unordered_map<std::string, std::string>& data,
                       const std::optional<std::string>& file_id,
                       std::optional<int> line) {
  return [[ArcBreadcrumb alloc] initWithRecord:{category, message, data, file_id, line}];
}

// ---- ArcMemoryDumpProvider ---------------------------------------------------------------------

@interface ArcMemoryDumpMetric : NSObject
@property(nonatomic, readonly) NSString* name;
@property(nonatomic, readonly) NSString* units;
@property(nonatomic, readonly) uint64_t value;
@end

namespace {

class BlockMemoryDumpProvider final : public base::trace_event::MemoryDumpProvider {
 public:
  using DataProvider = void (^)(void (^completion)(NSArray<ArcMemoryDumpMetric*>*));

  BlockMemoryDumpProvider(std::string name, DataProvider data_provider)
      : name_(std::move(name)), data_provider_(data_provider) {
    // Provider names are checked at compile time against the MemoryDumpProviderName histogram variants,
    // so the Arc-chosen name only labels the allocator dump; the registration uses a variant reserved
    // for non-production providers.
    base::trace_event::MemoryDumpManager::GetInstance()->RegisterDumpProvider(
        this, "TestDumpProvider", base::SingleThreadTaskRunner::GetCurrentDefault());
  }

  ~BlockMemoryDumpProvider() override {
    base::trace_event::MemoryDumpManager::GetInstance()->UnregisterDumpProvider(this);
  }

  bool OnMemoryDump(const base::trace_event::MemoryDumpArgs& args,
                    base::trace_event::ProcessMemoryDump* pmd) override {
    // Chromium reads the dump synchronously; the data provider has to report back before returning.
    __block NSArray<ArcMemoryDumpMetric*>* metrics = nil;
    data_provider_(^(NSArray<ArcMemoryDumpMetric*>* reported) {
      metrics = reported;
    });
    if (!metrics) {
      return false;
    }
    base::trace_event::MemoryAllocatorDump* dump = pmd->CreateAllocatorDump(name_);
    for (ArcMemoryDumpMetric* metric in metrics) {
      const std::string metric_name = ArcUTF8(metric.name);
      const std::string metric_units = ArcUTF8(metric.units);
      dump->AddScalar(metric_name.c_str(), metric_units.c_str(), metric.value);
    }
    return true;
  }

 private:
  const std::string name_;
  const DataProvider data_provider_;
};

}  // namespace

ARC_EXPORT @interface ArcMemoryDumpProvider : NSObject {
  std::unique_ptr<BlockMemoryDumpProvider> _cppProvider;
}
- (instancetype)initWithName:(NSString*)name
                dataProvider:(void (^)(void (^)(NSArray<ArcMemoryDumpMetric*>*)))dataProvider;
@end

@implementation ArcMemoryDumpProvider
- (instancetype)initWithName:(NSString*)name
                dataProvider:(void (^)(void (^)(NSArray<ArcMemoryDumpMetric*>*)))dataProvider {
  if ((self = [super init])) {
    _cppProvider = std::make_unique<BlockMemoryDumpProvider>("bcny-" + ArcUTF8(name), dataProvider);
  }
  return self;
}
- (void)dealloc {
  // Unregisters before the block and the name go away.
  _cppProvider.reset();
}
@end

// ---- ArcNetworkActivityMonitor -----------------------------------------------------------------

namespace {

net_log::NetExportFileWriter* NetExportWriter() {
  if (!g_browser_process || !g_browser_process->system_network_context_manager()) {
    return nullptr;
  }
  return g_browser_process->system_network_context_manager()->GetNetExportFileWriter();
}

}  // namespace

@implementation ArcNetworkActivityMonitor

// The application has one monitor over the process-wide NetLog writer, so every wrapper is equal.
- (BOOL)isEqual:(id)other {
  return self == other || [other isKindOfClass:[ArcNetworkActivityMonitor class]];
}

- (NSUInteger)hash {
  return reinterpret_cast<NSUInteger>(&kNetworkActivityMonitorKey);
}

// Arc counts initialization, not the final stop, as logging.
- (BOOL)isLogging {
  net_log::NetExportFileWriter* writer = NetExportWriter();
  if (!writer) {
    return NO;
  }
  const base::DictValue state_dict = writer->GetState();
  const std::string* state = state_dict.FindString("state");
  return state && (*state == "INITIALIZING" || *state == "STARTING_LOG" || *state == "LOGGING");
}

- (BOOL)startLogging:(ArcNetworkLoggingConfiguration*)configuration
                with:(id)context
               error:(NSError**)error {
  auto fail = [&](NSInteger code, NSString* description) {
    if (error) {
      *error = MakeError(kArcAppErrorDomain, code, description);
    }
    return NO;
  };
  Profile* profile = ArcProfileFrom(context);
  net_log::NetExportFileWriter* writer = NetExportWriter();
  if (!profile || !writer) {
    return fail(1, @"Network logging is unavailable");
  }
  if ([self isLogging]) {
    return fail(2, @"Network logging is already active");
  }
  writer->Initialize();
  writer->StartNetLog(
      base::FilePath(ArcUTF8(configuration.filePath)), net::NetLogCaptureMode::kDefault,
      net::NetLogFileFormat::kJson, net_log::NetExportFileWriter::kNoLimit,
      base::CommandLine::ForCurrentProcess()->GetCommandLineString(),
      chrome::GetChannelName(chrome::WithExtendedStable(false)),
      profile->GetDefaultStoragePartition()->GetNetworkContext());
  const double duration = configuration.loggingDurationInSeconds;
  if (duration > 0) {
    base::SingleThreadTaskRunner::GetCurrentDefault()->PostDelayedTask(
        FROM_HERE,
        base::BindOnce([] {
          if (net_log::NetExportFileWriter* writer = NetExportWriter()) {
            writer->StopNetLog();
          }
        }),
        base::Seconds(duration));
  }
  return YES;
}

@end

// ---- ArcSpake2Context --------------------------------------------------------------------------

namespace {

struct Spake2Deleter {
  void operator()(SPAKE2_CTX* ctx) const { SPAKE2_CTX_free(ctx); }
};

}  // namespace

ARC_EXPORT @interface ArcSpake2Context : NSObject {
  std::unique_ptr<SPAKE2_CTX, Spake2Deleter> _ctx;
}
- (instancetype)initWithRole:(NSInteger)role myName:(NSData*)myName theirName:(NSData*)theirName;
- (NSData*)generateMessageWithPassword:(NSData*)password error:(NSError**)error;
- (NSData*)processMessage:(NSData*)message error:(NSError**)error;
@end

@implementation ArcSpake2Context

- (instancetype)initWithRole:(NSInteger)role myName:(NSData*)myName theirName:(NSData*)theirName {
  if ((self = [super init])) {
    _ctx.reset(SPAKE2_CTX_new(role != 0 ? spake2_role_bob : spake2_role_alice,
                              static_cast<const uint8_t*>(myName.bytes), myName.length,
                              static_cast<const uint8_t*>(theirName.bytes), theirName.length));
  }
  return self;
}

- (NSData*)generateMessageWithPassword:(NSData*)password error:(NSError**)error {
  auto fail = [&](NSString* description) -> NSData* {
    if (error) {
      *error = MakeError(kArcSpake2ErrorDomain, 0, description);
    }
    return nil;
  };
  if (!_ctx) {
    return fail(@"SPAKE2 context is invalid");
  }
  std::array<uint8_t, SPAKE2_MAX_MSG_SIZE> message;
  size_t message_length = 0;
  if (!SPAKE2_generate_msg(_ctx.get(), message.data(), &message_length, message.size(),
                           static_cast<const uint8_t*>(password.bytes), password.length)) {
    return fail(@"SPAKE2 message generation failed");
  }
  return [NSData dataWithBytes:message.data() length:message_length];
}

- (NSData*)processMessage:(NSData*)message error:(NSError**)error {
  auto fail = [&](NSString* description) -> NSData* {
    if (error) {
      *error = MakeError(kArcSpake2ErrorDomain, 0, description);
    }
    return nil;
  };
  if (!_ctx) {
    return fail(@"SPAKE2 context is invalid");
  }
  std::array<uint8_t, SPAKE2_MAX_KEY_SIZE> key;
  size_t key_length = 0;
  if (!SPAKE2_process_msg(_ctx.get(), key.data(), &key_length, key.size(),
                          static_cast<const uint8_t*>(message.bytes), message.length)) {
    return fail(@"SPAKE2 message processing failed");
  }
  return [NSData dataWithBytes:key.data() length:key_length];
}

@end
