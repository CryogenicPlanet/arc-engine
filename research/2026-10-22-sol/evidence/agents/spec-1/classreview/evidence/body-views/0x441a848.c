// ==== 0x441a848  ArcWebContentsMediaSession::getMediaContentTypes:  (344 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcWebContentsMediaSession::getMediaContentTypes_(ID param_1,SEL param_2,ID *param_3)

{
  code *pcVar1;
  uint uVar2;
  ID IVar3;
  uint *puVar4;
  long lVar5;
  uint *local_58;
  uint *local_50;
  uint *puStack_48;
  ulong local_40;
  long local_38;
  
  IVar3 = _objc_alloc_init(&_OBJC_CLASS___NSMutableIndexSet);
  if ((*(byte *)(param_1 + 0x40) & 1) != 0) {
    puStack_48 = (uint *)0x0;
    local_50 = (uint *)0x0;
    local_38 = 0;
    local_40 = 0;
    FUN_043608d8(&local_50,*(undefined8 *)(param_1 + 0x30));
    if (local_50 == (uint *)((long)&MACH_HEADER.magic + 1)) {
      puVar4 = puStack_48;
      for (lVar5 = local_40 << 2; lVar5 != 0; lVar5 = lVar5 + -4) {
        if (*puVar4 < 4) {
          NSMutableIndexSet::addIndex_(IVar3,"addIndex:");
        }
        puVar4 = puVar4 + 1;
      }
      IVar3 = NSMutableIndexSet::copy(IVar3,"copy");
    }
    else {
      local_58 = local_50;
      puVar4 = local_50;
      if (((ulong)local_50 & 1) == 0) {
        uVar2 = *local_50;
        *local_50 = uVar2 + 1;
        puVar4 = (uint *)(ulong)uVar2;
      }
      FUN_04402548(puVar4,&local_58,param_3);
      if (((ulong)local_58 & 1) == 0) {
        FUN_04b35898();
      }
      IVar3 = 0;
    }
    if (local_50 == (uint *)((long)&MACH_HEADER.magic + 1)) {
      if (puStack_48 != (uint *)0x0) {
        if ((local_40 & 0x3fffffffffffffff) != 0) {
          lVar5 = local_40 * -4;
          puVar4 = puStack_48 + local_40;
          do {
            puVar4 = puVar4 + -1;
            if (puVar4 == (uint *)0x0) goto LAB_0441a9ac;
            lVar5 = lVar5 + 4;
          } while (lVar5 != 0);
        }
        local_40 = 0;
        FUN_00005810(puStack_48,local_38 << 2);
      }
    }
    else if (((ulong)local_50 & 1) == 0) {
      FUN_04b35898();
    }
    return IVar3;
  }
LAB_0441a9ac:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x441a9b0);
  (*pcVar1)();
}


