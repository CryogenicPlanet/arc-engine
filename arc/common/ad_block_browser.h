// Copyright 2026 ArcMod. Browser-process adblock configuration and bridge setup.
#ifndef ARC_COMMON_AD_BLOCK_BROWSER_H_
#define ARC_COMMON_AD_BLOCK_BROWSER_H_

#include <cstdint>
#include <string>
#include <vector>

#include "base/callback_list.h"
#include "base/files/file.h"
#include "base/functional/callback.h"

namespace content {
class WebContents;
}

namespace arc {
// Main-thread state. The descriptor is physically read-only and unlinked after
// compilation; each frame receives a duplicate, never the mutable build file.
struct AdBlockConfiguration {
  base::File indexed_ruleset;
  std::vector<std::string> excluded_origins;
  uint64_t ruleset_id = 0;
};

const AdBlockConfiguration& GetAdBlockConfiguration();
base::CallbackListSubscription ObserveAdBlockConfiguration(
    base::RepeatingClosure callback);

// Core must call this immediately after creating a WebContents, before its
// first navigation, so all RenderFrameCreated notifications are observed.
void InitializeRendererBridge(content::WebContents* contents);
}  // namespace arc

#endif  // ARC_COMMON_AD_BLOCK_BROWSER_H_
