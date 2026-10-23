// ==== 0x4440328  ArcMenuItemModel::setMenu_item_model_delegate_bridge:  (380 bytes)

/* Function Stack Size: 0x20 bytes */

void __thiscall
ArcMenuItemModel::setMenu_item_model_delegate_bridge_
          (ArcMenuItemModel *this,ID param_1,SEL param_2,
          shared_ptr<ArcMenuItemModelDelegateBridge> param_3)

{
  long lVar1;
  ID extraout_x1;
  ID extraout_x1_00;
  code *UNRECOVERED_JUMPTABLE;
  ID extraout_x1_01;
  ArcMenuItemModelDelegateBridge *pAVar2;
  long *plVar3;
  
  pAVar2 = param_3.field0_0x0;
  if (pAVar2 != (ArcMenuItemModelDelegateBridge *)0x0) {
    *(long *)(pAVar2 + 8) = *(long *)(pAVar2 + 8) + 1;
  }
  plVar3 = *(long **)(this + 0x30);
  *(SEL *)(this + 0x28) = param_2;
  *(ArcMenuItemModelDelegateBridge **)(this + 0x30) = pAVar2;
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        param_1 = extraout_x1;
        if (lVar1 != 0) goto LAB_04440368;
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar3 + 0x20);
      if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
         (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
        param_1 = extraout_x1_00;
      }
      else {
        (*UNRECOVERED_JUMPTABLE)(plVar3);
        param_1 = extraout_x1_01;
      }
    }
  }
LAB_04440368:
  if (pAVar2 != (ArcMenuItemModelDelegateBridge *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(pAVar2 + 8);
    *(long *)(pAVar2 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)pAVar2 + 0x10))(pAVar2,param_1);
      if (*(long *)(pAVar2 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(pAVar2 + 0x10);
        *(long *)(pAVar2 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*(long *)pAVar2 + 0x20);
      if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
         (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x044404a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*UNRECOVERED_JUMPTABLE)(pAVar2);
        return;
      }
      FUN_00005810(pAVar2,0x20);
      return;
    }
  }
  return;
}


