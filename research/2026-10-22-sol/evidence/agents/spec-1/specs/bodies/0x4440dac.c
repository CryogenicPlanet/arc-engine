// ==== 0x4440dac  ArcWebContents::bootstrapRenderingComplete  (64 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::bootstrapRenderingComplete(ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 local_18;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    local_18 = 0;
    FUN_01b92718(*(long *)(param_1 + 0x118) + 0x5f8,&local_18);
    FUN_01941cb4(&local_18);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4440dec);
  (*pcVar1)();
}


