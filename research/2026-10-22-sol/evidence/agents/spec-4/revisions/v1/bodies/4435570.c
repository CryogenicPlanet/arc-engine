// ==== 0x4435570  ArcBrowserContextPreferences::setPromptForDownloadPath:  (112 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcBrowserContextPreferences::setPromptForDownloadPath_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  long *plVar2;
  undefined8 uVar3;
  undefined1 local_40 [24];
  undefined1 local_28;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    plVar2 = *(long **)(*(long *)(param_1 + 0x10) + 0x70);
    if (plVar2 != (long *)0x0) {
      uVar3 = (**(code **)(*plVar2 + 0x168))();
      local_40[0] = (undefined1)param_3;
      local_28 = 1;
      FUN_0167ca68(uVar3,"download.prompt_for_download",0x1c,local_40);
      FUN_02721bb0(local_40);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44355e0);
  (*pcVar1)();
}


