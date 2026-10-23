// ==== 0x4346100  FUN_04346100  (744 bytes)

void FUN_04346100(undefined1 (*param_1) [16],long param_2,undefined8 *param_3)

{
  long lVar1;
  long lVar2;
  bool bVar3;
  long lVar4;
  code *UNRECOVERED_JUMPTABLE;
  long lVar5;
  long *plVar6;
  long *plVar7;
  undefined1 auVar8 [16];
  undefined8 local_70;
  undefined1 uStack_68;
  undefined7 uStack_67;
  undefined1 uStack_60;
  undefined8 uStack_5f;
  
  lVar5 = param_2;
  if (param_2 == 0) goto LAB_04346230;
  do {
    lVar4 = lVar5;
    lVar5 = *(long *)(lVar4 + 0x1e0);
  } while (lVar5 != 0);
  lVar5 = FUN_000805e0(lVar4 + 8,&DAT_0e4a2f38);
  if ((lVar5 == 0) || (plVar7 = *(long **)(lVar5 + 0x10), plVar7 == (long *)0x0)) goto LAB_04346230;
  lVar4 = plVar7[1];
  do {
    if (lVar4 == -1) goto LAB_04346230;
    lVar1 = lVar4 + 1;
    lVar2 = plVar7[1];
    bVar3 = lVar2 != lVar4;
    lVar4 = lVar2;
  } while (bVar3);
  plVar7[1] = lVar1;
  lVar5 = *(long *)(lVar5 + 8);
  if (lVar5 == 0) {
    plVar6 = (long *)0x0;
  }
  else {
    plVar7[1] = plVar7[1] + 1;
    plVar6 = plVar7;
  }
  LOAcquire();
  lVar4 = plVar7[1];
  plVar7[1] = lVar4 + -1;
  LORelease();
  if (lVar4 == 0) {
    (**(code **)(*plVar7 + 0x10))(plVar7);
    if (plVar7[2] != 0) {
      LOAcquire();
      lVar4 = plVar7[2];
      plVar7[2] = lVar4 + -1;
      LORelease();
      if (lVar4 != 0) goto LAB_043461ac;
    }
    UNRECOVERED_JUMPTABLE = *(code **)(*plVar7 + 0x20);
    if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
       (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
      FUN_00005810(plVar7,0x20);
    }
    else {
      (*UNRECOVERED_JUMPTABLE)(plVar7);
    }
  }
LAB_043461ac:
  if (lVar5 != 0) {
    *(long *)*param_1 = lVar5;
    *(long **)(*param_1 + 8) = plVar6;
    if (plVar6 == (long *)0x0) {
      return;
    }
    plVar7 = plVar6 + 1;
    *plVar7 = *plVar7 + 1;
    LOAcquire();
    lVar5 = *plVar7;
    *plVar7 = lVar5 + -1;
    LORelease();
    if (lVar5 != 0) {
      return;
    }
    (**(code **)(*plVar6 + 0x10))(plVar6);
    if (plVar6[2] != 0) {
      LOAcquire();
      lVar5 = plVar6[2];
      plVar6[2] = lVar5 + -1;
      LORelease();
      if (lVar5 != 0) {
        return;
      }
    }
    UNRECOVERED_JUMPTABLE = *(code **)(*plVar6 + 0x20);
    if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
       (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x043463e4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*UNRECOVERED_JUMPTABLE)(plVar6);
      return;
    }
    FUN_00005810(plVar6,0x20);
    return;
  }
LAB_04346230:
  local_70 = *param_3;
  uStack_68 = (undefined1)param_3[1];
  uStack_5f = *(undefined8 *)((long)param_3 + 0x11);
  uStack_67 = (undefined7)*(undefined8 *)((long)param_3 + 9);
  uStack_60 = (undefined1)((ulong)*(undefined8 *)((long)param_3 + 9) >> 0x38);
  auVar8 = FUN_0436edd0(param_2,&local_70);
  plVar7 = auVar8._8_8_;
  *param_1 = auVar8;
  if (plVar7 != (long *)0x0) {
    plVar6 = plVar7 + 1;
    *plVar6 = *plVar6 + 1;
    LOAcquire();
    lVar5 = *plVar6;
    *plVar6 = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar7 + 0x10))(plVar7);
      if (plVar7[2] != 0) {
        LOAcquire();
        lVar5 = plVar7[2];
        plVar7[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) {
          return;
        }
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar7 + 0x20);
      if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
         (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
        FUN_00005810(plVar7,0x20);
      }
      else {
        (*UNRECOVERED_JUMPTABLE)(plVar7);
      }
    }
  }
  return;
}


