// ==== 0x43e1e70  ArcBrowser::removeCookie:withName:  (216 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcBrowser::removeCookie_withName_(ID param_1,SEL param_2,ID param_3,ID param_4)

{
  code *pcVar1;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  undefined8 local_48 [2];
  undefined7 local_38;
  char cStack_31;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    FUN_043abdf8(local_48,param_3);
    if (param_4 == 0) {
      local_60 = 0;
      uStack_58 = 0;
      local_50 = 0;
    }
    else {
      FUN_01b356b8(&local_60,param_4);
    }
    FUN_04334078(param_1 + 0x28,local_48,&local_60);
    if ((long)local_50 < 0) {
      FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
    }
    if (cStack_31 < '\0') {
      FUN_00005810(local_48[0],CONCAT17(cStack_31,local_38) & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43e1f48);
  (*pcVar1)();
}


