// ==== 0x42a5760  FUN_042a5760  (244 bytes)

undefined8 * FUN_042a5760(undefined8 *param_1,undefined8 *param_2)

{
  code *pcVar1;
  void *pvVar2;
  void *pvVar3;
  ulong uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  
  if (param_1 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x42a584c);
    (*pcVar1)();
  }
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(param_1,*param_2,param_2[1]);
  }
  else {
    uVar6 = param_2[1];
    uVar5 = *param_2;
    param_1[2] = param_2[2];
    param_1[1] = uVar6;
    *param_1 = uVar5;
  }
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  uVar4 = param_2[4];
  if (uVar4 != 0) {
    if (uVar4 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    pvVar3 = (void *)param_2[3];
    pvVar2 = (void *)FUN_00008790(uVar4 << 2);
    param_1[3] = pvVar2;
    param_1[4] = 0;
    param_1[5] = uVar4;
    _memcpy(pvVar2,pvVar3,uVar4 << 2);
    param_1[4] = (long)(uVar4 << 2) >> 2;
  }
  param_1[6] = 0;
  param_1[7] = 0;
  param_1[8] = 0;
  uVar4 = param_2[7];
  if (uVar4 != 0) {
    if (uVar4 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    pvVar3 = (void *)param_2[6];
    pvVar2 = (void *)FUN_00008790(uVar4 << 2);
    param_1[6] = pvVar2;
    param_1[7] = 0;
    param_1[8] = uVar4;
    _memcpy(pvVar2,pvVar3,uVar4 << 2);
    param_1[7] = (long)(uVar4 << 2) >> 2;
  }
  return param_1;
}


