// ==== 0x42c8400  FUN_042c8400  (248 bytes)

void FUN_042c8400(undefined8 *param_1,long param_2)

{
  long lVar1;
  long *plVar2;
  bool bVar3;
  long lVar4;
  code *UNRECOVERED_JUMPTABLE;
  long *plVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  plVar5 = *(long **)(param_2 + 0x10);
  if (plVar5 != (long *)0x0) {
    uVar7 = *(undefined8 *)(param_2 + 0x10);
    uVar6 = *(undefined8 *)(param_2 + 8);
    lVar4 = plVar5[1];
    while (lVar4 != -1) {
      lVar1 = lVar4 + 1;
      plVar2 = plVar5 + 1;
      bVar3 = *plVar2 == lVar4;
      lVar4 = *plVar2;
      if (bVar3) {
        *plVar2 = lVar1;
        param_1[1] = uVar7;
        *param_1 = uVar6;
        *plVar2 = *plVar2 + 1;
        LOAcquire();
        lVar4 = *plVar2;
        *plVar2 = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) {
          return;
        }
        (**(code **)(*plVar5 + 0x10))(plVar5);
        if (plVar5[2] != 0) {
          LOAcquire();
          lVar4 = plVar5[2];
          plVar5[2] = lVar4 + -1;
          LORelease();
          if (lVar4 != 0) {
            return;
          }
        }
        UNRECOVERED_JUMPTABLE = *(code **)(*plVar5 + 0x20);
        if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
           (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x042c84f4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          (*UNRECOVERED_JUMPTABLE)(plVar5);
          return;
        }
        FUN_00005810(plVar5,0x20);
        return;
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_050d342c();
}


