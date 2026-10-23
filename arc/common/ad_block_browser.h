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
class RenderProcessHost;
}

class GURL;
class Profile;

namespace arc {
// Main-thread state. The descriptor is physically read-only and unlinked after
// compilation; each renderer process receives a duplicate of the immutable file.
struct AdBlockConfiguration {
  base::File indexed_ruleset;
  std::vector<std::string> excluded_origins;
  uint64_t ruleset_id = 0;
  // Revision of cached exclusions (including re-enable), not rule updates.
  // Disabled mutations advance the global cache without publishing to profiles.
  uint64_t config_version = 0;
  bool enabled = false;
  // An engaged update may carry an invalid file: that explicitly clears rules.
  // Disabling the feature disengages this manager update without resetting the
  // independent profile and renderer copies.
  bool has_ruleset_update = false;
  bool has_cosmetic_rules = false;
};

const AdBlockConfiguration& GetAdBlockConfiguration();
base::CallbackListSubscription ObserveAdBlockConfiguration(
    base::RepeatingClosure callback);

// The application is remembered weakly. Feature11 changes call Refresh after
// releasing the app's feature-table lock because Refresh invokes its getter.
#ifdef __OBJC__
void SetRendererAdBlockApplication(id application);
#endif
void RefreshRendererAdBlockFeature();
// Pure live read for queued publication; does not refresh or mutate state.
bool RendererAdBlockFeatureEnabled();
void InitializeRendererProcessBridge();
void InitializeRendererProfile(Profile* profile);
// Pass true only from a paired embedder patch that also installs the process
// agent in RenderThreadStarted before associated requests can be dispatched.
void RendererProcessWillLaunch(content::RenderProcessHost* host,
                               bool early_process_agent_installed = false);
void RendererProcessFrameReady(content::RenderProcessHost* host);
void ReleaseRendererRulesetFile(base::File file);
void GetRendererAdBlockDebugInfo(
    Profile* profile,
    const GURL& url,
    base::OnceCallback<void(const std::string&)> completion);

// Core must call this immediately after creating a WebContents, before its
// first navigation, so all RenderFrameCreated notifications are observed.
void InitializeRendererBridge(content::WebContents* contents);
}  // namespace arc

#endif  // ARC_COMMON_AD_BLOCK_BROWSER_H_
