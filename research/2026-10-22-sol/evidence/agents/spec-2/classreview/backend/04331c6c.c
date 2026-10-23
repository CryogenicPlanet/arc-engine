// ==== 0x4331c6c  FUN_04331c6c  (204 bytes)

void FUN_04331c6c(char *param_1,long *param_2)

{
  code *pcVar1;
  size_t sVar2;
  char *pcVar3;
  char *pcVar4;
  
  if (10 < (*(uint *)(*param_2 + 0x3f0) & 0xff)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x4331d38);
    (*pcVar1)();
  }
  pcVar4 = (&PTR_s_link_0df6aee8)[(ulong)*(uint *)(*param_2 + 0x3f0) & 0xf];
  sVar2 = _strlen(pcVar4);
  if (sVar2 < 0x7fffffffffffffef) {
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
    if ((pcVar3 <= pcVar3 + sVar2) && (pcVar4 < pcVar3 || pcVar3 + sVar2 <= pcVar4)) {
      _memcpy(pcVar3,pcVar4,sVar2);
      pcVar3[sVar2] = '\0';
      return;
    }
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4331d30);
    (*pcVar1)();
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b089c0();
}


