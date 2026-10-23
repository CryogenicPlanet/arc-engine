// ==== 0x44354f4  ArcBrowserContextPreferences::clearAutoOpenFiles  (40 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserContextPreferences::clearAutoOpenFiles(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    FUN_07dab804(*(undefined8 *)(*(long *)(param_1 + 0x10) + 0x70));
    FUN_07dac02c();
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443551c);
  (*pcVar1)();
}


