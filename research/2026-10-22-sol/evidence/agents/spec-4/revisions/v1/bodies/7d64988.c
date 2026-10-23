// ==== 0x7d64988  FUN_07d64988  (228 bytes)

void FUN_07d64988(long param_1)

{
  code *pcVar1;
  int iVar2;
  undefined8 uVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  undefined1 auStack_80 [80];
  
  iVar2 = FUN_040991c0();
  if ((iVar2 != 1) && (iVar2 = FUN_040991c0(param_1), iVar2 != 7)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x7d64a70);
    (*pcVar1)();
  }
  FUN_07d6427c(param_1,1);
  iVar2 = FUN_01460670(param_1 + 0x10);
  if ((iVar2 != 0) && (*(long *)(param_1 + 0x18) != 0)) {
    uVar3 = FUN_07d64470(param_1);
    FUN_07d64260(auStack_80,param_1);
    FUN_07d64a84(uVar3,auStack_80);
    FUN_018af424(auStack_80);
    uVar4 = FUN_01460670(param_1 + 0x10);
    if ((uVar4 & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x7d64a7c);
      (*pcVar1)();
    }
    lVar7 = *(long *)(param_1 + 0x18);
    lVar5 = FUN_04081580(param_1);
    lVar6 = FUN_04081580(param_1);
    FUN_078b2e84(lVar7 + -0x50,lVar5 + 0x270,lVar6 + 0x2e0);
    uVar4 = FUN_01460670(param_1 + 0x10);
    if ((uVar4 & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x7d64a64);
      (*pcVar1)();
    }
  }
  return;
}


