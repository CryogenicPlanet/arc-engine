// ==== 0x4335e08  FUN_04335e08  (324 bytes)

void FUN_04335e08(long param_1)

{
  char cVar1;
  code *pcVar2;
  int iVar3;
  int *piVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  
  if ((((*(char *)(param_1 + 0x1c) == '\0') || (*(long *)(param_1 + 0x30) == 0)) ||
      (*(char *)(*(long *)(param_1 + 0x30) + 4) != '\0')) || (*(long *)(param_1 + 0x38) == 0))
  goto LAB_04335e30;
  if ((*(byte *)(param_1 + 0x1c) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x4335f40);
    (*pcVar2)();
  }
  if (*(long *)(param_1 + 0x30) == 0) {
    uVar5 = 0;
    uVar6 = 0;
    if (*(int *)(param_1 + 0x18) != 0) goto LAB_04335ef0;
LAB_04335f34:
    FUN_09fc7f3c(*(undefined8 *)(param_1 + 0x20),uVar5);
  }
  else {
    cVar1 = *(char *)(*(long *)(param_1 + 0x30) + 4);
    if (*(int *)(param_1 + 0x18) == 0) {
      if (cVar1 == '\0') {
        uVar5 = *(undefined8 *)(param_1 + 0x38);
      }
      else {
        uVar5 = 0;
      }
      goto LAB_04335f34;
    }
    if (cVar1 == '\0') {
      uVar6 = *(undefined8 *)(param_1 + 0x38);
    }
    else {
      uVar6 = 0;
    }
LAB_04335ef0:
    FUN_0459a568(*(undefined8 *)(param_1 + 0x20),uVar6,0x15);
  }
  FUN_020da370(*(long *)(param_1 + 0x40) + 0x150,param_1);
LAB_04335e30:
  if (*(char *)(param_1 + 0x1c) != '\0') {
    *(undefined1 *)(param_1 + 0x1c) = 0;
  }
  piVar4 = *(int **)(param_1 + 0x30);
  *(undefined8 *)(param_1 + 0x30) = 0;
  if (piVar4 != (int *)0x0) {
    LOAcquire();
    iVar3 = *piVar4;
    *piVar4 = iVar3 + -1;
    LORelease();
    if (iVar3 == 1) {
      FUN_00005810(piVar4,8);
    }
  }
  *(undefined8 *)(param_1 + 0x38) = 0;
  if ((*(ulong *)(param_1 + 0x20) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  *(undefined8 *)(param_1 + 0x20) = 0;
  if ((*(ulong *)(param_1 + 0x28) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  *(undefined8 *)(param_1 + 0x28) = 0;
  if ((*(ulong *)(param_1 + 0x40) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  *(undefined8 *)(param_1 + 0x40) = 0;
  return;
}


