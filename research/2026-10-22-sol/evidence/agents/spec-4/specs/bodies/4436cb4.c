// ==== 0x4436cb4  ArcBrowserContextEffects::registerColorScheme:forHostPattern:  (164 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcBrowserContextEffects::registerColorScheme_forHostPattern_
               (ID param_1,SEL param_2,long_long param_3,ID param_4)

{
  code *pcVar1;
  undefined8 local_48;
  undefined8 uStack_40;
  ulong local_38;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if (param_4 == 0) {
      local_48 = 0;
      uStack_40 = 0;
      local_38 = 0;
    }
    else {
      FUN_01b356b8(&local_48,param_4);
    }
    FUN_042e3140(*(undefined8 *)(param_1 + 0x10),param_3,&local_48);
    if ((long)local_38 < 0) {
      FUN_00005810(local_48,local_38 & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4436d58);
  (*pcVar1)();
}


