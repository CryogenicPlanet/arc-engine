// ==== 0x43e96ac  ArcBrowserApplication::setFreeMemoryTargetOnCriticalMemoryPressure:  (40 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setFreeMemoryTargetOnCriticalMemoryPressure_
               (ID param_1,SEL param_2,unsigned_long_long param_3)

{
  code *pcVar1;
  
  if (((-1 < (long)param_3) && (param_3 >> 0x35 == 0)) && (param_3 >> 0x2b == 0)) {
    DAT_0e2ef310 = param_3 << 0x14;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43e96d4);
  (*pcVar1)();
}


