// ==== 0x444202c  ArcWebContents::skipUserActivationForPiP:  (24 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContents::skipUserActivationForPiP_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_04379998(*(undefined8 *)(param_1 + 0x118),param_3);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4442044);
  (*pcVar1)();
}


