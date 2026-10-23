// ==== 0x7ab624  FUN_007ab624  (320 bytes)

undefined8 * FUN_007ab624(undefined8 *param_1,undefined8 *param_2,ulong param_3)

{
  undefined8 *puVar1;
  ulong uVar2;
  char cVar3;
  code *pcVar4;
  undefined8 *puVar5;
  ulong uVar6;
  ulong uVar7;
  
  if (param_3 < 0xb) {
    *(char *)((long)param_1 + 0x17) = (char)param_3;
    puVar1 = (undefined8 *)((long)param_1 + param_3 * 2);
    if ((param_1 <= puVar1) && (puVar1 <= param_2 || param_2 < param_1)) {
      _memmove(param_1,param_2,param_3 << 1);
      *(undefined2 *)((long)param_1 + param_3 * 2) = 0;
      return param_1;
    }
  }
  else {
    if (0x7fffffffffffffee < param_3) {
                    /* WARNING: Subroutine does not return */
      FUN_04c31780();
    }
    if (*(char *)((long)param_1 + 0x17) < '\0') {
      uVar6 = (param_1[2] & 0x7fffffffffffffff) - 1;
      uVar2 = param_3;
      if (param_3 <= uVar6 * 2) {
        uVar2 = uVar6 * 2;
      }
      uVar7 = 0x7ffffffffffffff0;
      if (uVar6 < 0x3fffffffffffffe8) {
        uVar7 = (uVar2 & 0x7ffffffffffffff8) + 8;
      }
    }
    else {
      uVar2 = param_3;
      if (param_3 < 0x15) {
        uVar2 = 0x14;
      }
      uVar7 = (uVar2 & 0x7ffffffffffffff8) + 8;
    }
    puVar5 = (undefined8 *)FUN_00008790(uVar7 << 1);
    puVar1 = (undefined8 *)((long)puVar5 + param_3 * 2);
    if ((puVar5 <= puVar1) && (puVar1 <= param_2 || param_2 < puVar5)) {
      cVar3 = *(char *)((long)param_1 + 0x17);
      _memcpy(puVar5,param_2,param_3 << 1);
      *(undefined2 *)((long)puVar5 + param_3 * 2) = 0;
      if (cVar3 < '\0') {
        FUN_00005810(*param_1,param_1[2] << 1);
      }
      *param_1 = puVar5;
      param_1[1] = param_3;
      param_1[2] = uVar7 | 0x8000000000000000;
      return param_1;
    }
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x7ab760);
  (*pcVar4)();
}


