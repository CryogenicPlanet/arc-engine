// ==== 0x4443f3c  ArcWebContents::setWeb_contents_navigation_delegate_bridge:  (380 bytes)

/* Function Stack Size: 0x20 bytes */

void __thiscall
ArcWebContents::setWeb_contents_navigation_delegate_bridge_
          (ArcWebContents *this,ID param_1,SEL param_2,
          shared_ptr<ArcWebContentsNavigationDelegateBridge> param_3)

{
  long lVar1;
  ID extraout_x1;
  ID extraout_x1_00;
  code *UNRECOVERED_JUMPTABLE;
  ID extraout_x1_01;
  ArcWebContentsNavigationDelegateBridge *pAVar2;
  long *plVar3;
  
  pAVar2 = param_3.field0_0x0;
  if (pAVar2 != (ArcWebContentsNavigationDelegateBridge *)0x0) {
    *(long *)(pAVar2 + 8) = *(long *)(pAVar2 + 8) + 1;
  }
  plVar3 = *(long **)(this + 0xa0);
  *(SEL *)(this + 0x98) = param_2;
  *(ArcWebContentsNavigationDelegateBridge **)(this + 0xa0) = pAVar2;
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
        if (lVar1 != 0) goto LAB_04443f7c;
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
LAB_04443f7c:
  if (pAVar2 != (ArcWebContentsNavigationDelegateBridge *)0x0) {
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
                    /* WARNING: Could not recover jumptable at 0x044440b4. Too many branches */
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


