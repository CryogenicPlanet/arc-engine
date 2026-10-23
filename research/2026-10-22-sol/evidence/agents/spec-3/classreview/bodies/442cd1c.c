// ==== 0x442cd1c  ArcSearchEngine::isPrepopulated  (32 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcSearchEngine::isPrepopulated(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)(0 < *(int *)(*(long *)(param_1 + 8) + 0x354));
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442cd3c);
  (*pcVar1)();
}


