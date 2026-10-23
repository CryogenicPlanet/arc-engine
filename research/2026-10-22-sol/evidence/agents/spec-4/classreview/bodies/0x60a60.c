// ==== 0x60a60  FUN_00060a60  (152 bytes)

void FUN_00060a60(undefined8 *param_1,undefined8 *param_2,ulong param_3)

{
  undefined8 *puVar1;
  code *pcVar2;
  undefined8 *puVar3;
  
  if (0x7fffffffffffffee < param_3) {
                    /* WARNING: Subroutine does not return */
    FUN_04b089c0();
  }
  if (param_3 < 0x17) {
    *(char *)((long)param_1 + 0x17) = (char)param_3;
    puVar3 = param_1;
  }
  else {
    puVar3 = (undefined8 *)FUN_00008790((param_3 & 0x7ffffffffffffff0) + 0x10);
    *param_1 = puVar3;
    param_1[1] = param_3;
    param_1[2] = (param_3 & 0x7ffffffffffffff0) + 0x8000000000000010;
  }
  puVar1 = (undefined8 *)((long)puVar3 + param_3 + 1);
  if ((puVar3 <= puVar1) && (puVar1 <= param_2 || param_2 < puVar3)) {
    _memmove(puVar3,param_2,param_3 + 1);
    return;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x60af4);
  (*pcVar2)();
}


