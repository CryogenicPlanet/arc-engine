// ==== 0x44369e8  ArcBrowserContextEffects::unregisterScript:  (184 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserContextEffects::unregisterScript_(ID param_1,SEL param_2,ID param_3)

{
  ulong uVar1;
  code *pcVar2;
  undefined8 local_48;
  undefined8 uStack_40;
  ulong local_38;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if (param_3 == 0) {
      local_48 = 0;
      uStack_40 = 0;
      local_38 = 0;
      FUN_042e2240(*(undefined8 *)(param_1 + 0x10),&local_48,1);
    }
    else {
      FUN_01b356b8(&local_48,param_3);
      uVar1 = local_38;
      FUN_042e2240(*(undefined8 *)(param_1 + 0x10),&local_48,1);
      if ((long)uVar1 < 0) {
        FUN_00005810(local_48,local_38 & 0x7fffffffffffffff);
      }
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4436aa0);
  (*pcVar2)();
}


