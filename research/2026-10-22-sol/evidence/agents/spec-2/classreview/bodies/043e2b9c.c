// ==== 0x43e2b9c  ArcBrowser::restoreWebContentsWithSessionId:  (116 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowser::restoreWebContentsWithSessionId_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  ID IVar2;
  undefined4 local_28;
  undefined1 local_24;
  
  if ((*(byte *)(param_1 + 0x38) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x43e2c0c);
    (*pcVar1)();
  }
  IVar2 = _objc_msgSend(param_3,"sessionId");
  local_28 = (undefined4)IVar2;
  local_24 = (undefined1)(IVar2 >> 0x20);
  if ((IVar2 >> 0x20 & 1) != 0) {
    FUN_042faed0(*(undefined8 *)(param_1 + 0x28),&local_28);
    return;
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}


