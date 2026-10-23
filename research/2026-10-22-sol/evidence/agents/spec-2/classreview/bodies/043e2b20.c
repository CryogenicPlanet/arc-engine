// ==== 0x43e2b20  ArcBrowser::canRestoreWebContentsWithSessionId:  (124 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool ArcBrowser::canRestoreWebContentsWithSessionId_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  bool bVar2;
  ID IVar3;
  undefined4 local_28;
  undefined1 local_24;
  
  if ((*(byte *)(param_1 + 0x38) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x43e2b98);
    (*pcVar1)();
  }
  IVar3 = _objc_msgSend(param_3,"sessionId");
  local_28 = (undefined4)IVar3;
  local_24 = (undefined1)(IVar3 >> 0x20);
  if ((IVar3 >> 0x20 & 1) != 0) {
    bVar2 = FUN_042fae00(*(undefined8 *)(param_1 + 0x28),&local_28);
    return bVar2;
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}


