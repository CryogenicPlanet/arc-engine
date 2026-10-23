// Copyright 2026 ArcMod. Native navigation commands and immutable boundary values.
#import "arc/browser/arc_navigation.h"

#include <utility>

#include "base/memory/weak_ptr.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/reload_type.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "net/base/net_errors.h"
#include "net/http/http_response_headers.h"
#include "ui/base/page_transition_types.h"
#include "url/origin.h"

@interface ArcNavigation ()
- (instancetype)initWithNavigationHandle:(content::NavigationHandle*)handle;
- (ArcNavigation*)eventSnapshot;
@end

@implementation ArcNavigation {
  NSString* _url;
  NSDictionary* _responseHeaders;
  NSInteger _responseStatusCode;
  int64_t _navigationIdentifier;
}
+ (instancetype)snapshotForNavigationHandle:(content::NavigationHandle*)handle {
  return handle ? [[self alloc] initWithNavigationHandle:handle] : nil;
}
- (instancetype)initWithNavigationHandle:(content::NavigationHandle*)handle {
  if ((self = [super init])) {
    _url = ArcNSString(handle->GetURL().spec());
    _navigationIdentifier = handle->GetNavigationId();
    const net::HttpResponseHeaders* headers = handle->GetResponseHeaders();
    if (headers) {
      _responseStatusCode = headers->response_code();
      NSMutableDictionary* snapshot = [NSMutableDictionary dictionary];
      size_t iterator = 0;
      std::string name;
      std::string value;
      while (headers->EnumerateHeaderLines(&iterator, &name, &value)) {
        snapshot[ArcNSString(name)] = ArcNSString(value);
      }
      if (snapshot.count) {
        _responseHeaders = [snapshot copy];
      }
    }
  }
  return self;
}
- (ArcNavigation*)eventSnapshot {
  ArcNavigation* snapshot = [[ArcNavigation alloc] init];
  snapshot->_url = _url;
  snapshot->_responseHeaders = _responseHeaders;
  snapshot->_responseStatusCode = _responseStatusCode;
  snapshot->_navigationIdentifier = _navigationIdentifier;
  return snapshot;
}
- (NSString*)url { return _url; }
- (BOOL)didRedirectToOtherTab { return NO; }
- (NSInteger)responseStatusCode { return _responseStatusCode; }
- (NSDictionary*)responseHeaders { return _responseHeaders; }
- (NSUInteger)hash { return static_cast<NSUInteger>(_navigationIdentifier); }
- (BOOL)isEqual:(id)other {
  return self == other || ([other isKindOfClass:[ArcNavigation class]] &&
      _navigationIdentifier != 0 &&
      _navigationIdentifier == ((ArcNavigation*)other)->_navigationIdentifier);
}
@end

namespace {
struct HistoryEntrySnapshot {
  base::WeakPtr<content::WebContents> contents;
  int entry_id = 0;
  std::u16string title;
  GURL url;
  bool valid = false;
};
}  // namespace

@interface ArcBackForwardListItem ()
- (int)indexInWebContents:(content::WebContents*)contents;
@end

@implementation ArcBackForwardListItem {
  base::WeakPtr<content::WebContents> _contents;
  int _entryIdentifier;
  NSString* _title;
  NSURL* _url;
  BOOL _isValid;
}
- (instancetype)initWithBackForwardListItem:(const void*)data {
  if ((self = [super init])) {
    const auto& snapshot = *static_cast<const HistoryEntrySnapshot*>(data);
    _contents = snapshot.contents;
    _entryIdentifier = snapshot.entry_id;
    _title = ArcNSString(snapshot.title);
    _url = ArcNSURL(snapshot.url);
    _isValid = snapshot.valid;
  }
  return self;
}
- (NSString*)title { return _title; }
- (NSURL*)url { return _url; }
- (BOOL)isValid { return _isValid; }
- (int)indexInWebContents:(content::WebContents*)contents {
  if (!contents || _contents.get() != contents || !_isValid) {
    return -1;
  }
  auto& controller = contents->GetController();
  for (int index = 0; index < controller.GetEntryCount(); ++index) {
    if (controller.GetEntryAtIndex(index)->GetUniqueID() == _entryIdentifier) {
      return index;
    }
  }
  return -1;
}
@end

namespace {
ArcBackForwardListItem* SnapshotHistoryItem(content::WebContents* contents, int index) {
  HistoryEntrySnapshot snapshot;
  if (contents) {
    snapshot.contents = contents->GetWeakPtr();
    auto& controller = contents->GetController();
    if (index >= 0 && index < controller.GetEntryCount()) {
      content::NavigationEntry* entry = controller.GetEntryAtIndex(index);
      snapshot.entry_id = entry->GetUniqueID();
      snapshot.title = entry->GetTitle();
      snapshot.url = entry->GetVirtualURL();
      snapshot.valid = true;
    }
  }
  return [[ArcBackForwardListItem alloc] initWithBackForwardListItem:&snapshot];
}
}  // namespace

@implementation ArcBackForwardList {
  ArcBackForwardListItem* _backItem;
  ArcBackForwardListItem* _currentItem;
  ArcBackForwardListItem* _forwardItem;
  NSArray<ArcBackForwardListItem*>* _backList;
  NSArray<ArcBackForwardListItem*>* _forwardList;
}
+ (instancetype)snapshotForWebContents:(content::WebContents*)contents {
  return [[self alloc] initWithBackForwardList:contents];
}
- (instancetype)initWithBackForwardList:(const void*)contents {
  if ((self = [super init])) {
    auto* native = static_cast<content::WebContents*>(const_cast<void*>(contents));
    const int current = native ? native->GetController().GetCurrentEntryIndex() : -1;
    const int count = native ? native->GetController().GetEntryCount() : 0;
    NSMutableArray<ArcBackForwardListItem*>* back = [NSMutableArray array];
    NSMutableArray<ArcBackForwardListItem*>* forward = [NSMutableArray array];
    for (int index = 0; index < current; ++index) {
      [back addObject:SnapshotHistoryItem(native, index)];
    }
    for (int index = current + 1; index < count; ++index) {
      [forward addObject:SnapshotHistoryItem(native, index)];
    }
    _backList = [back copy];
    _forwardList = [forward copy];
    _backItem = back.lastObject ?: SnapshotHistoryItem(native, -1);
    _currentItem = SnapshotHistoryItem(native, current);
    _forwardItem = forward.firstObject ?: SnapshotHistoryItem(native, -1);
  }
  return self;
}
- (ArcBackForwardListItem*)backItem { return _backItem; }
- (ArcBackForwardListItem*)currentItem { return _currentItem; }
- (ArcBackForwardListItem*)forwardItem { return _forwardItem; }
- (NSArray*)backList { return _backList; }
- (NSArray*)forwardList { return _forwardList; }
@end

@protocol ArcCoreNavigationObserver <NSObject>
- (void)webContents:(ArcWebContents*)contents didStartProvisionalNavigation:(ArcNavigation*)navigation;
- (void)webContents:(ArcWebContents*)contents didCommitNavigation:(ArcNavigation*)navigation;
- (void)webContents:(ArcWebContents*)contents didFinishNavigation:(ArcNavigation*)navigation;
- (void)webContents:(ArcWebContents*)contents didReceiveServerRedirectForProvisionalNavigation:(ArcNavigation*)navigation;
- (void)webContents:(ArcWebContents*)contents didFailNavigation:(ArcNavigation*)navigation;
- (void)webContents:(ArcWebContents*)contents didFailProvisionalNavigation:(ArcNavigation*)navigation withError:(NSError*)error;
- (void)webContents:(ArcWebContents*)contents didStartInPageNavigation:(ArcNavigation*)navigation;
- (void)webContents:(ArcWebContents*)contents didFinishInPageNavigation:(ArcNavigation*)navigation;
@end

namespace {
ArcNavigation* SnapshotPrimaryNavigation(
    const content::NavigationController::WeakNavigationHandleVector& handles) {
  for (const auto& handle : handles) {
    if (handle && handle->IsInPrimaryMainFrame()) {
      return [ArcNavigation snapshotForNavigationHandle:handle.get()];
    }
  }
  return nil;
}

// Reload returns void in Chromium. Only return a navigation when an actual
// primary-frame navigation starts during that native command.
class ReloadNavigationCapture : public content::WebContentsObserver {
 public:
  explicit ReloadNavigationCapture(content::WebContents* contents)
      : content::WebContentsObserver(contents) {}
  void DidStartNavigation(content::NavigationHandle* handle) override {
    if (!captured_ && handle->IsInPrimaryMainFrame()) {
      captured_ = true;
      navigation_identifier_ = handle->GetNavigationId();
      snapshot = [ArcNavigation snapshotForNavigationHandle:handle];
    }
  }
  void DidFinishNavigation(content::NavigationHandle* handle) override {
    if (captured_ && handle->GetNavigationId() == navigation_identifier_ &&
        (!handle->HasCommitted() || handle->GetNetErrorCode() != net::OK)) {
      snapshot = nil;
    }
  }
  ArcNavigation* __strong snapshot = nil;
 private:
  bool captured_ = false;
  int64_t navigation_identifier_ = 0;
};
}  // namespace

@interface ArcWebContentsNavigationController ()
- (void)deliverSelector:(SEL)selector navigation:(ArcNavigation*)navigation error:(NSError*)error;
- (ArcNavigation*)reloadWithType:(content::ReloadType)type;
@end

@implementation ArcWebContentsNavigationController {
  NSHashTable<id>* _observers;
  NSPointerArray* _observerOrder;
  ArcNavigation* _documentNavigation;
}
@synthesize owner = _owner;
- (void)setOwner:(ArcWebContents*)owner {
  if (_owner != owner) {
    _documentNavigation = nil;
    _owner = owner;
  }
}
- (instancetype)initForWebContents:(ArcWebContents*)owner {
  if ((self = [super init])) {
    _owner = owner;
    _observers = [NSHashTable hashTableWithOptions:
        NSPointerFunctionsWeakMemory | NSPointerFunctionsObjectPointerPersonality];
    _observerOrder = [NSPointerArray weakObjectsPointerArray];
  }
  return self;
}
- (BOOL)isEqual:(id)other { return self == other; }
- (NSUInteger)hash { return [super hash]; }
- (void)addObserver:(id)observer {
  if (observer && ![_observers member:observer]) {
    [_observers addObject:observer];
    [_observerOrder addPointer:(__bridge void*)observer];
  }
}
- (void)removeObserver:(id)observer {
  if (!observer) {
    return;
  }
  [_observers removeObject:observer];
  for (NSUInteger index = _observerOrder.count; index > 0; --index) {
    if ([_observerOrder pointerAtIndex:index - 1] == (__bridge void*)observer) {
      [_observerOrder removePointerAtIndex:index - 1];
    }
  }
  [_observerOrder compact];
}
- (void)deliverSelector:(SEL)selector navigation:(ArcNavigation*)navigation error:(NSError*)error {
  ArcWebContents* owner = _owner;
  if (!owner || ![owner webContents] || !navigation) {
    return;
  }
  for (id<ArcCoreNavigationObserver> observer in _observerOrder.allObjects) {
    if (_owner != owner || ![owner webContents]) {
      return;
    }
    if (![_observers member:observer] || ![observer respondsToSelector:selector]) {
      continue;
    }
    if (selector == @selector(webContents:didStartProvisionalNavigation:)) {
      [observer webContents:owner didStartProvisionalNavigation:navigation];
    } else if (selector == @selector(webContents:didCommitNavigation:)) {
      [observer webContents:owner didCommitNavigation:navigation];
    } else if (selector == @selector(webContents:didFinishNavigation:)) {
      [observer webContents:owner didFinishNavigation:navigation];
    } else if (selector == @selector(webContents:didReceiveServerRedirectForProvisionalNavigation:)) {
      [observer webContents:owner didReceiveServerRedirectForProvisionalNavigation:navigation];
    } else if (selector == @selector(webContents:didFailNavigation:)) {
      [observer webContents:owner didFailNavigation:navigation];
    } else if (selector == @selector(webContents:didFailProvisionalNavigation:withError:)) {
      [observer webContents:owner didFailProvisionalNavigation:navigation withError:error];
    } else if (selector == @selector(webContents:didStartInPageNavigation:)) {
      [observer webContents:owner didStartInPageNavigation:navigation];
    } else if (selector == @selector(webContents:didFinishInPageNavigation:)) {
      [observer webContents:owner didFinishInPageNavigation:navigation];
    }
  }
}
- (void)didStartNavigation:(content::NavigationHandle*)handle {
  if (!handle || !handle->IsInPrimaryMainFrame() || handle->GetWebContents() != [_owner webContents]) {
    return;
  }
  [self didStartNavigationSnapshot:[ArcNavigation snapshotForNavigationHandle:handle]
                           inPage:handle->IsSameDocument()];
}
- (void)didStartNavigationSnapshot:(ArcNavigation*)navigation inPage:(BOOL)inPage {
  if (!inPage) {
    _documentNavigation = nil;
  }
  SEL selector = inPage ? @selector(webContents:didStartInPageNavigation:)
                        : @selector(webContents:didStartProvisionalNavigation:);
  [self deliverSelector:selector navigation:navigation error:nil];
}
- (void)didRedirectNavigation:(content::NavigationHandle*)handle {
  if (handle && handle->IsInPrimaryMainFrame() && handle->GetWebContents() == [_owner webContents]) {
    [self deliverSelector:@selector(webContents:didReceiveServerRedirectForProvisionalNavigation:)
              navigation:[ArcNavigation snapshotForNavigationHandle:handle] error:nil];
  }
}
- (void)didFinishNavigation:(content::NavigationHandle*)handle {
  if (!handle || !handle->IsInPrimaryMainFrame() || handle->GetWebContents() != [_owner webContents]) {
    return;
  }
  [self didFinishNavigationSnapshot:[ArcNavigation snapshotForNavigationHandle:handle]
                            inPage:handle->IsSameDocument() committed:handle->HasCommitted()
                             error:handle->GetNetErrorCode() restored:handle->IsServedFromBackForwardCache()];
}
- (void)didFinishNavigationSnapshot:(ArcNavigation*)navigation inPage:(BOOL)inPage
                        committed:(BOOL)committed error:(NSInteger)error restored:(BOOL)restored {
  if (error != net::OK) {
    _documentNavigation = nil;
    if (committed) {
      [self deliverSelector:@selector(webContents:didFailNavigation:) navigation:navigation error:nil];
    } else {
      NSError* failure = [NSError errorWithDomain:@"company.thebrowser.Arc"
          code:error userInfo:@{NSURLErrorKey: [NSURL URLWithString:navigation.url] ?: [NSNull null]}];
      [self deliverSelector:@selector(webContents:didFailProvisionalNavigation:withError:)
                navigation:navigation error:failure];
    }
    return;
  }
  if (!committed) {
    return;
  }
  if (inPage) {
    [self deliverSelector:@selector(webContents:didFinishInPageNavigation:) navigation:navigation error:nil];
  } else {
    _documentNavigation = navigation;
    [self deliverSelector:@selector(webContents:didCommitNavigation:) navigation:navigation error:nil];
    if (restored && _documentNavigation == navigation) {
      [self didFinishDocumentLoad];
    }
  }
}
- (void)didFinishDocumentLoad {
  ArcNavigation* navigation = [_documentNavigation eventSnapshot];
  _documentNavigation = nil;
  [self deliverSelector:@selector(webContents:didFinishNavigation:) navigation:navigation error:nil];
}
- (ArcNavigation*)load:(id)url initiatorOrigin:(id)origin {
  return [self load:url initiatorOrigin:origin transition:1 fromAddressBar:YES typedWithHttpScheme:NO];
}
- (ArcNavigation*)load:(id)url initiatorOrigin:(id)origin transition:(NSInteger)transition
       fromAddressBar:(BOOL)fromAddressBar typedWithHttpScheme:(BOOL)typedWithHttpScheme {
  ArcWebContents* owner = _owner;
  content::WebContents* contents = [owner webContents];
  const GURL destination = ArcGURL(url);
  if (!contents || !destination.is_valid()) {
    return nil;
  }
  content::NavigationController::LoadURLParams params(destination);
  int32_t native_transition = transition >= 1 && transition <= 10 ? static_cast<int32_t>(transition) : 0;
  if (fromAddressBar) {
    native_transition |= ui::PAGE_TRANSITION_FROM_ADDRESS_BAR;
  }
  params.transition_type = ui::PageTransitionFromInt(native_transition);
  if (origin) {
    const GURL initiator = ArcGURL(origin);
    if (!initiator.is_valid()) {
      return nil;
    }
    params.initiator_origin = url::Origin::Create(initiator);
    if (params.initiator_origin->opaque()) {
      return nil;
    }
  }
  // Chromium's content controller has no typed-with-http-scheme field. The
  // supplied URL is already fully specified; preserve it without URL fixup.
  auto navigation = contents->GetController().LoadURLWithParams(params);
  return navigation ? [ArcNavigation snapshotForNavigationHandle:navigation.get()] : nil;
}
- (ArcNavigation*)goBack {
  ArcWebContents* owner = _owner;
  content::WebContents* contents = [owner webContents];
  return contents && contents->GetController().CanGoBack()
      ? SnapshotPrimaryNavigation(contents->GetController().GoBack()) : nil;
}
- (ArcNavigation*)goForward {
  ArcWebContents* owner = _owner;
  content::WebContents* contents = [owner webContents];
  return contents && contents->GetController().CanGoForward()
      ? SnapshotPrimaryNavigation(contents->GetController().GoForward()) : nil;
}
- (ArcNavigation*)goToBackForwardListItem:(ArcBackForwardListItem*)item {
  ArcWebContents* owner = _owner;
  content::WebContents* contents = [owner webContents];
  if (![item isKindOfClass:[ArcBackForwardListItem class]]) {
    return nil;
  }
  const int index = [item indexInWebContents:contents];
  return index >= 0 ? SnapshotPrimaryNavigation(contents->GetController().GoToIndex(index)) : nil;
}
- (ArcNavigation*)reloadWithType:(content::ReloadType)type {
  ArcWebContents* owner = _owner;
  content::WebContents* contents = [owner webContents];
  if (!contents) {
    return nil;
  }
  ReloadNavigationCapture capture(contents);
  contents->GetController().Reload(type, true);
  return capture.snapshot;
}
- (ArcNavigation*)reload { return [self reloadWithType:content::ReloadType::NORMAL]; }
- (ArcNavigation*)reloadWithoutCache { return [self reloadWithType:content::ReloadType::BYPASSING_CACHE]; }
@end
