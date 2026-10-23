// Copyright 2026 ArcMod. ArcBrowserApplicationConfiguration and ArcBrowserApplication: the engine's
// entry point for Arc's app. Bootstraps Chrome's browser process inside the app's own NSApplication
// (initialize:) and runs its main loop (run). Methods not implemented here fall back to the logging
// stubs in arc_api_stubs.mm.

#import <AppKit/AppKit.h>

#import "arc/api/arc_internal.h"

#include <string>
#include <vector>

#include "base/check.h"
#include "base/functional/bind.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/sys_string_conversions.h"
#include "base/task/sequenced_task_runner.h"
#include "chrome/app/chrome_main.h"
#include "chrome/browser/lifetime/application_lifetime.h"
#include "components/keep_alive_registry/keep_alive_types.h"
#include "components/keep_alive_registry/scoped_keep_alive.h"
#include "components/version_info/version_info.h"

#define ARC_EXPORT __attribute__((visibility("default")))

@protocol ArcBrowserApplicationDelegate <NSObject>
- (void)didInitializeBrowserProcess;
- (void)didConnectToNetworkProcess;
- (void)didReceiveGPUInfoUpdate:(BOOL)available;
- (void)didQuitRunLoop;
@end

ARC_EXPORT @interface ArcBrowserApplicationConfiguration : NSObject
@end

@implementation ArcBrowserApplicationConfiguration {
  int _argc;
  NSArray<NSString*>* _argv;
  NSString* _profilePath;
  NSUInteger _applicationID;
  BOOL _isProduction;
  NSUInteger _sessionRestorationPolicy;
  BOOL _shouldLoadAllProfilesAtStartup;
  BOOL _quitRunLoopOnTermination;
  NSString* _userDefaultsSuiteName;
}

- (instancetype)init:(int)argc
                                argv:(NSArray<NSString*>*)argv
                         profilePath:(NSString*)profilePath
                       applicationID:(NSUInteger)applicationID
                        isProduction:(BOOL)isProduction
            sessionRestorationPolicy:(NSUInteger)sessionRestorationPolicy
      shouldLoadAllProfilesAtStartup:(BOOL)shouldLoadAllProfilesAtStartup
            quitRunLoopOnTermination:(BOOL)quitRunLoopOnTermination
               userDefaultsSuiteName:(NSString*)userDefaultsSuiteName {
  if ((self = [super init])) {
    _argc = argc;
    _argv = [argv copy];
    _profilePath = [profilePath copy];
    _applicationID = applicationID;
    _isProduction = isProduction;
    _sessionRestorationPolicy = sessionRestorationPolicy;
    _shouldLoadAllProfilesAtStartup = shouldLoadAllProfilesAtStartup;
    _quitRunLoopOnTermination = quitRunLoopOnTermination;
    _userDefaultsSuiteName = [userDefaultsSuiteName copy];
  }
  return self;
}

- (int)argc {
  return _argc;
}
- (NSArray<NSString*>*)argv {
  return _argv;
}
- (NSString*)profilePath {
  return _profilePath;
}
- (NSUInteger)applicationID {
  return _applicationID;
}
- (BOOL)isProduction {
  return _isProduction;
}
- (NSUInteger)sessionRestorationPolicy {
  return _sessionRestorationPolicy;
}
- (BOOL)shouldLoadAllProfilesAtStartup {
  return _shouldLoadAllProfilesAtStartup;
}
- (BOOL)quitRunLoopOnTermination {
  return _quitRunLoopOnTermination;
}
- (NSString*)userDefaultsSuiteName {
  return _userDefaultsSuiteName;
}

@end

// The extensions package can be omitted from a focused build. Its strong
// definition replaces this no-op whenever that package is linked.
namespace arc {
__attribute__((weak)) void InstallExtensionProfileInitializationHook() {}
}

namespace {

// The app's ArcBrowserApplicationDelegate (ADKApplication), held weakly like Arc's engine does.
__weak id<ArcBrowserApplicationDelegate> g_delegate;

// argv for Chrome. CommandLine copies it, but the process title code keeps the pointers.
std::vector<std::string>& ArgvStorage() {
  static base::NoDestructor<std::vector<std::string>> storage;
  return *storage;
}

// Keeps the browser running while Arc's app owns the process, as Chrome's AppController does on the Mac
// (which host mode skips). Released by -quit.
std::unique_ptr<ScopedKeepAlive>& AppKeepAlive() {
  static base::NoDestructor<std::unique_ptr<ScopedKeepAlive>> keep_alive;
  return *keep_alive;
}

bool HasSwitch(const std::vector<std::string>& args, const std::string& name) {
  for (const std::string& arg : args) {
    if (arg == "--" + name || arg.starts_with("--" + name + "=")) {
      return true;
    }
  }
  return false;
}

// Adds |feature| to --<switch_name>=..., merging with a value already on the command line.
void AppendFeatures(std::vector<std::string>& args, const std::string& switch_name, const std::string& feature) {
  const std::string prefix = "--" + switch_name + "=";
  for (std::string& arg : args) {
    if (arg.starts_with(prefix)) {
      arg += "," + feature;
      return;
    }
  }
  args.push_back(prefix + feature);
}

}  // namespace

id ArcBrowserApplicationDelegateObject() {
  return g_delegate;
}

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end

@implementation ArcBrowserApplication

- (instancetype)init {
  return [self init:nil];
}

- (instancetype)init:(id<ArcBrowserApplicationDelegate>)delegate {
  if ((self = [super init])) {
    if (delegate) {
      g_delegate = delegate;
    }
  }
  return self;
}

// Starts Chrome's browser process inside the app's NSApplication: command line, content and browser
// initialization (profiles included), but not the main loop. Returns 0 on success.
- (NSInteger)initialize:(ArcBrowserApplicationConfiguration*)config {
  std::vector<std::string>& args = ArgvStorage();
  args.clear();
  for (NSString* arg in config.argv) {
    args.push_back(base::SysNSStringToUTF8(arg));
  }
  if (args.empty()) {
    args.push_back(base::SysNSStringToUTF8(NSBundle.mainBundle.executablePath));
  }
  // Arc's app owns windows, menus and the app delegate; Chrome must not open its own.
  args.push_back("--arcmod-host-app");
  args.push_back("--no-startup-window");
  args.push_back("--no-first-run");
  // Chrome's code-sign clone protects Chrome's own updater; Arc's app has none.
  AppendFeatures(args, "disable-features", "MacAppCodeSignClone");
  // Arc's engine doesn't expose FedCM (web-features golden), and its account chooser is a views dialog
  // that needs a BrowserView, which Arc's windows don't have.
  AppendFeatures(args, "disable-features", "FedCm");
  if (!HasSwitch(args, "user-data-dir") && config.profilePath.length) {
    args.push_back("--user-data-dir=" + base::SysNSStringToUTF8(config.profilePath));
  }

  static base::NoDestructor<std::vector<const char*>> argv;
  argv->clear();
  for (const std::string& arg : args) {
    argv->push_back(arg.c_str());
  }
  arc::InstallExtensionProfileInitializationHook();
  int exit_code = ChromeMainInitializeHostedBrowser(static_cast<int>(argv->size()), argv->data());
  if (exit_code != 0) {
    LOG(ERROR) << "ArcCore: browser initialization stopped with exit code " << exit_code;
    return exit_code;
  }
  // The app expects these once the main loop runs.
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, base::BindOnce(^{
                                                             [g_delegate didInitializeBrowserProcess];
                                                             [g_delegate didConnectToNetworkProcess];
                                                           }));
  return 0;
}

// Runs the browser's main loop until quit, then shuts the browser down.
- (NSInteger)run {
  // Created here rather than in -initialize: so it lands after the app's observers are registered, as in
  // Arc's engine (its keep-alive crash key is reported from inside -run).
  AppKeepAlive() =
      std::make_unique<ScopedKeepAlive>(KeepAliveOrigin::APP_CONTROLLER, KeepAliveRestartOption::DISABLED);
  int exit_code = ChromeMainRunHostedBrowser();
  [g_delegate didQuitRunLoop];
  return exit_code;
}

// Releases Arc's application keep-alive; Chromium ends -run once its remaining
// keep-alives permit shutdown.
- (void)quit {
  AppKeepAlive().reset();
}

- (NSString*)version {
  return base::SysUTF8ToNSString(std::string(version_info::GetVersionNumber()));
}

// Chromium 156 only reports encryption availability asynchronously (OSCryptAsync's Encryptor), but the
// app asks synchronously right after initialize: and treats NO as a warning state. Report available;
// OSCryptAsync itself still decides how data is encrypted.
- (BOOL)isEncryptionAvailable {
  return YES;
}

@end
