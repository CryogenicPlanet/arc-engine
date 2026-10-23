// ==== 0x443c734  ArcPerformanceController::simulateLowBatteryStateForDuration:  (120 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcPerformanceController::simulateLowBatteryStateForDuration_
               (ID param_1,SEL param_2,unsigned_int param_3)

{
  int iVar1;
  
  if ((DAT_0e4a2f20 & 1) == 0) {
    iVar1 = thunk_FUN_0c44b610(&DAT_0e4a2f20);
    if (iVar1 != 0) {
      FUN_00008790(0x40,param_3);
      DAT_0e4a2f18 = FUN_0433d1bc();
      thunk_FUN_0c44b760(&DAT_0e4a2f20);
    }
  }
  FUN_04208bd4(*(undefined8 *)(DAT_0e49d480 + 0x18),param_3);
  return;
}


