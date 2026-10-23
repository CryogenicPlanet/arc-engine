// ==== 0x4441e30  ArcWebContents::setZoomFactor:  (20 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContents::setZoomFactor_(ID param_1,SEL param_2,float param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_043776b8(*(undefined8 *)(param_1 + 0x118));
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4441e44);
  (*pcVar1)();
}


