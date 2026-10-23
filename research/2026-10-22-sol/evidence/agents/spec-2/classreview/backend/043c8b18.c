// ==== 0x43c8b18  FUN_043c8b18  (288 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_043c8b18(long param_1,long *param_2)

{
  long *plVar1;
  code *pcVar2;
  int iVar3;
  undefined8 uVar4;
  int *piVar5;
  long lVar6;
  long lVar7;
  long *local_40;
  long local_38;
  
  if (*(long *)(param_1 + 0x268) == 0) {
    if (*param_2 == 0) {
      lVar6 = 0;
    }
    else {
      _os_release(0);
      *(long *)(param_1 + 0x268) = *param_2;
      _os_retain();
      lVar6 = *param_2;
    }
    _os_retain(lVar6);
    local_40 = (long *)FUN_00008790(8);
    *local_40 = 0;
    *local_40 = lVar6;
    piVar5 = *(int **)(param_1 + 0x2a8);
    local_38 = lVar6;
    if ((piVar5 != (int *)0x0) && (iVar3 = *piVar5, *piVar5 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x43c8c38);
      (*pcVar2)();
    }
    lVar7 = 0;
    if (*(long *)(param_1 + 0x2b0) != 0) {
      lVar7 = *(long *)(param_1 + 0x2b0) + 8;
    }
    uVar4 = FUN_047359ec(&local_40,piVar5,lVar7);
    lVar7 = *(long *)(param_1 + 0x270);
    *(undefined8 *)(param_1 + 0x270) = uVar4;
    if (lVar7 != 0) {
      FUN_00005810(lVar7,0x10);
    }
    plVar1 = local_40;
    local_40 = (long *)0x0;
    if (plVar1 != (long *)0x0) {
      _objc_destroyWeak(plVar1);
      FUN_00005810(plVar1,8);
    }
    _os_release(lVar6);
  }
  return;
}


