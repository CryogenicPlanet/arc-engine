// ==== 0x7d0cf8  FUN_007d0cf8  (180 bytes)

void FUN_007d0cf8(char *param_1,long param_2)

{
  ulong uVar1;
  int *piVar2;
  byte bVar3;
  code *pcVar4;
  undefined1 uStack_11;
  
  if (*(char *)(param_2 + 0x48) != '\0') {
    param_1[0x17] = '\x04';
    if (param_1 + 4 <= "null" || "null" < param_1) {
      builtin_strncpy(param_1,"null",5);
      return;
    }
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x7d0dac);
    (*pcVar4)();
  }
  bVar3 = *(byte *)(param_2 + 0x1f);
  uVar1 = *(ulong *)(param_2 + 0x10);
  if (-1 < (char)bVar3) {
    uVar1 = (ulong)bVar3;
  }
  if (uVar1 == 4) {
    piVar2 = *(int **)(param_2 + 8);
    if (-1 < (char)bVar3) {
      piVar2 = (int *)(param_2 + 8);
    }
    if (*piVar2 == 0x656c6966) {
      FUN_00b89b80(param_1,"file://",&uStack_11);
      return;
    }
  }
  FUN_007d0dac();
  return;
}


