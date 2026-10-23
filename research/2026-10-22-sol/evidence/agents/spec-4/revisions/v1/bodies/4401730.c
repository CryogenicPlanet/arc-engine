// ==== 0x4401730  ArcBrowserSidePanel::setNativeSidePanel:  (216 bytes)

/* Function Stack Size: 0x20 bytes */

void ArcBrowserSidePanel::setNativeSidePanel_(ID param_1,SEL param_2,SidePanel param_3)

{
  long lVar1;
  code *UNRECOVERED_JUMPTABLE;
  Impl *pIVar2;
  long *plVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  
  pIVar2 = param_3.field0_0x0.field0_0x0;
  uVar5 = *(undefined8 *)(pIVar2 + 8);
  uVar4 = *(undefined8 *)pIVar2;
  if (*(long *)(pIVar2 + 8) != 0) {
    plVar3 = (long *)(*(long *)(pIVar2 + 8) + 8);
    *plVar3 = *plVar3 + 1;
  }
  plVar3 = *(long **)(param_1 + 0x10);
  *(undefined8 *)(param_1 + 0x10) = uVar5;
  *(undefined8 *)(param_1 + 8) = uVar4;
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3,param_2,pIVar2,param_3.field0_0x0.field1_0x8);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar3 + 0x20);
      if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
         (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x04401804. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*UNRECOVERED_JUMPTABLE)(plVar3);
        return;
      }
      FUN_00005810(plVar3,0x20);
      return;
    }
  }
  return;
}


