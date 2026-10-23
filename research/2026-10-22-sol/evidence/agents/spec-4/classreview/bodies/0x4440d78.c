// ==== 0x4440d78  ArcWebContents::isInitialized  (32 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContents::isInitialized(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    return (bool)(*(long *)(*(long *)(param_1 + 0x118) + 0x130) != 0);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4440d98);
  (*pcVar1)();
}


