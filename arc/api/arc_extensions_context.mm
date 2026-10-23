// Copyright 2026 ArcMod. Arc extension services backed by Chromium's profile.
#import "arc/browser/extensions_context.h"
#import "arc/browser/extensions_bridge.h"
#include "arc/browser/extensions_internal_manifest.h"

#include <algorithm>
#include <array>
#include <map>
#include <set>
#include <utility>

#include "base/auto_reset.h"
#include "base/functional/bind.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/one_shot_event.h"
#include "base/strings/sys_string_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "base/supports_user_data.h"
#include "base/task/sequenced_task_runner.h"
#include "chrome/browser/extensions/commands/command_service.h"
#include "chrome/browser/extensions/error_console/error_console.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/extensions/webstore_install_with_prompt.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/tab_list/tab_list_interface.h"
#include "chrome/browser/ui/browser_window/public/profile_browser_collection.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/toolbar/toolbar_actions_model.h"
#include "chrome/common/extensions/api/omnibox/omnibox_handler.h"
#include "components/tabs/public/tab_interface.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/isolated_world_ids.h"
#include "extensions/browser/event_router.h"
#include "extensions/browser/extension_error.h"
#include "extensions/browser/extension_host.h"
#include "extensions/browser/extension_icon_image.h"
#include "extensions/browser/extension_registrar.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_registry_observer.h"
#include "extensions/browser/extension_system.h"
#include "extensions/browser/load_error_reporter.h"
#include "extensions/browser/extension_util.h"
#include "extensions/browser/permissions/active_tab_permission_granter.h"
#include "extensions/browser/process_manager.h"
#include "extensions/browser/script_executor.h"
#include "extensions/browser/scripting_utils.h"
#include "extensions/common/mojom/execution_world.mojom.h"
#include "extensions/browser/unpacked_installer.h"
#include "extensions/browser/webstore_install_result.h"
#include "extensions/common/api/extension_action/action_info.h"
#include "extensions/common/extension.h"
#include "extensions/common/manifest_handlers/icons_handler.h"
#include "extensions/common/manifest_handlers/description_info.h"
#include "extensions/common/manifest_handlers/manifest_url_handlers.h"
#include "extensions/common/permissions/permissions_data.h"
#include "url/origin.h"

@interface ArcBrowser (ArcExtensionCommandDispatch)
- (void)executeExtensionAction:(NSString*)extensionID;
@end

@protocol ArcExtensionsContextCallbacks <NSObject>
- (void)browserContext:(ArcBrowserContext*)context didEnableExtension:(NSString*)identifier;
- (void)browserContext:(ArcBrowserContext*)context didDisableExtension:(NSString*)identifier;
- (void)browserContext:(ArcBrowserContext*)context didInstallExtension:(NSString*)identifier;
- (void)browserContext:(ArcBrowserContext*)context didUninstallExtension:(NSString*)identifier;
- (void)browserContext:(ArcBrowserContext*)context didFailLoadUnpacked:(NSString*)path
    withManifestError:(NSString*)message;
- (void)browserContext:(ArcBrowserContext*)context didFail:(NSString*)identifier
     withRuntimeError:(ArcExtensionRuntimeError*)error;
- (void)browserContext:(ArcBrowserContext*)context didFail:(NSString*)identifier
    withManifestError:(NSString*)message;
- (void)browserContext:(ArcBrowserContext*)context didFail:(NSString*)identifier
    withInternalError:(NSString*)message;
- (void)browserContext:(ArcBrowserContext*)context didChangePinnedExtensionOrder:(NSArray*)order;
- (void)browserContextDidExtensionSystemReady:(ArcBrowserContext*)context;
@end

@interface ArcBrowserContextExtensions ()
- (id<ArcExtensionsContextCallbacks>)arc_delegate __attribute__((objc_direct));
@end

namespace {
constexpr int kInstalledMask = extensions::ExtensionRegistry::ENABLED |
                               extensions::ExtensionRegistry::DISABLED |
                               extensions::ExtensionRegistry::TERMINATED;

bool VisibleExtension(Profile* profile, const extensions::Extension& extension) {
  return profile && extension.is_extension() &&
         !extensions::Manifest::IsComponentLocation(extension.location()) &&
         (!profile->IsOffTheRecord() ||
          extensions::util::IsIncognitoEnabled(extension.id(), profile));
}

// IconImage owns its asynchronous load and unregisters from the registry on
// shutdown. Retain it on the profile so a later snapshot sees the loaded icon.
class ExtensionIcons : public base::SupportsUserData::Data {
 public:
  std::map<std::string, std::unique_ptr<extensions::IconImage>> icons;
};
constexpr char kIconsKey[] = "ArcExtensionsSnapshotIcons";

arc::Image ExtensionIcon(Profile* profile, const extensions::Extension& extension) {
  auto* cache = static_cast<ExtensionIcons*>(profile->GetUserData(kIconsKey));
  if (!cache) {
    auto data = std::make_unique<ExtensionIcons>();
    cache = data.get();
    profile->SetUserData(kIconsKey, std::move(data));
  }
  auto& icon = cache->icons[extension.id()];
  if (!icon || !icon->is_valid()) {
    icon = std::make_unique<extensions::IconImage>(
        profile, &extension, extensions::IconsInfo::GetIcons(&extension), 32,
        extensions::util::GetDefaultExtensionIcon(), nullptr);
  }
  icon->image_skia().GetRepresentation(1.0f);
  return arc::Image(icon->image());
}

NSArray<NSString*>* PinnedOrder(ToolbarActionsModel* model) {
  NSMutableArray<NSString*>* result = [NSMutableArray array];
  if (model) for (const auto& identifier : model->pinned_action_ids())
    [result addObject:ArcNSString(identifier)];
  return [result copy];
}

class ArcWebstoreInstaller : public extensions::WebstoreInstallWithPrompt {
 public:
  ArcWebstoreInstaller(const std::string& id, Profile* profile, bool unattended,
                      Callback callback)
      : WebstoreInstallWithPrompt(id, profile, gfx::NativeWindow(),
                                  std::move(callback)),
        unattended_(unattended) {
    set_show_post_install_ui(false);
  }
 protected:
  ~ArcWebstoreInstaller() override = default;
  std::unique_ptr<extensions::InstallPromptData> CreateInstallPrompt() const override {
    return unattended_ ? nullptr : WebstoreInstallWithPrompt::CreateInstallPrompt();
  }
 private:
  const bool unattended_;
};

NSInteger ArcInstallResult(extensions::webstore_install::Result result) {
  // Verified Arc native-result -> public-result mapping, composed with the
  // Chromium WebstoreStandaloneInstaller result conversion.
  constexpr std::array<NSInteger, 15> kResults = {0, 2, 2, 2, 4, 3, 6, 6, 1, 2, 2, 5, 2, 2, 5};
  const auto index = static_cast<size_t>(result);
  return index < std::size(kResults) ? kResults[index] : 2;
}

// Native DelegateWrapper queues a weak bridge. Replacing a nonnil delegate
// updates that bridge; clearing it invalidates already queued notifications.
struct ExtensionCallbackTarget {
  ExtensionCallbackTarget(id<ArcExtensionsContextCallbacks> target,
                          ArcBrowserContext* owner)
      : delegate(target), context(owner) {}
  __weak id<ArcExtensionsContextCallbacks> delegate;
  __weak ArcBrowserContext* context;
};

void PostExtensionCallback(
    std::weak_ptr<ExtensionCallbackTarget> target,
    void (^callback)(id<ArcExtensionsContextCallbacks>, ArcBrowserContext*)) {
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(
          [](std::weak_ptr<ExtensionCallbackTarget> weak,
             void (^notify)(id<ArcExtensionsContextCallbacks>, ArcBrowserContext*)) {
            auto state = weak.lock();
            if (!state) return;
            id<ArcExtensionsContextCallbacks> delegate = state->delegate;
            ArcBrowserContext* context = state->context;
            if (delegate) notify(delegate, context);
          }, std::move(target), [callback copy]));
}

class ExtensionContextObserver : public extensions::ExtensionRegistryObserver,
                                 public extensions::ErrorConsole::Observer,
                                 public extensions::LoadErrorReporter::Observer,
                                 public ToolbarActionsModel::Observer {
 public:
  ExtensionContextObserver(Profile* profile,
                           std::shared_ptr<ExtensionCallbackTarget> callbacks,
                           std::shared_ptr<bool> order_suppression)
      : callbacks_(callbacks),
        order_suppression_(std::move(order_suppression)), registry_(extensions::ExtensionRegistry::Get(profile)),
        errors_(extensions::ErrorConsole::Get(profile)),
        toolbar_(ToolbarActionsModel::Get(profile)) {
    if (registry_) registry_->AddObserver(this);
    if (errors_) errors_->AddObserver(this);
    load_errors_ = extensions::LoadErrorReporter::GetInstance();
    load_errors_->AddObserver(this);
    if (toolbar_) toolbar_->AddObserver(this);
    if (auto* system = extensions::ExtensionSystem::Get(profile)) {
      if (!system->is_ready())
        system->ready().Post(FROM_HERE,
            base::BindOnce(&ExtensionContextObserver::Ready, weak_factory_.GetWeakPtr()));
    }
  }
  ~ExtensionContextObserver() override { Detach(); }

  void Detach() {
    weak_factory_.InvalidateWeakPtrs();
    if (registry_) registry_->RemoveObserver(this);
    if (errors_) errors_->RemoveObserver(this);
    if (load_errors_) load_errors_->RemoveObserver(this);
    if (toolbar_) toolbar_->RemoveObserver(this);
    registry_ = nullptr;
    errors_ = nullptr;
    load_errors_ = nullptr;
    toolbar_ = nullptr;
  }
  void OnShutdown(extensions::ExtensionRegistry*) override { Detach(); }
  void OnErrorConsoleDestroyed() override { errors_ = nullptr; }
  void OnToolbarActionsModelShutdown() override {
    toolbar_->RemoveObserver(this);
    toolbar_ = nullptr;
  }
  void OnExtensionLoaded(content::BrowserContext*,
                         const extensions::Extension* extension) override {
    if (extension->id() == arc::kInternalExtensionId) return;
    NSString* identifier = base::SysUTF8ToNSString(extension->id());
    PostExtensionCallback(callbacks_, ^(id<ArcExtensionsContextCallbacks> delegate,
                                       ArcBrowserContext* context) {
      if ([delegate respondsToSelector:@selector(browserContext:didEnableExtension:)])
        [delegate browserContext:context didEnableExtension:identifier];
    });
  }
  void OnExtensionUnloaded(content::BrowserContext*,
                           const extensions::Extension* extension,
                           extensions::UnloadedExtensionReason) override {
    if (extension->id() == arc::kInternalExtensionId) return;
    NSString* identifier = base::SysUTF8ToNSString(extension->id());
    PostExtensionCallback(callbacks_, ^(id<ArcExtensionsContextCallbacks> delegate,
                                       ArcBrowserContext* context) {
      if ([delegate respondsToSelector:@selector(browserContext:didDisableExtension:)])
        [delegate browserContext:context didDisableExtension:identifier];
    });
  }
  void OnExtensionInstalled(content::BrowserContext*,
                            const extensions::Extension* extension, bool) override {
    if (extension->id() == arc::kInternalExtensionId) return;
    NSString* identifier = base::SysUTF8ToNSString(extension->id());
    PostExtensionCallback(callbacks_, ^(id<ArcExtensionsContextCallbacks> delegate,
                                       ArcBrowserContext* context) {
      if ([delegate respondsToSelector:@selector(browserContext:didInstallExtension:)])
        [delegate browserContext:context didInstallExtension:identifier];
    });
  }
  void OnExtensionUninstalled(content::BrowserContext*,
                              const extensions::Extension* extension,
                              extensions::UninstallReason) override {
    if (extension->id() == arc::kInternalExtensionId) return;
    NSString* identifier = base::SysUTF8ToNSString(extension->id());
    PostExtensionCallback(callbacks_, ^(id<ArcExtensionsContextCallbacks> delegate,
                                       ArcBrowserContext* context) {
      if ([delegate respondsToSelector:@selector(browserContext:didUninstallExtension:)])
        [delegate browserContext:context didUninstallExtension:identifier];
    });
  }
  void OnLoadFailure(content::BrowserContext*, const base::FilePath& path,
                     const std::u16string& error) override {
    // Stock observes the global reporter without filtering its source profile.
    NSString* filename = base::SysUTF8ToNSString(path.value());
    NSString* message = base::SysUTF16ToNSString(error);
    PostExtensionCallback(callbacks_, ^(id<ArcExtensionsContextCallbacks> delegate,
                                       ArcBrowserContext* context) {
      if ([delegate respondsToSelector:@selector(browserContext:didFailLoadUnpacked:withManifestError:)])
        [delegate browserContext:context didFailLoadUnpacked:filename
              withManifestError:message];
    });
  }
  void OnErrorAdded(const extensions::ExtensionError* error) override {
    NSString* identifier = base::SysUTF8ToNSString(error->extension_id());
    NSString* message = base::SysUTF16ToNSString(error->message());
    switch (error->type()) {
      case extensions::ExtensionError::Type::kRuntimeError: {
        const auto& runtime = *static_cast<const extensions::RuntimeError*>(error);
        arc::ExtensionRuntimeError snapshot;
        snapshot.extension_id = runtime.extension_id();
        snapshot.context_url_spec = runtime.context_url().spec();
        snapshot.message = runtime.message();
        snapshot.source = runtime.source();
        snapshot.from_incognito = runtime.from_incognito();
        snapshot.severity = runtime.level();
        for (const auto& frame : runtime.stack_trace()) {
          snapshot.stack_trace.push_back({frame.line_number, frame.column_number,
                                          frame.source, frame.function});
        }
        PostExtensionCallback(callbacks_, ^(id<ArcExtensionsContextCallbacks> delegate,
                                           ArcBrowserContext* context) {
          if ([delegate respondsToSelector:@selector(browserContext:didFail:withRuntimeError:)])
            [delegate browserContext:context didFail:identifier
                withRuntimeError:[[ArcExtensionRuntimeError alloc] initWithExtensionRuntimeError:&snapshot]];
        });
        break;
      }
      case extensions::ExtensionError::Type::kManifestError: {
        PostExtensionCallback(callbacks_, ^(id<ArcExtensionsContextCallbacks> delegate,
                                           ArcBrowserContext* context) {
          if ([delegate respondsToSelector:@selector(browserContext:didFail:withManifestError:)])
            [delegate browserContext:context didFail:identifier withManifestError:message];
        });
        break;
      }
      case extensions::ExtensionError::Type::kInternalError: {
        PostExtensionCallback(callbacks_, ^(id<ArcExtensionsContextCallbacks> delegate,
                                           ArcBrowserContext* context) {
          if ([delegate respondsToSelector:@selector(browserContext:didFail:withInternalError:)])
            [delegate browserContext:context didFail:identifier withInternalError:message];
        });
        break;
      }
      case extensions::ExtensionError::Type::kNumErrorTypes:
        break;
    }
  }
  void OnToolbarActionAdded(const std::string&) override {}
  void OnToolbarActionRemoved(const std::string&) override {}
  void OnToolbarActionUpdated(const std::string&) override {}
  void OnToolbarModelInitialized() override {}
  void OnToolbarPinnedActionsChanged() override {
    if (*order_suppression_) return;
    NSArray* order = PinnedOrder(toolbar_);
    PostExtensionCallback(callbacks_, ^(id<ArcExtensionsContextCallbacks> delegate,
                                       ArcBrowserContext* context) {
      if ([delegate respondsToSelector:@selector(browserContext:didChangePinnedExtensionOrder:)])
        [delegate browserContext:context didChangePinnedExtensionOrder:order];
    });
  }
 private:
  void Ready() {
    PostExtensionCallback(callbacks_, ^(id<ArcExtensionsContextCallbacks> delegate,
                                       ArcBrowserContext* context) {
      if ([delegate respondsToSelector:@selector(browserContextDidExtensionSystemReady:)])
        [delegate browserContextDidExtensionSystemReady:context];
    });
  }
  const std::weak_ptr<ExtensionCallbackTarget> callbacks_;
  const std::shared_ptr<bool> order_suppression_;
  raw_ptr<extensions::ExtensionRegistry> registry_;
  raw_ptr<extensions::ErrorConsole> errors_;
  raw_ptr<extensions::LoadErrorReporter> load_errors_;
  raw_ptr<ToolbarActionsModel> toolbar_;
  base::WeakPtrFactory<ExtensionContextObserver> weak_factory_{this};
};
}  // namespace

namespace arc {
ExtensionInfo MakeExtensionInfo(Profile* profile, const extensions::Extension& extension) {
  ExtensionInfo result;
  result.name = extension.name();
  result.id = extension.id();
  result.url = extension.url().spec();
  result.description = extensions::DescriptionInfo::GetDescription(extension);
  for (const auto& message : extension.permissions_data()->GetPermissionMessages())
    result.permissions.push_back(message.message());
  if (profile) {
    result.icon = ExtensionIcon(profile, extension);
    if (auto* registry = extensions::ExtensionRegistry::Get(profile))
      result.enabled = registry->enabled_extensions().Contains(extension.id());
  }
  result.has_action = extensions::ActionInfo::GetExtensionActionInfo(&extension) != nullptr;
  result.version = extension.VersionString();
  result.omnibox_keyword = extensions::OmniboxInfo::GetKeyword(&extension);
  result.from_webstore = extension.from_webstore();
  return result;
}
}  // namespace arc

@implementation ArcBrowserContextExtensions {
  base::WeakPtr<Profile> _profile;
  uintptr_t _serviceIdentity;
  __weak ArcBrowserContext* _owner;
  __weak id<ArcExtensionsContextCallbacks> _delegate;
  std::unique_ptr<ExtensionContextObserver> _observer;
  std::shared_ptr<bool> _orderSuppression;
  std::shared_ptr<ExtensionCallbackTarget> _callbackTarget;
}
@synthesize owner = _owner;
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context {
  return [self initWithBrowserContextExtensions:nullptr forBrowserContext:context];
}
- (instancetype)initWithBrowserContextExtensions:(const void*)extensions
                              forBrowserContext:(ArcBrowserContext*)context {
  if ((self = [super init])) {
    // Native C++ storage from the original engine is not ABI compatible. Our
    // service factory supplies the owner; its profile identifies the service.
    _owner = context;
    _orderSuppression = std::make_shared<bool>(false);
    if (Profile* profile = ArcProfileFrom(context)) {
      _profile = profile->GetWeakPtr();
      _serviceIdentity = reinterpret_cast<uintptr_t>(profile);
      arc::EnsureInternalExtension(profile);
    }
  }
  return self;
}
- (BOOL)isEqual:(id)object {
  return object == self || ([object isKindOfClass:[ArcBrowserContextExtensions class]] &&
      _serviceIdentity == ((ArcBrowserContextExtensions*)object)->_serviceIdentity);
}
- (NSUInteger)hash { return std::hash<uintptr_t>()(_serviceIdentity); }
- (id<ArcExtensionsContextCallbacks>)arc_delegate { return _delegate; }
- (void)setDelegate:(id)delegate {
  _delegate = delegate;
  if (!delegate) {
    _observer.reset();
    _callbackTarget.reset();
    return;
  }
  if (_callbackTarget) _callbackTarget->delegate = delegate;
  else _callbackTarget = std::make_shared<ExtensionCallbackTarget>(delegate, _owner);
  if (!_observer && _profile)
    _observer = std::make_unique<ExtensionContextObserver>(
        _profile.get(), _callbackTarget, _orderSuppression);
}
- (BOOL)isExtensionSystemReady {
  auto* system = _profile ? extensions::ExtensionSystem::Get(_profile.get()) : nullptr;
  return system && system->is_ready();
}
- (NSArray<ArcExtensionInfo*>*)listExtensions {
  NSMutableArray* result = [NSMutableArray array];
  auto* registry = _profile ? extensions::ExtensionRegistry::Get(_profile.get()) : nullptr;
  if (!registry) return [result copy];
  for (const auto& extension : registry->GenerateInstalledExtensionsSet(kInstalledMask)) {
    if (!VisibleExtension(_profile.get(), *extension)) continue;
    auto info = arc::MakeExtensionInfo(_profile.get(), *extension);
    [result addObject:[[ArcExtensionInfo alloc] initWithExtensionInfo:&info]];
  }
  return [result copy];
}
- (ArcExtensionInfo*)getExtensionInfo:(NSString*)extensionID {
  auto* registry = _profile ? extensions::ExtensionRegistry::Get(_profile.get()) : nullptr;
  const auto* extension = registry ? registry->GetExtensionById(base::SysNSStringToUTF8(extensionID), kInstalledMask) : nullptr;
  if (!extension || !VisibleExtension(_profile.get(), *extension)) return nil;
  auto info = arc::MakeExtensionInfo(_profile.get(), *extension);
  return [[ArcExtensionInfo alloc] initWithExtensionInfo:&info];
}
- (void)enableExtension:(NSString*)extensionID {
  if (_profile) extensions::ExtensionRegistrar::Get(_profile.get())->EnableExtension(base::SysNSStringToUTF8(extensionID));
}
- (void)disableExtension:(NSString*)extensionID {
  if (_profile) extensions::ExtensionRegistrar::Get(_profile.get())->DisableExtension(
      base::SysNSStringToUTF8(extensionID), {extensions::disable_reason::DISABLE_USER_ACTION});
}
- (void)reloadExtension:(NSString*)extensionID {
  if (_profile) extensions::ExtensionRegistrar::Get(_profile.get())->ReloadExtension(base::SysNSStringToUTF8(extensionID));
}
- (void)uninstallExtension:(NSString*)extensionID {
  if (_profile) {
    const std::string identifier = base::SysNSStringToUTF8(extensionID);
    auto* registry = extensions::ExtensionRegistry::Get(_profile.get());
    if (!registry || !registry->GetInstalledExtension(identifier)) return;
    std::u16string error;
    if (auto* registrar = extensions::ExtensionRegistrar::Get(_profile.get()))
      registrar->UninstallExtension(identifier, extensions::UNINSTALL_REASON_USER_INITIATED, &error);
  }
}
- (void)installExtensionFromWebstore:(NSString*)extensionID unattended:(BOOL)unattended
                        completion:(void (^)(NSString*, NSInteger))completion {
  void (^callback)(NSString*, NSInteger) = [completion copy];
  NSString* requestedID = [extensionID copy];
  if (!_profile) { if (callback) callback(requestedID, 2); return; }
  auto installer = base::MakeRefCounted<ArcWebstoreInstaller>(
      base::SysNSStringToUTF8(extensionID), _profile.get(), unattended,
      base::BindOnce([](void (^done)(NSString*, NSInteger), NSString* identifier,
                        bool, const std::string&, extensions::webstore_install::Result result) {
        if (done) done(identifier, ArcInstallResult(result));
      }, callback, requestedID));
  installer->BeginInstall();
}
- (void)installExtensionFromUnpackedPath:(NSString*)path
                            completion:(void (^)(NSString*, NSInteger))completion {
  void (^callback)(NSString*, NSInteger) = [completion copy];
  if (!_profile) { if (callback) callback(@"", 1); return; }
  auto installer = extensions::UnpackedInstaller::Create(_profile.get());
  installer->set_be_noisy_on_failure(true);
  installer->set_completion_callback(base::BindOnce(
      [](void (^done)(NSString*, NSInteger), const extensions::Extension* extension,
         const base::FilePath&, const std::u16string&) {
        if (done) done(extension ? base::SysUTF8ToNSString(extension->id()) : @"", extension ? 0 : 1);
      }, callback));
  installer->Load(base::FilePath(base::SysNSStringToUTF8(path)));
}
- (BOOL)runJSInExtensionBackgroundPage:(NSString*)extensionID js_code:(NSString*)code {
  if (!_profile) return NO;
  const std::string identifier = base::SysNSStringToUTF8(extensionID);
  auto* manager = extensions::ProcessManager::Get(_profile.get());
  auto* host = manager ? manager->GetBackgroundHostForExtension(identifier) : nullptr;
  auto* frame = host && host->host_contents() ? host->host_contents()->GetPrimaryMainFrame() : nullptr;
  if (!frame || !frame->IsRenderFrameLive() ||
      frame->GetLastCommittedOrigin() != url::Origin::Create(extensions::Extension::GetBaseURLFromExtensionId(identifier))) return NO;
  // ScriptExecutor permits an extension to inject into its own origin. Use
  // the main world so the script sees the background page's globals/bindings.
  extensions::ScriptExecutor executor(host->host_contents());
  std::vector<extensions::mojom::JSSourcePtr> sources;
  sources.push_back(extensions::mojom::JSSource::New(
      base::SysNSStringToUTF8(code), GURL()));
  extensions::scripting::ExecuteScript(identifier, std::move(sources),
      extensions::mojom::ExecutionWorld::kMain, std::nullopt, &executor,
      extensions::ScriptExecutor::SPECIFIED_FRAMES, {0},
      /*inject_immediately=*/true, /*user_gesture=*/false,
      base::DoNothing());
  return YES;
}
- (NSArray<ArcExtensionCommand*>*)getAllExtensionCommands {
  NSMutableArray* result = [NSMutableArray array];
  if (!_profile) return [result copy];
  auto* registry = extensions::ExtensionRegistry::Get(_profile.get());
  auto* service = extensions::CommandService::Get(_profile.get());
  if (!registry || !service) return [result copy];
  for (const auto& extension : registry->GenerateInstalledExtensionsSet(kInstalledMask)) {
    if (!VisibleExtension(_profile.get(), *extension)) continue;
    ui::CommandMap commands;
    service->GetNamedCommands(extension->id(), extensions::CommandService::ALL,
                             extensions::CommandService::ANY_SCOPE, &commands);
    for (auto type : {extensions::ActionInfo::Type::kAction,
                      extensions::ActionInfo::Type::kBrowser,
                      extensions::ActionInfo::Type::kPage}) {
      extensions::Command command;
      if (service->GetExtensionActionCommand(extension->id(), type,
          extensions::CommandService::ALL, &command, nullptr))
        commands.insert_or_assign(command.command_name(), command);
    }
    for (const auto& [name, command] : commands) {
      arc::ExtensionCommand snapshot;
      snapshot.extension_id = extension->id();
      snapshot.command_name = name;
      snapshot.command_description = base::UTF16ToUTF8(command.description());
      snapshot.is_global = command.global();
      if (command.accelerator().key_code() != ui::VKEY_UNKNOWN)
        snapshot.shortcut = ui::Command::AcceleratorToString(command.accelerator());
      [result addObject:[[ArcExtensionCommand alloc] initWithExtensionCommand:&snapshot]];
    }
  }
  return [result copy];
}
- (void)executeExtensionCommand:(NSString*)extensionID commandName:(NSString*)name {
  if (!_profile) return;
  const std::string identifier = base::SysNSStringToUTF8(extensionID);
  const std::string command = base::SysNSStringToUTF8(name);
  auto* registry = extensions::ExtensionRegistry::Get(_profile.get());
  const auto* extension = registry ? registry->enabled_extensions().GetByID(identifier) : nullptr;
  if (!extension || !VisibleExtension(_profile.get(), *extension)) return;
  BrowserWindowInterface* browser = nullptr;
  ProfileBrowserCollection::GetForProfile(_profile.get())->ForEach(
      [&](BrowserWindowInterface* candidate) {
        if (candidate->GetType() == BrowserWindowInterface::TYPE_NORMAL &&
            !candidate->IsDeleteScheduled() && arc::BrowserForHostedExtensions(candidate)) {
          browser = candidate;
          return false;
        }
        return true;
      }, BrowserCollection::Order::kActivation);
  if (extensions::Command::IsActionRelatedCommand(command)) {
    if (browser) [arc::BrowserForHostedExtensions(browser) executeExtensionAction:extensionID];
    return;
  }
  base::ListValue arguments;
  arguments.Append(command);
  base::Value tab;
  auto* tabs = browser ? TabListInterface::From(browser) : nullptr;
  auto* active = tabs ? tabs->GetActiveTab() : nullptr;
  auto* contents = active ? active->GetContents() : nullptr;
  if (contents) {
    if (auto* granter = extensions::ActiveTabPermissionGranter::FromWebContents(contents))
      granter->GrantIfRequested(extension);
    auto scrub = extensions::ExtensionTabUtil::GetScrubTabBehavior(
        extension, extensions::mojom::ContextType::kPrivilegedExtension, contents);
    tab = base::Value(extensions::ExtensionTabUtil::CreateTabObject(contents, scrub, extension).ToValue());
  }
  arguments.Append(std::move(tab));
  auto event = std::make_unique<extensions::Event>(extensions::events::COMMANDS_ON_COMMAND,
      "commands.onCommand", std::move(arguments), _profile.get());
  event->user_gesture = extensions::EventRouter::UserGestureState::kEnabled;
  if (auto* router = extensions::EventRouter::Get(_profile.get()))
    router->DispatchEventToExtension(identifier, std::move(event));
}
- (NSArray<NSString*>*)getPinnedExtensionOrder {
  return PinnedOrder(_profile ? ToolbarActionsModel::Get(_profile.get()) : nullptr);
}
- (void)setPinnedExtensionOrder:(NSArray<NSString*>*)order {
  auto profile = _profile;
  if (!profile || profile->IsOffTheRecord()) return;
  // Reacquire after each model mutation: another observer can reenter the app
  // and destroy the profile or clear this facade's delegate.
  auto currentModel = [&profile]() -> ToolbarActionsModel* {
    return profile ? ToolbarActionsModel::Get(profile.get()) : nullptr;
  };
  auto* model = currentModel();
  if (!model) return;
  std::vector<std::string> requested;
  std::set<std::string> seen;
  for (NSString* identifier in order) {
    std::string value = base::SysNSStringToUTF8(identifier);
    if (seen.insert(value).second) requested.push_back(std::move(value));
  }
  // Shared independently of the observer: reentrant setDelegate:nil can delete
  // the observer while this guard is alive.
  auto suppression = _orderSuppression;
  base::AutoReset<bool> setting(suppression.get(), true);
  const auto previous = model->pinned_action_ids();
  for (const auto& identifier : previous) {
    model = currentModel();
    if (!model) return;
    if (!seen.contains(identifier) && model->IsActionPinned(identifier) &&
        !model->IsActionForcePinned(identifier))
      model->SetActionVisibility(identifier, false);
  }
  std::vector<std::string> pinned;
  for (const auto& identifier : requested) {
    model = currentModel();
    if (!model) return;
    if (!model->HasAction(identifier) || model->IsActionForcePinned(identifier)) continue;
    if (!model->IsActionPinned(identifier)) model->SetActionVisibility(identifier, true);
    model = currentModel();
    if (!model) return;
    if (model->IsActionPinned(identifier)) pinned.push_back(identifier);
  }
  size_t index = 0;
  for (const auto& identifier : pinned) {
    model = currentModel();
    if (!model) return;
    if (model->IsActionPinned(identifier) && !model->IsActionForcePinned(identifier))
      model->MovePinnedAction(identifier, index++);
  }
}
@end
