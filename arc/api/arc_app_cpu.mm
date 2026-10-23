// Copyright 2026 ArcMod. CPU snapshot exposed at the app/engine boundary.
#include "arc/api/arc_internal.h"
#include "base/cpu.h"

ARC_EXPORT @interface ArcCpuInfo : NSObject
@end

@implementation ArcCpuInfo {
  base::CPU _cpu;
}
- (NSString*)brand { return ArcNSString(_cpu.cpu_brand()); }
- (NSString*)vendor { return ArcNSString(_cpu.vendor_name()); }
- (NSString*)architecture { return @"arm"; }
- (NSInteger)type { return _cpu.type(); }
- (NSInteger)family { return _cpu.family(); }
- (NSInteger)model { return _cpu.model(); }
- (NSInteger)stepping { return _cpu.stepping(); }
- (NSInteger)extModel { return _cpu.extended_model(); }
- (NSInteger)extFamily { return _cpu.extended_family(); }
// Chromium's macOS ARM CPU snapshot does not query MIDR.
- (NSUInteger)armPartNumber { return 0; }
- (unsigned char)armImplementer { return 0; }
- (unsigned char)isHypervisorActive { return _cpu.is_running_in_vm(); }
- (unsigned char)hasBMI2 { return 0; }
- (unsigned char)hasMMX { return 0; }
- (unsigned char)hasSSE { return 0; }
- (unsigned char)hasSSE2 { return 0; }
- (unsigned char)hasSSE3 { return 0; }
- (unsigned char)hasSSSE3 { return 0; }
- (unsigned char)hasSSE41 { return 0; }
- (unsigned char)hasSSE42 { return 0; }
- (unsigned char)hasPOPCNT { return 0; }
- (unsigned char)hasAVX { return 0; }
- (unsigned char)hasAVX2 { return 0; }
- (unsigned char)hasFMA3 { return 0; }
- (unsigned char)hasAESNI { return _cpu.has_aesni(); }
- (unsigned char)hasMTE { return _cpu.has_mte(); }
- (unsigned char)hasBTI { return _cpu.has_bti(); }
@end
