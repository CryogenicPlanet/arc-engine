// Copyright 2026 ArcMod. Profile history delegate registration.
#import "arc/api/arc_internal.h"

ARC_EXPORT @interface ArcBrowserContextHistory : NSObject
@property(nonatomic, weak) ArcBrowserContext* owner;
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
@end

@implementation ArcBrowserContextHistory {
  __weak ArcBrowserContext* _owner;
  __weak id _delegate;
}
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context {
  if ((self = [super init])) { _owner = context; }
  return self;
}
- (instancetype)initWithBrowserContextHistory:(const void*)unused
                           forBrowserContext:(ArcBrowserContext*)context {
  return [self initWithArcBrowserContext:context];
}
- (ArcBrowserContext*)owner { return _owner; }
- (void)setOwner:(ArcBrowserContext*)owner { _owner = owner; }
- (void)setDelegate:(id)delegate {
  (void)[self owner];
  if (delegate) { _delegate = delegate; }
}
@end
