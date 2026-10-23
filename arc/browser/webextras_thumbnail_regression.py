#!/usr/bin/env python3
"""Test production ordered/weak delivery with GCD and nestable FIFO adapters.

Chromium UI queue integration is verified by production compile checks only.
"""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
source = (root / "arc/api/arc_webextras_capture.mm").read_text()
start = source.index("// BEGIN WEBEXTRAS_THUMBNAIL_DELIVERY")
end = source.index("// END WEBEXTRAS_THUMBNAIL_DELIVERY", start)
helper = source[start:end]
checks = r'''
static void Add(NSMutableArray* registrations, id observer, id owner) {
  WebextrasThumbnailRegistration* registration = [WebextrasThumbnailRegistration new];
  registration.observer = observer;
  registration.owner = owner;
  [registrations addObject:registration];
}
static void DrainUI(void) {
  __block BOOL complete = NO;
  dispatch_async(dispatch_get_main_queue(), ^{ complete = YES; });
  while (!complete) CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.01, true);
}
static void Expect(NSArray* actual, NSArray* expected) {
  NSCAssert([actual isEqual:expected], @"delivery mismatch: %@ != %@", actual, expected);
}
int main(void) {
  @autoreleasepool {
    // Callbacks are asynchronous and preserve registration order.
    {
      NSMutableArray* registrations = [NSMutableArray array];
      for (NSString* observer in @[@"A", @"B", @"C"]) Add(registrations, observer, nil);
      NSMutableArray* delivered = [NSMutableArray array];
      PostThumbnailNotifications(registrations, ^(id observer __attribute__((unused)), id owner __attribute__((unused))) { [delivered addObject:observer]; });
      Expect(delivered, @[]);
      DrainUI(); Expect(delivered, (@[@"A", @"B", @"C"]));
    }
    // A removes B after all tasks were queued. C still receives its callback.
    {
      NSMutableArray* registrations = [NSMutableArray array];
      for (NSString* observer in @[@"A", @"B", @"C"]) Add(registrations, observer, nil);
      NSMutableArray* delivered = [NSMutableArray array];
      PostThumbnailNotifications(registrations, ^(id observer __attribute__((unused)), id owner __attribute__((unused))) {
        [delivered addObject:observer];
        if ([observer isEqual:@"A"]) [registrations removeObjectAtIndex:1];
      });
      DrainUI(); Expect(delivered, (@[@"A", @"C"]));
    }
    // C added during A's callback belongs to the next emission, not this batch.
    {
      NSMutableArray* registrations = [NSMutableArray array];
      Add(registrations, @"A", nil); Add(registrations, @"B", nil);
      NSMutableArray* delivered = [NSMutableArray array];
      __block BOOL added = NO;
      void (^delivery)(id, id) = ^(id observer __attribute__((unused)), id owner __attribute__((unused))) {
        [delivered addObject:observer];
        if (!added && [observer isEqual:@"A"]) { added = YES; Add(registrations, @"C", nil); }
      };
      PostThumbnailNotifications(registrations, delivery);
      DrainUI(); Expect(delivered, (@[@"A", @"B"]));
      [delivered removeAllObjects];
      PostThumbnailNotifications(registrations, delivery);
      DrainUI(); Expect(delivered, (@[@"A", @"B", @"C"]));
    }
    // Queuing a notification must not keep the weak observer alive.
    {
      NSMutableArray* registrations = [NSMutableArray array];
      __weak NSObject* expired;
      __block NSUInteger callbacks = 0;
      @autoreleasepool {
        NSObject* observer = [NSObject new]; expired = observer;
        Add(registrations, observer, nil);
        PostThumbnailNotifications(registrations, ^(id value __attribute__((unused)), id owner __attribute__((unused))) { ++callbacks; });
        observer = nil;
      }
      NSCAssert(!expired, @"queued notification retained the observer");
      DrainUI(); NSCAssert(callbacks == 0, @"expired observer received a callback");
    }
    // Remove/re-add creates a new registration; the old queued B task stays cancelled.
    {
      NSMutableArray* registrations = [NSMutableArray array];
      Add(registrations, @"A", nil); Add(registrations, @"B", nil);
      NSMutableArray* delivered = [NSMutableArray array];
      __block BOOL replaced = NO;
      void (^delivery)(id, id) = ^(id observer __attribute__((unused)), id owner __attribute__((unused))) {
        [delivered addObject:observer];
        if (!replaced && [observer isEqual:@"A"]) {
          replaced = YES; [registrations removeObjectAtIndex:1]; Add(registrations, @"B", nil);
        }
      };
      PostThumbnailNotifications(registrations, delivery);
      DrainUI(); Expect(delivered, (@[@"A"]));
      [delivered removeAllObjects];
      PostThumbnailNotifications(registrations, delivery);
      DrainUI(); Expect(delivered, (@[@"A", @"B"]));
    }
    // The captured owner is weak too, and may become nil before delivery.
    {
      NSMutableArray* registrations = [NSMutableArray array];
      __weak NSObject* expired;
      __block NSUInteger callbacks = 0;
      @autoreleasepool {
        NSObject* owner = [NSObject new]; expired = owner;
        Add(registrations, @"A", owner);
        PostThumbnailNotifications(registrations, ^(id observer __attribute__((unused)), id value) {
          NSCAssert(!value, @"owner unexpectedly retained"); ++callbacks;
        });
        owner = nil;
      }
      NSCAssert(!expired, @"queued notification retained its owner");
      DrainUI(); NSCAssert(callbacks == 1, @"missing callback with a nil owner");
    }
    // Destroying the registration collection cancels all pending callbacks.
    {
      __block NSUInteger callbacks = 0;
      @autoreleasepool {
        NSMutableArray* registrations = [NSMutableArray array]; Add(registrations, @"A", nil);
        PostThumbnailNotifications(registrations, ^(id observer __attribute__((unused)), id owner __attribute__((unused))) { ++callbacks; });
        registrations = nil;
      }
      DrainUI(); NSCAssert(callbacks == 0, @"destroyed registration delivered a callback");
    }
    puts("7 ordered/weak delivery cases passed with dispatch adapter; native queue integration compile-only");
  }
}
'''
nested_adapter = r'''
static NSMutableArray* nested_tasks;
static NSUInteger nested_steps;
static void PostThumbnailTask(dispatch_block_t task) {
  NSCAssert(nested_tasks.count < 4, @"nestable test queue exceeded its bound");
  [nested_tasks addObject:[task copy]];
}
static void RunNext(void) {
  NSCAssert(nested_tasks.count && ++nested_steps <= 4, @"unexpected nested task");
  dispatch_block_t task = [nested_tasks.firstObject copy];
  [nested_tasks removeObjectAtIndex:0];
  task();
}
'''
nested_checks = r'''
static void Add(NSMutableArray* registrations) {
  WebextrasThumbnailRegistration* registration = [WebextrasThumbnailRegistration new];
  registration.observer = @"A";
  [registrations addObject:registration];
}
int main(void) {
  @autoreleasepool {
    nested_tasks = [NSMutableArray array];
    NSMutableArray* registrations = [NSMutableArray array];
    Add(registrations);
    __weak WebextrasThumbnailRegistration* original = registrations.firstObject;
    __block NSUInteger callbacks = 0;
    __block BOOL first = YES;
    void (^delivery)(id, id) = ^(id observer __attribute__((unused)), id owner __attribute__((unused))) {
      ++callbacks;
      if (first) {
        first = NO;
        [registrations removeAllObjects];
        Add(registrations);
        // Native 0x42db6d4..6fc locks the shared control block, invokes the
        // observer at 0x42db708..710, and releases only at 0x42db734..73c.
        // Self-removal therefore leaves the in-flight registration lockable.
        NSCAssert(original, @"in-flight registration was released early");
        RunNext();
        NSCAssert(callbacks == 2, @"nested old delivery lost its shared lock");
      }
    };
    // Three deliveries refer to the old token. The second nests within the
    // first, while the third runs after the original registration expires.
    for (NSUInteger index = 0; index < 3; ++index) {
      PostThumbnailNotifications(registrations, delivery);
    }
    RunNext();
    NSCAssert(callbacks == 2 && !original, @"old registration outlived its callback");
    RunNext();
    NSCAssert(callbacks == 2, @"expired old token delivered after unwinding");
    PostThumbnailNotifications(registrations, delivery);
    RunNext();
    NSCAssert(callbacks == 3, @"new registration did not receive its emission");
    NSCAssert(nested_steps == 4 && !nested_tasks.count, @"incomplete nestable FIFO test");
    puts("8 total ordered/weak delivery cases passed (7 dispatch, 1 nestable FIFO); native queue integration compile-only");
  }
}
'''
with tempfile.TemporaryDirectory(prefix="webextras-thumbnail-") as temporary:
    folder = Path(temporary)
    program = folder / "delivery.m"
    adapter = "static void PostThumbnailTask(dispatch_block_t task) { dispatch_async(dispatch_get_main_queue(), task); }\n"
    program.write_text("#import <Foundation/Foundation.h>\n#import <CoreFoundation/CoreFoundation.h>\n#import <dispatch/dispatch.h>\n" + adapter + helper + checks)
    executable = folder / "delivery"
    subprocess.run(["xcrun", "clang", "-fobjc-arc", "-Wall", "-Wextra", "-Werror", "-framework", "Foundation", str(program), "-o", str(executable)], check=True)
    subprocess.run([str(executable)], check=True, timeout=15)
    nested_program = folder / "nested_delivery.m"
    nested_program.write_text("#import <Foundation/Foundation.h>\n#import <dispatch/dispatch.h>\n" + nested_adapter + helper + nested_checks)
    nested_executable = folder / "nested_delivery"
    subprocess.run(["xcrun", "clang", "-fobjc-arc", "-Wall", "-Wextra", "-Werror", "-O2", "-framework", "Foundation", str(nested_program), "-o", str(nested_executable)], check=True)
    subprocess.run([str(nested_executable)], check=True, timeout=15)
