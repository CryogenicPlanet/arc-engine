// Copyright 2026 ArcMod. Shared internal declarations for the ArcCore layer. See arc/ARCHITECTURE.md.

#ifndef ARC_API_ARC_INTERNAL_H_
#define ARC_API_ARC_INTERNAL_H_

#import <AppKit/AppKit.h>

#include <string>

#include "base/strings/sys_string_conversions.h"
#include "base/time/time.h"
#include "third_party/skia/include/core/SkColor.h"
#include "url/gurl.h"

#define ARC_EXPORT __attribute__((visibility("default")))

class Profile;
namespace content {
class WebContents;
}

// ---- Conversions --------------------------------------------------------------------------------

inline NSString* ArcNSString(std::string_view s) {
  return base::SysUTF8ToNSString(std::string(s));
}
inline NSString* ArcNSString(std::u16string_view s) {
  return base::SysUTF16ToNSString(std::u16string(s));
}
inline std::string ArcUTF8(NSString* s) {
  return s ? base::SysNSStringToUTF8(s) : std::string();
}
inline std::u16string ArcUTF16(NSString* s) {
  return s ? base::SysNSStringToUTF16(s) : std::u16string();
}
// URLs cross the boundary as NSString or NSURL depending on the method; check the type encoding/metadata.
inline NSURL* ArcNSURL(const GURL& url) {
  return url.is_valid() ? [NSURL URLWithString:ArcNSString(url.spec())] : nil;
}
inline GURL ArcGURL(id url) {
  if ([url isKindOfClass:[NSURL class]]) {
    return GURL(ArcUTF8([url absoluteString]));
  }
  return [url isKindOfClass:[NSString class]] ? GURL(ArcUTF8(url)) : GURL();
}
inline NSDate* ArcNSDate(base::Time t) {
  return t.is_null() ? nil : [NSDate dateWithTimeIntervalSince1970:t.InSecondsFSinceUnixEpoch()];
}
inline base::Time ArcTime(NSDate* d) {
  return d ? base::Time::FromSecondsSinceUnixEpoch(d.timeIntervalSince1970) : base::Time();
}

// ---- ArcColor (arc_browser_context.mm) ------------------------------------------------------------

ARC_EXPORT @interface ArcColor : NSObject
- (instancetype)initWithR:(uint8_t)r g:(uint8_t)g b:(uint8_t)b a:(uint8_t)a;
+ (instancetype)colorWithSkColor:(SkColor)color;
- (SkColor)skColor;
@end

// ---- Profiles (arc_browser_context.mm) ------------------------------------------------------------

ARC_EXPORT @interface ArcBrowserContextID : NSObject
- (instancetype)initWithDirectoryBasename:(NSString*)basename isIncognito:(BOOL)isIncognito;
@property(nonatomic, readonly, copy) NSString* directoryBasename;
@property(nonatomic, readonly) BOOL isIncognito;
@end

ARC_EXPORT @interface ArcBrowserContext : NSObject
// The one wrapper for |profile| (created on first use, owned by the profile's user data).
+ (instancetype)wrapperForProfile:(Profile*)profile;
// nullptr once the profile is gone.
- (Profile*)profile;
@property(nonatomic, readonly) ArcBrowserContextID* id;
// Per-profile service wrapper of |className| (preferences, history, ...), created once.
- (id)serviceNamed:(NSString*)className;
@end

// Services returned by -serviceNamed: that implement -initWithArcBrowserContext:(ArcBrowserContext*) are created
// with it (the wrapper is held weakly by convention; the context owns the service).

// Profile of an ArcBrowserContext argument, or nullptr.
inline Profile* ArcProfileFrom(id context) {
  return [context isKindOfClass:[ArcBrowserContext class]] ? [(ArcBrowserContext*)context profile] : nullptr;
}

// ---- core (arc_browser.mm) -----------------------------------------------------------------------

class BrowserWindowInterface;
ARC_EXPORT @interface ArcSessionID : NSObject
- (instancetype)initWithID:(NSInteger)identifier;
@property(nonatomic, readonly) NSInteger identifier;
@end

ARC_EXPORT @interface ArcBrowser : NSObject
+ (instancetype)wrapperForBrowser:(BrowserWindowInterface*)browser;
- (BrowserWindowInterface*)browser;
- (void)setDelegate:(id)delegate;
- (void)setWindow:(NSWindow*)window;
@property(nonatomic, copy) NSString* windowIdentifier;
@end

// ---- app package: URL loading (implemented in arc/api/arc_app_*.mm) ------------------------------

@class ArcURLLoadConfiguration;
@class ArcURLLoadCancellationToken;
// Starts a URL load like -[ArcBrowserApplication loadUrl:withDelegate:], using |profile|'s network context
// (nullptr: the system network context, as the application-level loader does). Delegate callbacks follow
// ArcURLLoadDelegate. Returns the cancellation token the API returns.
ArcURLLoadCancellationToken* ArcStartURLLoad(Profile* profile, ArcURLLoadConfiguration* configuration, id delegate);

#endif  // ARC_API_ARC_INTERNAL_H_
