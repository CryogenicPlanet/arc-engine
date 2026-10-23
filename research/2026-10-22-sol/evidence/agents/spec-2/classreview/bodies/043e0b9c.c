// ==== 0x43e0b9c  ArcBrowser::setWindowIdentifier:  (152 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowser::setWindowIdentifier_(ID param_1,SEL param_2,ID param_3)

{
  cfstringStruct *pcVar1;
  code *pcVar2;
  undefined8 local_48 [2];
  undefined7 local_38;
  char cStack_31;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    pcVar1 = &cf___;
    if (param_3 != 0) {
      pcVar1 = (cfstringStruct *)param_3;
    }
    FUN_01b356b8(local_48,pcVar1);
    FUN_043341e4(param_1 + 0x28,local_48);
    if (cStack_31 < '\0') {
      FUN_00005810(local_48[0],CONCAT17(cStack_31,local_38) & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x43e0c34);
  (*pcVar2)();
}


