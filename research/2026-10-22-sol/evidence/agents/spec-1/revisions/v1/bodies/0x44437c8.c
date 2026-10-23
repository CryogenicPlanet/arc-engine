// ==== 0x44437c8  ArcWebContents::applyColorFilter:  (92 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContents::applyColorFilter_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  undefined1 auStack_70 [80];
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_043deee4(auStack_70,param_3);
    FUN_04379218(*(undefined8 *)(param_1 + 0x118),auStack_70);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4443824);
  (*pcVar1)();
}


