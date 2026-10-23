// ==== 0x44145b0  ArcWebContentsBoostIDE::sendErrorResponse:jsonString:  (260 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcWebContentsBoostIDE::sendErrorResponse_jsonString_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  code *pcVar1;
  SEL extraout_x1;
  long lVar2;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  undefined8 local_48;
  undefined8 uStack_40;
  ulong local_38;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    if (param_3 == 0) {
      local_48 = 0;
      uStack_40 = 0;
      local_38 = 0;
    }
    else {
      FUN_01b356b8(&local_48,param_3);
      param_2 = extraout_x1;
    }
    if (param_4 == 0) {
      local_60 = 0;
      uStack_58 = 0;
      local_50 = 0;
    }
    else {
      FUN_01b356b8(&local_60,param_4,param_2);
    }
    lVar2 = *(long *)(*(long *)(param_1 + 0x18) + 0x28);
    if (lVar2 != 0) {
      FUN_043d38cc(lVar2 + -0x28,&local_48,&local_60);
    }
    if ((long)local_50 < 0) {
      FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
    }
    if ((long)local_38 < 0) {
      FUN_00005810(local_48,local_38 & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44146b4);
  (*pcVar1)();
}


