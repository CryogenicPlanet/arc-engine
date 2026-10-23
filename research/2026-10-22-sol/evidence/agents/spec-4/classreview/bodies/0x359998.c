// ==== 0x359998  FUN_00359998  (320 bytes)

undefined8 * FUN_00359998(undefined8 *param_1,undefined8 *param_2,ulong param_3)

{
  char cVar1;
  code *pcVar2;
  undefined8 *puVar3;
  ulong uVar4;
  ulong uVar5;
  
  if (param_3 < 0x17) {
    *(char *)((long)param_1 + 0x17) = (char)param_3;
    if ((param_1 <= (undefined8 *)((long)param_1 + param_3)) &&
       ((undefined8 *)((long)param_1 + param_3) <= param_2 || param_2 < param_1)) {
      _memmove(param_1,param_2,param_3);
      *(undefined1 *)((long)param_1 + param_3) = 0;
      return param_1;
    }
  }
  else {
    if (0x7fffffffffffffee < param_3) {
                    /* WARNING: Subroutine does not return */
      FUN_04b089c0();
    }
    if (*(char *)((long)param_1 + 0x17) < '\0') {
      uVar4 = (param_1[2] & 0x7fffffffffffffff) - 1;
      uVar5 = param_3;
      if (param_3 <= uVar4 * 2) {
        uVar5 = uVar4 * 2;
      }
      uVar5 = (uVar5 & 0xfffffffffffffff0) + 0x10;
      if (0x3fffffffffffffe7 < uVar4) {
        uVar5 = 0x7ffffffffffffff0;
      }
    }
    else {
      uVar5 = param_3;
      if (param_3 < 0x2d) {
        uVar5 = 0x2c;
      }
      uVar5 = (uVar5 & 0x7ffffffffffffff0) + 0x10;
    }
    puVar3 = (undefined8 *)FUN_00008790(uVar5);
    if ((puVar3 <= (undefined8 *)((long)puVar3 + param_3)) &&
       ((undefined8 *)((long)puVar3 + param_3) <= param_2 || param_2 < puVar3)) {
      cVar1 = *(char *)((long)param_1 + 0x17);
      _memcpy(puVar3,param_2,param_3);
      *(undefined1 *)((long)puVar3 + param_3) = 0;
      if (cVar1 < '\0') {
        FUN_00005810(*param_1,param_1[2] & 0x7fffffffffffffff);
      }
      *param_1 = puVar3;
      param_1[1] = param_3;
      param_1[2] = uVar5 | 0x8000000000000000;
      return param_1;
    }
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x359ad4);
  (*pcVar2)();
}


