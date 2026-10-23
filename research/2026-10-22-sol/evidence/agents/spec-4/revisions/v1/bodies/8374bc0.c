// ==== 0x8374bc0  ArcPerformanceController::temporarilyDisableBatterySaverMode:  (116 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcPerformanceController::temporarilyDisableBatterySaverMode_
               (ID param_1,SEL param_2,bool param_3)

{
  int iVar1;
  
  if (((DAT_0e4a2f20 & 1) == 0) && (iVar1 = FUN_0c44b610(&DAT_0e4a2f20), iVar1 != 0)) {
    thunk_FUN_00008790(0x40,param_3);
    DAT_0e4a2f18 = FUN_0433d1bc();
    FUN_0c44b760(&DAT_0e4a2f20);
  }
  FUN_07afc3a8(DAT_0e49d480,param_3);
  return;
}


