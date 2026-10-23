// Copyright 2026 ArcMod. Stock Arc's bundled, hidden tabs event listener.
#import "arc/browser/extensions_context.h"
#include "arc/browser/extensions_internal_manifest.h"

#include "base/command_line.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/one_shot_event.h"
#include "base/strings/sys_string_conversions.h"
#include "base/supports_user_data.h"
#include "chrome/browser/profiles/profile.h"
#include "extensions/browser/extension_registrar.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/extension.h"

namespace arc {
namespace {
constexpr char kRegistrationKey[] = "ArcInternalExtensionRegistration";
class InternalExtensionRegistration : public base::SupportsUserData::Data {};

void Install(base::WeakPtr<Profile> profile) {
  if (!profile) return;
  auto* registry = extensions::ExtensionRegistry::Get(profile.get());
  if (!registry || registry->GetInstalledExtension(std::string(kInternalExtensionId))) return;
  auto manifest = base::JSONReader::ReadDict(
      kInternalExtensionManifest, base::JSON_PARSE_RFC);
  if (!manifest) return;
  NSString* resources = [NSBundle bundleForClass:[ArcExtensionInfo class]].resourcePath;
  base::FilePath directory(base::SysNSStringToUTF8(
      [resources stringByAppendingPathComponent:@"arc_internal_extension"]));
  std::u16string error;
  scoped_refptr<extensions::Extension> extension = extensions::Extension::Create(
      directory, extensions::mojom::ManifestLocation::kComponent, *manifest,
      extensions::Extension::REQUIRE_KEY, &error);
  if (!extension) {
    LOG(ERROR) << "Arc internal component manifest: " << error;
    return;
  }
  // Use the same production registrar as ComponentLoader, with the stock key,
  // manifest, and files. A real worker/listener then drives normal tabs events.
  extensions::ExtensionRegistrar::Get(profile.get())->AddComponentExtension(extension.get());
}
}  // namespace

void EnsureInternalExtension(Profile* profile) {
  if (!base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app") || !profile) return;
  profile = profile->GetOriginalProfile();
  if (profile->GetUserData(kRegistrationKey)) return;
  auto* system = extensions::ExtensionSystem::Get(profile);
  if (!system) return;
  profile->SetUserData(kRegistrationKey, std::make_unique<InternalExtensionRegistration>());
  auto install = base::BindOnce(&Install, profile->GetWeakPtr());
  if (system->is_ready()) std::move(install).Run();
  else system->ready().Post(FROM_HERE, std::move(install));
}
}  // namespace arc
