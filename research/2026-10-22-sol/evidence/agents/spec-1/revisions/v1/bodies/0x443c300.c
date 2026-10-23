// ==== 0x443c300  ArcMediaPosition::playbackRate  (24 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcMediaPosition::playbackRate(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return **(double **)(param_1 + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443c318);
  (*pcVar1)();
}


