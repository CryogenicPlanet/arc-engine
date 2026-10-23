// ==== 0x4441d80  ArcWebContents::stop  (32 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::stop(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4441da0);
    (*pcVar1)();
  }
  if (*(long *)(*(long *)(param_1 + 0x118) + 0x130) != 0) {
    FUN_02fea8a0();
    return;
  }
  return;
}


