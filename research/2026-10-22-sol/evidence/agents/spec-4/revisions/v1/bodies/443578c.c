// ==== 0x443578c  ArcBrowserContextPreferences::notificationPermissionRequestsEnabled  (64 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBrowserContextPreferences::notificationPermissionRequestsEnabled(ID param_1,SEL param_2)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  undefined8 uVar4;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    uVar3 = 0;
    if (*(long *)(*(long *)(param_1 + 0x10) + 0x70) != 0) {
      uVar4 = FUN_0190f6b8();
      iVar2 = FUN_00d5b400(uVar4,6,0);
      uVar3 = (uint)(iVar2 != 2);
    }
    return uVar3;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44357cc);
  (*pcVar1)();
}


