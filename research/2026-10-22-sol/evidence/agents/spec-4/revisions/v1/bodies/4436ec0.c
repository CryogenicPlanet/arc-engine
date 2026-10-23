// ==== 0x4436ec0  ArcBrowserContextEffects::unregisterZoomFactor:  (204 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserContextEffects::unregisterZoomFactor_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  long lVar2;
  SEL extraout_x1;
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  undefined1 local_21;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if (param_3 == 0) {
      local_40 = 0;
      uStack_38 = 0;
      local_30 = 0;
    }
    else {
      FUN_01b356b8(&local_40,param_3);
      param_2 = extraout_x1;
    }
    lVar2 = *(long *)(*(long *)(param_1 + 0x10) + 0x30);
    if (lVar2 != 0) {
      FUN_0119426c(lVar2,param_2);
      lVar2 = FUN_04146794();
      if (lVar2 != 0) {
        local_21 = 0;
        FUN_000f71cc(&local_21,0,0xa0);
        FUN_064fee44(*(undefined8 *)(lVar2 + 0x70),lVar2,&local_40);
      }
    }
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4436f8c);
  (*pcVar1)();
}


