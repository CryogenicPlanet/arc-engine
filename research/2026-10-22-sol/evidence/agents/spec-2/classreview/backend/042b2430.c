// ==== 0x42b2430  FUN_042b2430  (348 bytes)

undefined8 FUN_042b2430(long param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  long lVar4;
  code *pcVar5;
  undefined8 uVar6;
  byte *local_168;
  long lStack_160;
  char *local_158 [36];
  char local_32;
  byte local_31;
  
  if (*(long *)(param_1 + 0x98) == 0) {
    uVar2 = 1;
  }
  else {
    if (DAT_0e49cf50 == '\0') {
      DAT_0e49cf50 = '\x01';
      uVar2 = *param_2;
      uVar1 = param_2[1];
      uVar6 = param_2[3];
      uVar3 = param_2[2];
      if (*(code **)param_2[3] != (code *)0x0) {
        uVar2 = (**(code **)param_2[3])();
      }
      lVar4 = *(long *)(param_1 + 0x168);
      *(undefined8 *)(param_1 + 0x168) = uVar6;
      *(undefined8 *)(param_1 + 0x160) = uVar3;
      uVar3 = *(undefined8 *)(param_1 + 0x150);
      *(undefined8 *)(param_1 + 0x150) = uVar2;
      *(undefined8 *)(param_1 + 0x158) = uVar1;
      pcVar5 = *(code **)(lVar4 + 8);
      if (pcVar5 != (code *)0x0) {
        (*pcVar5)(uVar3);
      }
      local_31 = 0;
      local_32 = '\x01';
      local_168 = &local_31;
      local_158[0] = &local_32;
      lStack_160 = param_1;
      FUN_018f0070(*(undefined8 *)(*(long *)(DAT_0e481618 + 0x2a0) + 8),&local_168,FUN_042c21a8,0,0)
      ;
      if (local_32 == '\0') {
        return 2;
      }
      if ((local_31 & 1) != 0) {
        return 2;
      }
      FUN_042b258c(param_1);
    }
    else if ((DAT_0e4a3078 < 2) && (DAT_0e2f1ce0 != 0 || DAT_0e4a3080 != 0)) {
      FUN_00f195d8(&local_168,"../../arc/browser/browser_application_impl.cc",0x1e4,1);
      FUN_00040060(local_158,
                   "Trying to terminate the browser that is terminating. Deferring to later.",0x48);
      FUN_00f1950c(&local_168);
    }
    uVar2 = 2;
  }
  return uVar2;
}


