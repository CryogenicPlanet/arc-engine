// ==== 0x4440d98  ArcWebContents::bootstrapRendering  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::bootstrapRendering(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_04396620(*(undefined8 *)(param_1 + 0x118));
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4440dac);
  (*pcVar1)();
}


