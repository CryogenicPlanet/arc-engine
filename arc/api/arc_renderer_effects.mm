// Copyright 2026 ArcMod. Profile-owned Effects service. Zoom uses Chromium's
// host zoom map, matching the reviewed Arc bridge (0x4436dec / 0x4436ec0).
#import "arc/api/arc_internal.h"

#include <cmath>

#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/host_zoom_map.h"

ARC_EXPORT @interface ArcBrowserContextEffects : NSObject
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
@property(nonatomic, weak) ArcBrowserContext* owner;
@end

@implementation ArcBrowserContextEffects
@synthesize owner = _owner;
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context {
  if ((self = [super init])) {
    _owner = context;
  }
  return self;
}
- (void)registerZoomFactor:(float)factor forHostPattern:(NSString*)hostPattern {
  Profile* profile = [_owner profile];
  if (!profile) {
    return;
  }
  auto* zoom = content::HostZoomMap::GetDefaultForBrowserContext(profile);
  if (zoom) {
    zoom->SetZoomLevelForHost(ArcUTF8(hostPattern), std::log(static_cast<double>(factor)) /
                                                       0.1823215567939546);
  }
}
- (void)unregisterZoomFactor:(NSString*)hostPattern {
  Profile* profile = [_owner profile];
  if (!profile) {
    return;
  }
  auto* zoom = content::HostZoomMap::GetDefaultForBrowserContext(profile);
  if (zoom) {
    zoom->SetZoomLevelForHost(ArcUTF8(hostPattern), zoom->GetDefaultZoomLevel());
  }
}
@end
