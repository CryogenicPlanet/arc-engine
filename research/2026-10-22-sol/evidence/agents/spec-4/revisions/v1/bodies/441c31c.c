// ==== 0x441c31c  ArcWebContentsMediaSessionPiPController::videoView  (96 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcWebContentsMediaSessionPiPController::videoView(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  
  if (*(char *)(param_1 + 0x18) == '\0') {
    IVar2 = 0;
  }
  else {
    lVar1 = *(long *)(*(long *)(param_1 + 8) + 0x10);
    if (lVar1 == 0) {
      IVar2 = 0;
    }
    else {
      IVar2 = *(ID *)(*(long *)(*(long *)(lVar1 + 0x38) + 8) + 0x10);
    }
  }
  return IVar2;
}


