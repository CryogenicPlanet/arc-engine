// ==== 0xb89b80  FUN_00b89b80  (188 bytes)

char * FUN_00b89b80(char *param_1,char *param_2)

{
  code *pcVar1;
  size_t sVar2;
  char *pcVar3;
  
  if (param_2 != (char *)0x0) {
    sVar2 = _strlen(param_2);
    if (0x7fffffffffffffee < sVar2) {
                    /* WARNING: Subroutine does not return */
      FUN_04b089c0();
    }
    if (sVar2 < 0x17) {
      param_1[0x17] = (char)sVar2;
      pcVar3 = param_1;
    }
    else {
      pcVar3 = (char *)FUN_00008790((sVar2 & 0x7ffffffffffffff0) + 0x10);
      *(char **)param_1 = pcVar3;
      *(size_t *)(param_1 + 8) = sVar2;
      *(size_t *)(param_1 + 0x10) = (sVar2 & 0x7ffffffffffffff0) + 0x8000000000000010;
    }
    if ((pcVar3 <= pcVar3 + sVar2) && (pcVar3 + sVar2 <= param_2 || param_2 < pcVar3)) {
      _memmove(pcVar3,param_2,sVar2);
      pcVar3[sVar2] = '\0';
      return param_1;
    }
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0xb89c38);
  (*pcVar1)();
}


