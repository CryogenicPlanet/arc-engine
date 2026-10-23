// ==== 0x4341e44  FUN_04341e44  (172 bytes)

void FUN_04341e44(char *param_1)

{
  char *pcVar1;
  code *pcVar2;
  long lVar3;
  size_t sVar4;
  char *pcVar5;
  
  lVar3 = FUN_083a7e78();
  pcVar1 = (char *)(lVar3 + 0x2e);
  sVar4 = _strlen(pcVar1);
  if (0x7fffffffffffffee < sVar4) {
                    /* WARNING: Subroutine does not return */
    FUN_04b089c0();
  }
  if (sVar4 < 0x17) {
    param_1[0x17] = (char)sVar4;
    pcVar5 = param_1;
  }
  else {
    pcVar5 = (char *)FUN_00008790((sVar4 & 0x7ffffffffffffff0) + 0x10);
    *(char **)param_1 = pcVar5;
    *(size_t *)(param_1 + 8) = sVar4;
    *(size_t *)(param_1 + 0x10) = (sVar4 & 0x7ffffffffffffff0) + 0x8000000000000010;
  }
  if ((pcVar5 <= pcVar5 + sVar4) && (pcVar5 + sVar4 <= pcVar1 || pcVar1 < pcVar5)) {
    _memmove(pcVar5,pcVar1,sVar4);
    pcVar5[sVar4] = '\0';
    return;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4341eec);
  (*pcVar2)();
}


