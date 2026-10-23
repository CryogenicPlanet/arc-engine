// ==== 0x4441eec  ArcWebContents::removeColorFilter  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::removeColorFilter(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_043793ac(*(undefined8 *)(param_1 + 0x118));
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4441f00);
  (*pcVar1)();
}


