// ==== 0x7ab764  FUN_007ab764  (328 bytes)

undefined8 * FUN_007ab764(undefined8 *param_1,void *param_2,ulong param_3)

{
  ulong uVar1;
  ulong uVar2;
  char cVar3;
  code *pcVar4;
  void *pvVar5;
  ulong uVar6;
  void *pvVar7;
  ulong uVar8;
  
  uVar8 = param_1[2];
  if (param_3 < (uVar8 & 0x7fffffffffffffff)) {
    pvVar7 = (void *)*param_1;
    param_1[1] = param_3;
    pvVar5 = (void *)((long)pvVar7 + param_3 * 2);
    if ((pvVar5 < pvVar7) || (param_2 < pvVar5 && pvVar7 <= param_2)) {
LAB_007ab8a4:
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(1,0x7ab8a8);
      (*pcVar4)();
    }
    _memmove(pvVar7,param_2,param_3 << 1);
    *(undefined2 *)((long)pvVar7 + param_3 * 2) = 0;
  }
  else {
    if (0x7fffffffffffffee < param_3) {
                    /* WARNING: Subroutine does not return */
      FUN_04c31780();
    }
    uVar6 = (uVar8 & 0x7fffffffffffffff) - 1;
    uVar1 = param_3;
    if (param_3 <= uVar6 * 2) {
      uVar1 = uVar6 * 2;
    }
    uVar2 = 0x7ffffffffffffff0;
    if (uVar6 < 0x3fffffffffffffe8) {
      uVar2 = (uVar1 & 0x7ffffffffffffff8) + 8;
    }
    uVar1 = param_3;
    if (param_3 < 0x15) {
      uVar1 = 0x14;
    }
    if ((uVar8 & 0x8000000000000000) == 0) {
      uVar2 = (uVar1 & 0x7ffffffffffffff8) + 8;
    }
    pvVar5 = (void *)FUN_00008790(uVar2 << 1);
    cVar3 = *(char *)((long)param_1 + 0x17);
    if (param_3 != 0) {
      pvVar7 = (void *)((long)pvVar5 + param_3 * 2);
      if ((pvVar7 < pvVar5) || (param_2 < pvVar7 && pvVar5 <= param_2)) goto LAB_007ab8a4;
      _memcpy(pvVar5,param_2,param_3 << 1);
    }
    *(undefined2 *)((long)pvVar5 + param_3 * 2) = 0;
    if (cVar3 < '\0') {
      FUN_00005810(*param_1,uVar8 << 1);
    }
    *param_1 = pvVar5;
    param_1[1] = param_3;
    param_1[2] = uVar2 | 0x8000000000000000;
  }
  return param_1;
}


