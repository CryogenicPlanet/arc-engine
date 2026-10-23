// ==== 0x43fd248  ArcBrowserContextExtensions::uninstallExtension:  (188 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserContextExtensions::uninstallExtension_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  undefined8 uVar2;
  SEL extraout_x1;
  long lVar3;
  undefined8 local_38;
  undefined8 uStack_30;
  ulong local_28;
  
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    if (param_3 == 0) {
      local_38 = 0;
      uStack_30 = 0;
      local_28 = 0;
    }
    else {
      FUN_01b356b8(&local_38,param_3);
      param_2 = extraout_x1;
    }
    lVar3 = *(long *)(*(long *)(param_1 + 0x20) + 0x68);
    if (lVar3 != 0) {
      uVar2 = FUN_0238b08c(0,param_2);
      lVar3 = FUN_0009278c(uVar2,lVar3,1);
      if (lVar3 != 0) {
        FUN_06afe0e0(lVar3,&local_38,1,0,0);
      }
    }
    if ((long)local_28 < 0) {
      FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43fd304);
  (*pcVar1)();
}


