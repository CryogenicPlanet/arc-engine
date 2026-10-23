// ==== 0x443551c  ArcBrowserContextPreferences::promptForDownloadPath  (84 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBrowserContextPreferences::promptForDownloadPath(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  uint uVar3;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if (*(long *)(*(long *)(param_1 + 0x10) + 0x70) == 0) {
      uVar3 = 0;
    }
    else {
      lVar2 = FUN_07dab804();
      FUN_022559a8(lVar2 + 8);
      uVar3 = (uint)*(byte *)(*(long *)(lVar2 + 0x50) + 0x1b);
    }
    return uVar3 & 1;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4435570);
  (*pcVar1)();
}


