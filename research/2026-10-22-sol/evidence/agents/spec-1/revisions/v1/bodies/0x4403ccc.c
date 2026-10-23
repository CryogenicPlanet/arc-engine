// ==== 0x4403ccc  ArcHungRendererDialog::setDialog:  (464 bytes)

/* Function Stack Size: 0x28 bytes */

void ArcHungRendererDialog::setDialog_
               (ID param_1,SEL param_2,optional<arc::HungRendererDialog> *param_3)

{
  char cVar1;
  long lVar2;
  __shared_weak_count *p_Var3;
  code *UNRECOVERED_JUMPTABLE;
  long *plVar4;
  Impl *pIVar5;
  __shared_weak_count *p_Var6;
  
  cVar1 = *(char *)(param_1 + 0x18);
  if (cVar1 == (char)param_3->field1_0x10) {
    if (cVar1 == '\0') {
      return;
    }
    p_Var3 = (param_3->field0_0x0).field1.field0_0x0.field1_0x8;
    p_Var6 = (param_3->field0_0x0).field1.field0_0x0.field1_0x8;
    pIVar5 = (param_3->field0_0x0).field1.field0_0x0.field0_0x0;
    if (p_Var3 != (__shared_weak_count *)0x0) {
      p_Var3 = p_Var3 + 8;
      *(long *)p_Var3 = *(long *)p_Var3 + 1;
    }
    plVar4 = *(long **)(param_1 + 0x10);
    *(__shared_weak_count **)(param_1 + 0x10) = p_Var6;
    *(Impl **)(param_1 + 8) = pIVar5;
    if (plVar4 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar2 = plVar4[1];
    plVar4[1] = lVar2 + -1;
    LORelease();
    if (lVar2 != 0) {
      return;
    }
    (**(code **)(*plVar4 + 0x10))(plVar4);
    if (plVar4[2] != 0) {
      LOAcquire();
      lVar2 = plVar4[2];
      plVar4[2] = lVar2 + -1;
      LORelease();
      if (lVar2 != 0) {
        return;
      }
    }
    UNRECOVERED_JUMPTABLE = *(code **)(*plVar4 + 0x20);
    if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
       (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x04403e98. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*UNRECOVERED_JUMPTABLE)(plVar4);
      return;
    }
    FUN_00005810(plVar4,0x20);
    return;
  }
  if (cVar1 == '\0') {
    *(Impl **)(param_1 + 8) = (param_3->field0_0x0).field1.field0_0x0.field0_0x0;
    p_Var3 = (param_3->field0_0x0).field1.field0_0x0.field1_0x8;
    *(__shared_weak_count **)(param_1 + 0x10) = p_Var3;
    if (p_Var3 != (__shared_weak_count *)0x0) {
      *(long *)(p_Var3 + 8) = *(long *)(p_Var3 + 8) + 1;
    }
    *(undefined1 *)(param_1 + 0x18) = 1;
    return;
  }
  plVar4 = *(long **)(param_1 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar2 = plVar4[1];
    plVar4[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar2 = plVar4[2];
        plVar4[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_04403d0c;
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar4 + 0x20);
      if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
         (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*UNRECOVERED_JUMPTABLE)(plVar4);
      }
    }
  }
LAB_04403d0c:
  *(undefined1 *)(param_1 + 0x18) = 0;
  return;
}


