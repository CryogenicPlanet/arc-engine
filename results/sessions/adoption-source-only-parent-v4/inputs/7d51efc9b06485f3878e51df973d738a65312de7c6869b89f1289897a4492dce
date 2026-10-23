// Copyright 2026 ArcMod. C exports that Arc's app (and ArcCore's own callers) link against.

#import <Foundation/Foundation.h>

#include "base/apple/call_with_eh_frame.h"

#define ARC_EXPORT extern "C" __attribute__((visibility("default")))

// Error domain for NSErrors from the engine (same value as Arc's engine).
ARC_EXPORT NSString* const ArcErrorDomain = @"company.thebrowser.Arc";

// Runs |block| synchronously inside Chromium's special exception frame, so an ObjC exception
// thrown inside terminates at the throw site (base/apple/call_with_eh_frame.h). Borrowed, not copied.
ARC_EXPORT void CallWithEHFrame(void (^block)(void)) {
  base::apple::CallWithEHFrame(block);
}

// Arc's app gates its trace events on *arcCategoryEnabledFlag(). Engine tracing from the app is not
// carried over, so the flag stays 0 and emitTraceEvent is never reached from the app.
ARC_EXPORT const unsigned char* arcCategoryEnabledFlag() {
  static const unsigned char kDisabled = 0;
  return &kDisabled;
}

ARC_EXPORT void emitTraceEvent(char phase,
                               NSString* name,
                               uint64_t a,
                               uint64_t b,
                               NSString* arg_name,
                               uint64_t c,
                               NSString* arg_value) {}

// Not imported by Arc's app; exported for parity with Arc's engine. Handler is ignored.
ARC_EXPORT void SetLogMessageHandler(void* handler) {}
