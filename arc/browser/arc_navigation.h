// Copyright 2026 ArcMod. Navigation snapshots and the app observer bridge.
#ifndef ARC_BROWSER_ARC_NAVIGATION_H_
#define ARC_BROWSER_ARC_NAVIGATION_H_

#import "arc/api/arc_internal.h"

namespace content {
class NavigationHandle;
}

ARC_EXPORT @interface ArcNavigationAction : NSObject
- (instancetype)initWithNavigationAction:(const void*)snapshot;
@property(nonatomic, readonly) NSURL* url;
@property(nonatomic, readonly) NSURL* referrer;
@property(nonatomic, readonly) NSUInteger navigationType;
@property(nonatomic, readonly) NSInteger navigationDisposition;
@end

namespace arc {
// Captures the synchronous policy payload. The disposition is an Arc boundary
// ordinal (the engine's supported range is 0 through 11).
ArcNavigationAction* CreateNavigationAction(content::NavigationHandle* handle,
                                          NSInteger navigation_disposition,
                                          NSUInteger navigation_type = 0);
}

ARC_EXPORT @interface ArcNavigation : NSObject
+ (instancetype)snapshotForNavigationHandle:(content::NavigationHandle*)handle __attribute__((objc_direct));
@property(nonatomic, readonly) NSString* url;
@property(nonatomic, readonly) BOOL didRedirectToOtherTab;
@property(nonatomic, readonly) NSInteger responseStatusCode;
@property(nonatomic, readonly) NSDictionary* responseHeaders;
@end

ARC_EXPORT @interface ArcBackForwardListItem : NSObject
- (instancetype)initWithBackForwardListItem:(const void*)snapshot;
@property(nonatomic, readonly) NSString* title;
@property(nonatomic, readonly) NSURL* url;
@property(nonatomic, readonly) BOOL isValid;
@end

ARC_EXPORT @interface ArcBackForwardList : NSObject
+ (instancetype)snapshotForWebContents:(content::WebContents*)contents __attribute__((objc_direct));
- (instancetype)initWithBackForwardList:(const void*)contents;
@property(nonatomic, readonly) ArcBackForwardListItem* backItem;
@property(nonatomic, readonly) ArcBackForwardListItem* currentItem;
@property(nonatomic, readonly) ArcBackForwardListItem* forwardItem;
@property(nonatomic, readonly) NSArray<ArcBackForwardListItem*>* backList;
@property(nonatomic, readonly) NSArray<ArcBackForwardListItem*>* forwardList;
@end

ARC_EXPORT @interface ArcWebContentsNavigationController : NSObject
- (instancetype)initForWebContents:(ArcWebContents*)owner __attribute__((objc_direct));
@property(nonatomic, weak) ArcWebContents* owner;
- (void)addObserver:(id)observer;
- (void)removeObserver:(id)observer;
- (ArcNavigation*)load:(id)url initiatorOrigin:(id)origin;
- (ArcNavigation*)load:(id)url initiatorOrigin:(id)origin transition:(NSInteger)transition
       fromAddressBar:(BOOL)fromAddressBar typedWithHttpScheme:(BOOL)typedWithHttpScheme;
- (ArcNavigation*)goBack;
- (ArcNavigation*)goForward;
- (ArcNavigation*)goToBackForwardListItem:(ArcBackForwardListItem*)item;
- (ArcNavigation*)reload;
- (ArcNavigation*)reloadWithoutCache;

// Called by ArcWebContents' native observer, on the UI thread. Handle arguments
// are inspected synchronously and never retained. Only primary-frame events
// are delivered. Document-load completion is distinct from navigation commit.
- (void)didStartNavigation:(content::NavigationHandle*)handle __attribute__((objc_direct));
- (void)didRedirectNavigation:(content::NavigationHandle*)handle __attribute__((objc_direct));
- (void)didFinishNavigation:(content::NavigationHandle*)handle __attribute__((objc_direct));
- (void)didFinishDocumentLoad __attribute__((objc_direct));
// Snapshotted variants allow the WebContents bridge to publish KVO state first
// without retaining a NavigationHandle across arbitrary app callbacks.
- (void)didStartNavigationSnapshot:(ArcNavigation*)navigation inPage:(BOOL)inPage __attribute__((objc_direct));
- (void)didFinishNavigationSnapshot:(ArcNavigation*)navigation inPage:(BOOL)inPage
                        committed:(BOOL)committed error:(NSInteger)error restored:(BOOL)restored __attribute__((objc_direct));
@end

#endif  // ARC_BROWSER_ARC_NAVIGATION_H_
