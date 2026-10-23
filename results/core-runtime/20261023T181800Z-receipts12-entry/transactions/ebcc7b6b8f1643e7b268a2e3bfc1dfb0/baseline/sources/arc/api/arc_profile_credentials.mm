// Copyright 2026 ArcMod. Immutable credentials supplied by native password forms.
#import "arc/api/arc_internal.h"

#include <string_view>

ARC_EXPORT @interface ArcLoginCredentials : NSObject {
 @package
  NSURL* _url;
  NSString* _username;
  NSString* _password;
}
@property(nonatomic, readonly) NSURL* url;
@property(nonatomic, readonly, copy) NSString* username;
@property(nonatomic, readonly, copy) NSString* password;
@end

@implementation ArcLoginCredentials
- (NSURL*)url { return _url; }
- (NSString*)username { return _username; }
- (NSString*)password { return _password; }
@end

// The UI package supplies these values from the actual observed PasswordForm;
// this DTO performs no credential lookup, persistence, or diagnostic logging.
id ArcMakeLoginCredentials(const GURL& url, std::u16string_view username,
                           std::u16string_view password) {
  ArcLoginCredentials* credentials = [[ArcLoginCredentials alloc] init];
  credentials->_url = ArcNSURL(url);
  credentials->_username = base::SysUTF16ToNSString(username);
  credentials->_password = base::SysUTF16ToNSString(password);
  return credentials;
}
