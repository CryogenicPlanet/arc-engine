// ==== 0x4441db4  ArcWebContents::discard  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::discard(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_04374c00(*(undefined8 *)(param_1 + 0x118));
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4441dc8);
  (*pcVar1)();
}


