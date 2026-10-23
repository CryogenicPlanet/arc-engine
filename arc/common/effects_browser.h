// Copyright 2026 ArcMod. Profile-owned Boost configuration.
#ifndef ARC_COMMON_EFFECTS_BROWSER_H_
#define ARC_COMMON_EFFECTS_BROWSER_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "base/callback_list.h"
#include "base/functional/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/memory/ref_counted.h"

class Profile;

namespace arc {
struct EffectSource {
  std::string identifier;
  std::string source;
};

struct HostEffectsConfiguration {
  std::vector<EffectSource> stylesheets;
  std::vector<EffectSource> scripts;
};

// Original keys are raw strings in lexicographic order. Application compares
// each key exactly against GURL::host_piece(), without pattern normalization.
using ProfileEffectsConfiguration = std::map<std::string, HostEffectsConfiguration>;

// Our native service initializer contract, independent of mutable weak owner.
// Opaque retained identity, distinct from the weak native Profile address.
using EffectsServiceIdentity = base::RefCountedData<uint8_t>;
struct EffectsServiceData {
  base::WeakPtr<Profile> profile;
  scoped_refptr<EffectsServiceIdentity> identity;
};

const ProfileEffectsConfiguration& GetProfileEffectsConfiguration(Profile* profile);
base::CallbackListSubscription ObserveProfileEffectsConfiguration(
    Profile* profile,
    base::RepeatingClosure callback);
}  // namespace arc

#endif  // ARC_COMMON_EFFECTS_BROWSER_H_
