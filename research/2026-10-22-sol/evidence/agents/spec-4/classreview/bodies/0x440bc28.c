// ==== 0x440bc28  ArcSiteSettingInfo::permissionType  (64 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcSiteSettingInfo::permissionType(ID param_1,SEL param_2)

{
  code *pcVar1;
  int iVar2;
  ulong uVar3;
  long_long lVar4;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_04332684(**(undefined4 **)(param_1 + 8));
    uVar3 = FUN_02122528();
    if ((uVar3 >> 0x20 & 1) == 0) {
      lVar4 = 0x1c;
    }
    else {
      iVar2 = FUN_0433f850();
      lVar4 = (long_long)iVar2;
    }
    return lVar4;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440bc68);
  (*pcVar1)();
}


