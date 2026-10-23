// ==== 0x433190c  FUN_0433190c  (204 bytes)

void FUN_0433190c(char *param_1,undefined8 *param_2)

{
  code *pcVar1;
  char *pcVar2;
  size_t sVar3;
  char *pcVar4;
  
  if (*(long *)*param_2 == 0) {
    pcVar2 = "";
  }
  else {
    pcVar2 = (char *)FUN_020cdf74(*(undefined4 *)(*(long *)*param_2 + 0x94));
    if (pcVar2 == (char *)0x0) goto LAB_043319d0;
  }
  sVar3 = _strlen(pcVar2);
  if (0x7fffffffffffffee < sVar3) {
                    /* WARNING: Subroutine does not return */
    FUN_04b089c0();
  }
  if (sVar3 < 0x17) {
    param_1[0x17] = (char)sVar3;
    pcVar4 = param_1;
  }
  else {
    pcVar4 = (char *)FUN_00008790((sVar3 & 0x7ffffffffffffff0) + 0x10);
    *(char **)param_1 = pcVar4;
    *(size_t *)(param_1 + 8) = sVar3;
    *(size_t *)(param_1 + 0x10) = (sVar3 & 0x7ffffffffffffff0) + 0x8000000000000010;
  }
  if ((pcVar4 <= pcVar4 + sVar3) && (pcVar2 < pcVar4 || pcVar4 + sVar3 <= pcVar2)) {
    _memmove(pcVar4,pcVar2,sVar3);
    pcVar4[sVar3] = '\0';
    return;
  }
LAB_043319d0:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43319d4);
  (*pcVar1)();
}


