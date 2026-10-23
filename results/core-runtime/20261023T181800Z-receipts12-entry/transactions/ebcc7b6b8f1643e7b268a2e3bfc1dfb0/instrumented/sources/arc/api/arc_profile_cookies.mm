// Copyright 2026 ArcMod. Arc's cookie values and the profile cookie-store bridge.
#import "arc/api/arc_internal.h"

#include <cmath>
#include <optional>
#include <utility>

#include "base/functional/bind.h"
#include "base/functional/callback_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "content/public/browser/storage_partition.h"
#include "net/cookies/canonical_cookie.h"
#include "net/cookies/cookie_access_result.h"
#include "net/cookies/cookie_constants.h"
#include "net/cookies/cookie_options.h"
#include "net/cookies/cookie_partition_key_collection.h"
#include "services/network/public/mojom/cookie_manager.mojom.h"

namespace {
// Original read adapter 0x43347f4 maps Chromium 0/1/2 to Arc 1/2/3,
// and all other values (including UNSPECIFIED=-1) to 0. The write adapter
// 0x42fb728 performs the inverse, defaulting unknown Arc values to -1.
NSInteger ArcSameSite(net::CookieSameSite value) {
  switch (value) {
    case net::CookieSameSite::NO_RESTRICTION: return 1;
    case net::CookieSameSite::LAX_MODE: return 2;
    case net::CookieSameSite::STRICT_MODE: return 3;
    default: return 0;
  }
}
net::CookieSameSite NativeSameSite(NSInteger value) {
  switch (value) {
    case 1: return net::CookieSameSite::NO_RESTRICTION;
    case 2: return net::CookieSameSite::LAX_MODE;
    case 3: return net::CookieSameSite::STRICT_MODE;
    default: return net::CookieSameSite::UNSPECIFIED;
  }
}
void CookiesOnMain(dispatch_block_t work) {
  if ([NSThread isMainThread]) {
    work();
  } else {
    dispatch_async(dispatch_get_main_queue(), work);
  }
}
network::mojom::CookieManager* CookieManager(Profile* profile) {
  return profile ? profile->GetDefaultStoragePartition()->GetCookieManagerForBrowserProcess() : nullptr;
}
ArcBrowserContext* BrowserCookieContext(ArcBrowser* browser) {
  BrowserWindowInterface* native = [browser browser];
  return native ? [ArcBrowserContext wrapperForProfile:native->GetProfile()] : nil;
}
}  // namespace

ARC_EXPORT @interface ArcCookie : NSObject
@property(nonatomic, copy) NSString* name;
@property(nonatomic, copy) NSString* value;
@property(nonatomic, copy) NSString* domain;
@property(nonatomic, copy) NSString* path;
@property(nonatomic) BOOL hostOnly;
@property(nonatomic) BOOL httpOnly;
@property(nonatomic) BOOL secure;
@property(nonatomic) BOOL session;
@property(nonatomic) double expirationDate;
@property(nonatomic) NSInteger sameSite;
- (instancetype)initWithNativeCookie:(const net::CanonicalCookie&)cookie;
- (std::unique_ptr<net::CanonicalCookie>)canonicalCookieForURL:(const GURL&)url;
@end

@implementation ArcCookie {
  std::string _name;
  std::string _value;
  std::string _domain;
  std::string _path;
  BOOL _hostOnly;
  BOOL _httpOnly;
  BOOL _secure;
  BOOL _session;
  double _expirationDate;
  int32_t _sameSite;
}
- (instancetype)init {
  if ((self = [super init])) {
    _hostOnly = YES;
    _session = YES;
    _sameSite = 1;
  }
  return self;
}
- (instancetype)initWithNativeCookie:(const net::CanonicalCookie&)cookie {
  if ((self = [super init])) {
    _name = cookie.Name();
    _value = cookie.Value();
    _domain = cookie.Domain();
    _path = cookie.Path();
    _hostOnly = cookie.IsHostCookie();
    _httpOnly = cookie.IsHttpOnly();
    _secure = cookie.IsSecure();
    _session = !cookie.IsPersistent();
    _expirationDate = cookie.ExpiryDate().InSecondsFSinceUnixEpoch();
    _sameSite = static_cast<int32_t>(ArcSameSite(cookie.SameSite()));
  }
  return self;
}
- (NSString*)name { return ArcNSString(_name); }
- (void)setName:(NSString*)name { _name = ArcUTF8(name); }
- (NSString*)value { return ArcNSString(_value); }
- (void)setValue:(NSString*)value { _value = ArcUTF8(value); }
- (NSString*)domain { return ArcNSString(_domain); }
- (void)setDomain:(NSString*)domain { _domain = ArcUTF8(domain); }
- (NSString*)path { return ArcNSString(_path); }
- (void)setPath:(NSString*)path { _path = ArcUTF8(path); }
- (BOOL)hostOnly { return _hostOnly; }
- (void)setHostOnly:(BOOL)value { _hostOnly = value; }
- (BOOL)httpOnly { return _httpOnly; }
- (void)setHttpOnly:(BOOL)value { _httpOnly = value; }
- (BOOL)secure { return _secure; }
- (void)setSecure:(BOOL)value { _secure = value; }
- (BOOL)session { return _session; }
- (void)setSession:(BOOL)value { _session = value; }
- (double)expirationDate { return _expirationDate; }
- (void)setExpirationDate:(double)value { _expirationDate = value; }
- (NSInteger)sameSite { return _sameSite; }
- (void)setSameSite:(NSInteger)value { _sameSite = static_cast<int32_t>(value); }
- (std::unique_ptr<net::CanonicalCookie>)canonicalCookieForURL:(const GURL&)url {
  // Arc's original write path uses the domain string and expiration value.
  // hostOnly and session are mutable DTO fields, but are not consulted by the
  // store write: the resulting cookie derives them from domain and expiry.
  base::Time expiration;
  if (_expirationDate != 0 && !std::isnan(_expirationDate)) {
    expiration = base::Time::FromSecondsSinceUnixEpoch(_expirationDate);
  }
  return net::CanonicalCookie::CreateSanitizedCookie(
      url, _name, _value, _domain, _path, base::Time(), expiration,
      base::Time(), _secure, _httpOnly, NativeSameSite(_sameSite),
      net::COOKIE_PRIORITY_MEDIUM, std::nullopt, nullptr);
}
@end

namespace {
using CookiesCompletion = void (^)(NSArray<ArcCookie*>*);

NSArray<ArcCookie*>* CookieValues(const net::CookieList& cookies) {
  NSMutableArray<ArcCookie*>* result = [NSMutableArray arrayWithCapacity:cookies.size()];
  for (const auto& cookie : cookies) {
    [result addObject:[[ArcCookie alloc] initWithNativeCookie:cookie]];
  }
  return [result copy];
}

void QueryCookies(ArcBrowserContext* context, id filter, CookiesCompletion completion) {
  network::mojom::CookieManager* manager = CookieManager(ArcProfileFrom(context));
  if (!manager) {
    if (completion) { completion(@[]); }
    return;
  }
  __weak ArcBrowserContext* weakContext = context;
  GURL url = ArcGURL(filter);
  // The original bridge treats an empty URL string like the unfiltered API;
  // a nonempty invalid URL produces an empty result immediately.
  bool has_filter = [filter isKindOfClass:[NSURL class]] ? [[filter absoluteString] length] != 0 :
      ([filter isKindOfClass:[NSString class]] && [filter length] != 0);
  if (!has_filter) {
    manager->GetAllCookies(base::BindOnce(^(const net::CookieList& cookies) {
      if (ArcProfileFrom(weakContext) && completion) { completion(CookieValues(cookies)); }
    }));
    return;
  }
  if (!url.is_valid()) {
    if (completion) { completion(@[]); }
    return;
  }
  // Original 0x42cbc2c uses inclusive HttpOnly/SameSite options, no access-time
  // update, and a disengaged optional key-set (all partition keys).
  manager->GetCookieList(url, net::CookieOptions::MakeAllInclusive(),
      net::CookiePartitionKeyCollection::ContainsAll(), base::BindOnce(
          ^(const net::CookieAccessResultList& cookies,
            const net::CookieAccessResultList& excluded) {
            if (!ArcProfileFrom(weakContext) || !completion) { return; }
            NSMutableArray<ArcCookie*>* result = [NSMutableArray arrayWithCapacity:cookies.size()];
            for (const auto& cookie : cookies) {
              [result addObject:[[ArcCookie alloc] initWithNativeCookie:cookie.cookie]];
            }
            completion([result copy]);
          }));
}
}  // namespace

@implementation ArcBrowserContext (ProfileCookies)
- (void)getAllCookies:(CookiesCompletion)completion {
  CookiesOnMain(^{ QueryCookies(self, nil, completion); });
}
- (void)getAllCookies:(NSURL*)url completion:(CookiesCompletion)completion {
  CookiesOnMain(^{ QueryCookies(self, url, completion); });
}
- (void)deleteAllCookies:(void (^)(NSUInteger))completion {
  CookiesOnMain(^{
    network::mojom::CookieManager* manager = CookieManager(ArcProfileFrom(self));
    if (!manager) {
      if (completion) { completion(0); }
      return;
    }
    __weak ArcBrowserContext* weakContext = self;
    manager->DeleteCookies(network::mojom::CookieDeletionFilter::New(),
        base::BindOnce(^(uint32_t count) {
          if (ArcProfileFrom(weakContext) && completion) { completion(count); }
        }));
  });
}
@end

@implementation ArcBrowser (ProfileCookies)
- (void)getAllCookies:(CookiesCompletion)completion {
  CookiesOnMain(^{ QueryCookies(BrowserCookieContext(self), nil, completion); });
}
- (void)getAllCookies:(NSURL*)url completion:(CookiesCompletion)completion {
  CookiesOnMain(^{ QueryCookies(BrowserCookieContext(self), url, completion); });
}
- (void)setCookie:(NSURL*)value withCookie:(ArcCookie*)cookie {
  CookiesOnMain(^{
    if (![cookie isKindOfClass:[ArcCookie class]]) { return; }
    ArcBrowserContext* context = BrowserCookieContext(self);
    network::mojom::CookieManager* manager = CookieManager(ArcProfileFrom(context));
    GURL url = ArcGURL(value);
    if (!manager || !url.is_valid()) { return; }
    auto native = [cookie canonicalCookieForURL:url];
    if (native) {
      manager->SetCanonicalCookie(*native, url, net::CookieOptions::MakeAllInclusive(), base::DoNothing());
    }
  });
}
- (void)removeCookie:(NSURL*)value withName:(NSString*)name {
  CookiesOnMain(^{
    network::mojom::CookieManager* manager = CookieManager(ArcProfileFrom(BrowserCookieContext(self)));
    GURL url = ArcGURL(value);
    if (!manager || !url.is_valid()) { return; }
    // Original filter sets both URL and cookie_name, including an empty name.
    auto filter = network::mojom::CookieDeletionFilter::New();
    filter->url = url;
    filter->cookie_name = ArcUTF8(name);
    manager->DeleteCookies(std::move(filter), base::DoNothing());
  });
}
- (void)saveCookiesToDisk {
  CookiesOnMain(^{
    if (auto* manager = CookieManager(ArcProfileFrom(BrowserCookieContext(self)))) {
      manager->FlushCookieStore(base::DoNothing());
    }
  });
}
@end
