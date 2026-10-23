// Copyright 2026 ArcMod. Color schemes and color filters of Boosts and tabs.
//
// Arc's engine applies both through Blink Page state (a preferred-color-scheme optional layered above the
// media overrides, and an HSLA paint filter); see arc/common/RENDERER_VISUAL_GAPS.md. Neither exists in this
// Chromium yet (renderer_effects_*_proposal.patch are unapplied). Until they land:
//  - schemes are applied through the existing stylesheet path as `color-scheme` on the root element, which
//    restyles UA-drawn parts and `light-dark()` but does not flip `prefers-color-scheme` media queries;
//  - filters are recorded (per tab and per profile host) for the future renderer transport.
#import "arc/common/renderer_values.h"

#include <array>
#include <map>
#include <optional>
#include <string>
#include <utility>

#include "base/no_destructor.h"
#include "base/supports_user_data.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_user_data.h"

// Defined in arc_renderer_effects.mm / arc_renderer_web_contents.mm.
@interface ArcBrowserContextEffects : NSObject
@property(nonatomic, weak) ArcBrowserContext* owner;
- (void)registerStylesheet:(NSString*)stylesheet
            withIdentifier:(NSString*)identifier
            forHostPattern:(NSString*)hostPattern;
- (void)unregisterStylesheet:(NSString*)identifier;
@end

@interface ArcWebContents (RendererStylesheets)
- (void)insertStylesheet:(NSString*)identifier stylesheet:(NSString*)stylesheet;
- (void)removeStylesheet:(NSString*)identifier;
@end

namespace arc {

using ColorFilterMatrix = std::array<float, 20>;

namespace {

constexpr NSInteger kSchemeLight = 0;
constexpr NSInteger kSchemeDark = 1;

ColorFilterMatrix MatrixOf(const HSLAMatrix& m) {
  return {m.m0,  m.m1,  m.m2,  m.m3,  m.m4,  m.m5,  m.m6,  m.m7,  m.m8,  m.m9,
          m.m10, m.m11, m.m12, m.m13, m.m14, m.m15, m.m16, m.m17, m.m18, m.m19};
}

// nil for a value outside the public enum (0 Light, 1 Dark).
NSString* SchemeStylesheet(NSInteger scheme) {
  switch (scheme) {
    case kSchemeLight: return @":root { color-scheme: light !important; }";
    case kSchemeDark: return @":root { color-scheme: dark !important; }";
    default: return nil;
  }
}

NSString* HostSchemeIdentifier(NSString* hostPattern) {
  return [@"arc.color-scheme:" stringByAppendingString:hostPattern ?: @""];
}

NSString* const kTabSchemeIdentifier = @"arc.color-scheme";

class ProfileColorFilters final : public base::SupportsUserData::Data {
 public:
  static ProfileColorFilters* For(Profile* profile) {
    auto* data = static_cast<ProfileColorFilters*>(profile->GetUserData(&kKey));
    if (!data) {
      auto owned = std::make_unique<ProfileColorFilters>();
      data = owned.get();
      profile->SetUserData(&kKey, std::move(owned));
    }
    return data;
  }
  std::map<std::string, ColorFilterMatrix> filters;

 private:
  inline static const int kKey = 0;
};

class TabColorFilter final : public content::WebContentsUserData<TabColorFilter> {
 public:
  std::optional<ColorFilterMatrix> filter;

 private:
  friend class content::WebContentsUserData<TabColorFilter>;
  explicit TabColorFilter(content::WebContents* contents)
      : content::WebContentsUserData<TabColorFilter>(*contents) {}
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(TabColorFilter);

}  // namespace

// For the future renderer transport: the filter registered for |host| in |profile|, if any.
std::optional<ColorFilterMatrix> ProfileColorFilterForHost(Profile* profile, const std::string& host) {
  if (!profile) {
    return std::nullopt;
  }
  const auto& filters = ProfileColorFilters::For(profile)->filters;
  const auto found = filters.find(host);
  return found == filters.end() ? std::nullopt : std::optional(found->second);
}

// For the future renderer transport: the filter applied to |contents|, if any.
std::optional<ColorFilterMatrix> AppliedTabColorFilter(content::WebContents* contents) {
  auto* data = contents ? TabColorFilter::FromWebContents(contents) : nullptr;
  return data ? data->filter : std::nullopt;
}

}  // namespace arc

@implementation ArcBrowserContextEffects (ColorEffects)

- (void)registerColorFilter:(const float[20])matrix forHostPattern:(NSString*)hostPattern {
  Profile* profile = ArcProfileFrom(self.owner);
  if (!profile || !matrix) {
    return;
  }
  arc::ColorFilterMatrix copy;
  std::copy_n(matrix, copy.size(), copy.begin());
  arc::ProfileColorFilters::For(profile)->filters[ArcUTF8(hostPattern)] = copy;
}

- (void)unregisterColorFilter:(NSString*)hostPattern {
  if (Profile* profile = ArcProfileFrom(self.owner)) {
    arc::ProfileColorFilters::For(profile)->filters.erase(ArcUTF8(hostPattern));
  }
}

- (void)registerColorScheme:(NSInteger)scheme forHostPattern:(NSString*)hostPattern {
  NSString* css = arc::SchemeStylesheet(scheme);
  if (!css) {
    return;
  }
  [self registerStylesheet:css
            withIdentifier:arc::HostSchemeIdentifier(hostPattern)
            forHostPattern:hostPattern];
}

- (void)unregisterColorScheme:(NSString*)hostPattern {
  [self unregisterStylesheet:arc::HostSchemeIdentifier(hostPattern)];
}

@end

@implementation ArcWebContents (ColorEffects)

- (void)applyColorScheme:(NSInteger)scheme {
  NSString* css = arc::SchemeStylesheet(scheme);
  if (!css || ![self webContents]) {
    return;
  }
  [self removeStylesheet:arc::kTabSchemeIdentifier];
  [self insertStylesheet:arc::kTabSchemeIdentifier stylesheet:css];
}

- (void)removeColorScheme {
  if ([self webContents]) {
    [self removeStylesheet:arc::kTabSchemeIdentifier];
  }
}

- (void)applyColorFilter:(ArcHSLAMatrix*)matrix {
  content::WebContents* contents = [self webContents];
  if (!contents) {
    return;
  }
  // A nil matrix clears the filter, as the renderer endpoint does for an absent optional.
  arc::TabColorFilter::CreateForWebContents(contents);
  arc::TabColorFilter::FromWebContents(contents)->filter =
      matrix ? std::optional(arc::MatrixOf(matrix.hslaMatrix)) : std::nullopt;
}

- (void)removeColorFilter {
  if (auto* data = [self webContents] ? arc::TabColorFilter::FromWebContents([self webContents])
                                      : nullptr) {
    data->filter.reset();
  }
}

@end
