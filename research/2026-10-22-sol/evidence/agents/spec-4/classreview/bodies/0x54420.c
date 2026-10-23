// ==== 0x54420  FUN_00054420  (160 bytes)

void FUN_00054420(undefined8 *param_1,undefined8 *param_2,ulong param_3)

{
  code *pcVar1;
  undefined8 *puVar2;
  
  if (0x7fffffffffffffee < param_3) {
                    /* WARNING: Subroutine does not return */
    FUN_04c31780();
  }
  if (param_3 < 0xb) {
    *(char *)((long)param_1 + 0x17) = (char)param_3;
  }
  else {
    puVar2 = (undefined8 *)FUN_00008790((param_3 & 0x7ffffffffffffff8) * 2 + 0x10);
    *param_1 = puVar2;
    param_1[1] = param_3;
    param_1[2] = (param_3 & 0x7ffffffffffffff8) + 0x8000000000000008;
    param_1 = puVar2;
  }
  puVar2 = (undefined8 *)((long)param_1 + (param_3 + 1) * 2);
  if ((param_1 <= puVar2) && (puVar2 <= param_2 || param_2 < param_1)) {
    _memmove(param_1,param_2,(param_3 + 1) * 2);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x544bc);
  (*pcVar1)();
}


