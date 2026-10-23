// ==== 0x435c74c  FUN_0435c74c  (408 bytes)

void FUN_0435c74c(ulong param_1)

{
  ulong *puVar1;
  uint uVar2;
  int iVar3;
  code *UNRECOVERED_JUMPTABLE;
  long lVar4;
  ulong *puVar5;
  ulong *puVar6;
  ulong uVar7;
  ulong uVar8;
  long *plVar9;
  ulong local_40;
  undefined8 local_38;
  undefined7 local_28;
  char cStack_21;
  
  plVar9 = *(long **)(param_1 + 0x28);
  if (plVar9 != (long *)0x0) {
    *(undefined8 *)(param_1 + 0x28) = 0;
    puVar1 = (ulong *)plVar9[8];
    lVar4 = plVar9[9] << 4;
    puVar6 = puVar1;
    if (lVar4 != 0) {
      uVar7 = lVar4 >> 4;
      puVar5 = puVar1;
      do {
        uVar8 = uVar7 >> 1;
        puVar6 = puVar5 + uVar8 * 2 + 2;
        uVar7 = uVar7 + ~(uVar7 >> 1);
        if (param_1 <= puVar5[uVar8 * 2]) {
          puVar6 = puVar5;
          uVar7 = uVar8;
        }
        puVar5 = puVar6;
      } while (uVar7 != 0);
    }
    if (((puVar6 != puVar1 + plVar9[9] * 2) && (*puVar6 <= param_1)) && (puVar6[1] != 0)) {
      uVar2 = *(uint *)(plVar9 + 1);
      *(uint *)(plVar9 + 1) = uVar2 + 1;
      if (0xfffffffe < uVar2) {
                    /* WARNING: Does not return */
        UNRECOVERED_JUMPTABLE = (code *)SoftwareBreakpoint(0,0x435c8c8);
        (*UNRECOVERED_JUMPTABLE)();
      }
      FUN_020cb924(plVar9);
      lVar4 = plVar9[1];
      iVar3 = (int)lVar4 + -1;
      *(int *)(plVar9 + 1) = iVar3;
      if ((int)lVar4 == 0) {
                    /* WARNING: Does not return */
        UNRECOVERED_JUMPTABLE = (code *)SoftwareBreakpoint(0,0x435c8d4);
        (*UNRECOVERED_JUMPTABLE)();
      }
      if (iVar3 == 0) {
        (**(code **)(*plVar9 + 0xe8))(plVar9);
      }
    }
    lVar4 = plVar9[1];
    iVar3 = (int)lVar4 + -1;
    *(int *)(plVar9 + 1) = iVar3;
    if ((int)lVar4 == 0) {
                    /* WARNING: Does not return */
      UNRECOVERED_JUMPTABLE = (code *)SoftwareBreakpoint(0,0x435c8e0);
      (*UNRECOVERED_JUMPTABLE)();
    }
    if (iVar3 == 0) {
      (**(code **)(*plVar9 + 0xe8))(plVar9);
    }
  }
  if ((*(byte *)(*(long *)(param_1 + 0x20) + 0x10) & 1) == 0) {
    local_40 = FUN_04b37104(0x39,"DevTools connection closed",0x1a,0x4a,
                            "../../arc/browser/web_contents/devtools_impl.cc");
    FUN_0435c8fc(param_1,&local_40);
    if (local_40 == 1) {
      if (cStack_21 < '\0') {
        FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
      }
    }
    else if ((local_40 & 1) == 0) {
      FUN_04b35898();
    }
    lVar4 = *(long *)(param_1 + 0x20);
    *(undefined8 *)(param_1 + 0x18) = 0;
    *(undefined8 **)(param_1 + 0x20) = &DAT_0c4b3500;
    UNRECOVERED_JUMPTABLE = *(code **)(lVar4 + 8);
    if (UNRECOVERED_JUMPTABLE != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0435c8b8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*UNRECOVERED_JUMPTABLE)(*(undefined8 *)(param_1 + 8));
      return;
    }
  }
  return;
}


