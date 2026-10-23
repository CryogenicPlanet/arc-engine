// ==== 0x43dd10c  ArcAIServiceConfiguration::timeout  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAIServiceConfiguration::timeout(int param_1)

{
  code *pcVar1;
  
  if ((*(byte *)((ulong)(uint)param_1 + 0x98) & 1) != 0) {
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43dd120);
  (*pcVar1)();
}


