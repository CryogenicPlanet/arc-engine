// ==== 0x430dfbc  FUN_0430dfbc  (524 bytes)

void FUN_0430dfbc(undefined8 *param_1,long param_2)

{
  int iVar1;
  code *pcVar2;
  long lVar3;
  undefined8 *puVar4;
  long lVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 local_98 [2];
  undefined7 local_88;
  char cStack_81;
  undefined1 auStack_78 [80];
  undefined1 auStack_28 [8];
  
  lVar3 = (**(code **)(**(long **)(param_2 + 0x18) + 0xf8))();
  if (lVar3 == 0) {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    return;
  }
  (**(code **)(**(long **)(param_2 + 0x18) + 0xf8))();
  lVar3 = FUN_0647a03c();
  if (lVar3 == 0) {
    (**(code **)(**(long **)(param_2 + 0x18) + 0x1b8))(local_98);
    puVar4 = (undefined8 *)FUN_007cf864(local_98);
    if (*(char *)((long)puVar4 + 0x17) < '\0') {
      FUN_00060a60(param_1,*puVar4,puVar4[1]);
    }
    else {
      uVar7 = puVar4[1];
      uVar6 = *puVar4;
      param_1[2] = puVar4[2];
      param_1[1] = uVar7;
      *param_1 = uVar6;
    }
    FUN_000282bc(auStack_28,0);
    FUN_0002827c(auStack_78);
    if (-1 < cStack_81) {
      return;
    }
    FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
    return;
  }
  lVar5 = *(long *)(lVar3 + 0x290);
  if (lVar5 == 0) {
LAB_0430e020:
    iVar1 = *(int *)(lVar3 + 0x2b4);
    if (*(undefined1 **)(*(long *)(lVar3 + 0x260) + 0x38) == &LAB_0414e5d8) {
      if (iVar1 == -1) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x430e1c8);
        (*pcVar2)();
      }
    }
    else if (iVar1 == -1) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x430e18c);
      (*pcVar2)();
    }
    if (*(ulong *)(lVar3 + 0x280) <= (ulong)(long)iVar1) {
LAB_0430e1b8:
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x430e1bc);
      (*pcVar2)();
    }
    lVar5 = *(long *)(*(long *)(lVar3 + 0x278) + (long)iVar1 * 8);
    if (lVar5 == 0) {
      lVar5 = FUN_030bc34c();
      goto LAB_0430e074;
    }
  }
  else if (*(int *)(lVar3 + 0x2b8) == -1) {
    if (*(char *)(lVar5 + 0x348) != '\0') {
      if (*(char *)(lVar3 + 0x2e8) != '\0') {
        iVar1 = *(int *)(lVar3 + 0x2b4);
        if (iVar1 == -1) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x430e1d4);
          (*pcVar2)();
        }
        if (*(ulong *)(lVar3 + 0x280) <= (ulong)(long)iVar1) goto LAB_0430e1b8;
        if (*(int *)(*(long *)(*(long *)(lVar3 + 0x278) + (long)iVar1 * 8) + 0x430) != 2) {
          if (*(long *)(lVar3 + 0x268) == 0) {
                    /* WARNING: Does not return */
            pcVar2 = (code *)SoftwareBreakpoint(0,0x430e1e0);
            (*pcVar2)();
          }
          if ((*(byte *)(*(long *)(lVar3 + 0x268) + 0x160) & 1) == 0) goto LAB_0430e05c;
        }
      }
      goto LAB_0430e020;
    }
  }
  else if ((*(char *)(lVar3 + 0x2e8) == '\0') || (*(char *)(lVar5 + 0x348) != '\0'))
  goto LAB_0430e020;
LAB_0430e05c:
  if (*(char *)(lVar5 + 0x3f) < '\0') {
    if (*(long *)(lVar5 + 0x30) != 0) goto LAB_0430e19c;
  }
  else if (*(char *)(lVar5 + 0x3f) != '\0') {
LAB_0430e19c:
    lVar5 = lVar5 + 0x28;
    goto LAB_0430e074;
  }
  lVar5 = *(long *)(*(long *)(lVar5 + 0x18) + 8) + 0x60;
LAB_0430e074:
  puVar4 = (undefined8 *)FUN_007cf864(lVar5);
  if (-1 < *(char *)((long)puVar4 + 0x17)) {
    uVar7 = puVar4[1];
    uVar6 = *puVar4;
    param_1[2] = puVar4[2];
    param_1[1] = uVar7;
    *param_1 = uVar6;
    return;
  }
  FUN_00060a60(param_1,*puVar4,puVar4[1]);
  return;
}


