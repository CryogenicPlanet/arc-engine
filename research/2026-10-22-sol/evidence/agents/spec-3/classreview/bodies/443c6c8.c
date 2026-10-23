// ==== 0x443c6c8  ArcPerformanceController::isBatterySaverModeTemporarilyDisabled  (108 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcPerformanceController::isBatterySaverModeTemporarilyDisabled(ID param_1,SEL param_2)

{
  int iVar1;
  uint uVar2;
  
  if (((DAT_0e4a2f20 & 1) == 0) && (iVar1 = thunk_FUN_0c44b610(&DAT_0e4a2f20), iVar1 != 0)) {
    FUN_00008790(0x40);
    DAT_0e4a2f18 = FUN_0433d1bc();
    thunk_FUN_0c44b760(&DAT_0e4a2f20);
  }
  uVar2 = 0;
  if (*(long *)(DAT_0e49d480 + 0x18) != 0) {
    uVar2 = (uint)*(byte *)(*(long *)(DAT_0e49d480 + 0x18) + 0x29);
  }
  return uVar2 & 1;
}


