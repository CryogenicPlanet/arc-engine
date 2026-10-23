// ==== 0x44357cc  ArcBrowserContextPreferences::setNotificationPermissionRequestsEnabled:  (80 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcBrowserContextPreferences::setNotificationPermissionRequestsEnabled_
               (ID param_1,SEL param_2,bool param_3)

{
  undefined8 uVar1;
  code *pcVar2;
  undefined8 uVar3;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x443581c);
    (*pcVar2)();
  }
  if (*(long *)(*(long *)(param_1 + 0x10) + 0x70) == 0) {
    return;
  }
  uVar3 = FUN_0190f6b8();
  uVar1 = 2;
  if (param_3 != 0) {
    uVar1 = 3;
  }
  FUN_09e5fc14(uVar3,6,uVar1,0x100000000);
  return;
}


