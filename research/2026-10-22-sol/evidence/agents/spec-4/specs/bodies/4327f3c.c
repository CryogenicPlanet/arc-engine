// ==== 0x4327f3c  FUN_04327f3c  (224 bytes)

void FUN_04327f3c(undefined8 *param_1,uint *param_2,undefined8 param_3)

{
  code *pcVar1;
  int iVar2;
  size_t sVar3;
  long lVar4;
  int *piVar5;
  char *pcVar6;
  undefined1 auStack_50 [32];
  
  if (8 < *param_2) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x432801c);
    (*pcVar1)();
  }
  pcVar6 = (&PTR_s_dia_airestricteddomainslist_0df6ace8)[*param_2];
  sVar3 = _strlen(pcVar6);
  lVar4 = FUN_005b4234(param_3,pcVar6,sVar3);
  if (lVar4 == 0) {
    piVar5 = (int *)FUN_04b37104(0x15,"Preference not found",0x14,0x42,
                                 "../../arc/browser/local_state_impl.cc");
    *param_1 = piVar5;
    if (((ulong)piVar5 & 1) == 0) {
      iVar2 = *piVar5;
      *piVar5 = iVar2 + 1;
      FUN_04b35898(iVar2);
      return;
    }
  }
  else {
    FUN_01860668();
    FUN_04327bc8(auStack_50);
    *(undefined1 *)(param_1 + 1) = 0;
    *(undefined1 *)(param_1 + 4) = 0xff;
    FUN_0433bba8(param_1 + 1,auStack_50);
    *param_1 = 1;
    FUN_0433b0ec(auStack_50);
  }
  return;
}


