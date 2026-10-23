// ==== 0x4415e20  ArcWebContentsDevTools::closeProtocolConnection:  (84 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContentsDevTools::closeProtocolConnection_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    IVar2 = _objc_msgSend(param_3,"intValue");
    FUN_04356754(*(undefined8 *)(param_1 + 0x20),IVar2);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4415e74);
  (*pcVar1)();
}


