// ==== 0x4443824  ArcWebContents::translateToTargetLanguage:  (168 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContents::translateToTargetLanguage_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  undefined8 uVar2;
  undefined8 local_38;
  undefined8 uStack_30;
  ulong local_28;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    if (param_3 == 0) {
      local_38 = 0;
      uStack_30 = 0;
      local_28 = 0;
    }
    else {
      FUN_01b356b8(&local_38,param_3);
    }
    uVar2 = FUN_000805e0(*(long *)(*(long *)(param_1 + 0x118) + 0x130) + 8,&DAT_0c9d7100);
    FUN_0434e058(uVar2,&local_38);
    if ((long)local_28 < 0) {
      FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44438cc);
  (*pcVar1)();
}


