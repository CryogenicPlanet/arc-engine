// ==== 0x359ad8  FUN_00359ad8  (328 bytes)

undefined8 * FUN_00359ad8(undefined8 *param_1,void *param_2,ulong param_3)

{
  ulong uVar1;
  char cVar2;
  code *pcVar3;
  ulong uVar4;
  ulong uVar5;
  void *pvVar6;
  
  uVar5 = param_1[2] & 0x7fffffffffffffff;
  if (param_3 < uVar5) {
    pvVar6 = (void *)*param_1;
    param_1[1] = param_3;
    if (((void *)((long)pvVar6 + param_3) < pvVar6) ||
       (param_2 < (void *)((long)pvVar6 + param_3) && pvVar6 <= param_2)) {
LAB_00359c18:
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(1,0x359c1c);
      (*pcVar3)();
    }
    _memmove(pvVar6,param_2,param_3);
    *(undefined1 *)((long)pvVar6 + param_3) = 0;
  }
  else {
    if (0x7fffffffffffffee < param_3) {
                    /* WARNING: Subroutine does not return */
      FUN_04b089c0();
    }
    uVar4 = uVar5 - 1;
    uVar1 = param_3;
    if (param_3 <= uVar4 * 2) {
      uVar1 = uVar4 * 2;
    }
    uVar1 = (uVar1 & 0xfffffffffffffff0) + 0x10;
    if (0x3fffffffffffffe7 < uVar4) {
      uVar1 = 0x7ffffffffffffff0;
    }
    uVar4 = param_3;
    if (param_3 < 0x2d) {
      uVar4 = 0x2c;
    }
    if ((param_1[2] & 0x8000000000000000) == 0) {
      uVar1 = (uVar4 & 0x7ffffffffffffff0) + 0x10;
    }
    pvVar6 = (void *)FUN_00008790(uVar1);
    cVar2 = *(char *)((long)param_1 + 0x17);
    if (param_3 != 0) {
      if (((void *)((long)pvVar6 + param_3) < pvVar6) ||
         (param_2 < (void *)((long)pvVar6 + param_3) && pvVar6 <= param_2)) goto LAB_00359c18;
      _memcpy(pvVar6,param_2,param_3);
    }
    *(undefined1 *)((long)pvVar6 + param_3) = 0;
    if (cVar2 < '\0') {
      FUN_00005810(*param_1,uVar5);
    }
    *param_1 = pvVar6;
    param_1[1] = param_3;
    param_1[2] = uVar1 | 0x8000000000000000;
  }
  return param_1;
}


