// ==== 0x4376580  FUN_04376580  (376 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */

long FUN_04376580(long param_1)

{
  long *plVar1;
  code *pcVar2;
  int iVar3;
  long lVar4;
  ulong uVar5;
  int *piVar6;
  long *local_48;
  long *local_40;
  undefined8 local_38;
  
  lVar4 = param_1;
  if ((*(long *)(param_1 + 0x130) != 0) && (lVar4 = FUN_020cf42c(0x1d), (int)lVar4 != 0)) {
    local_38 = *(undefined8 *)(param_1 + 0x288);
    uVar5 = FUN_0473c9a8(&local_38);
    lVar4 = 0;
    if ((uVar5 & 1) == 0) {
      local_40 = (long *)0x0;
      (**(code **)(**(long **)(*(long *)(param_1 + 0x130) + 0xe8) + 0x10))(&local_40);
      lVar4 = 0;
      if (*local_40 != 0) {
        local_48 = (long *)FUN_00008790(8);
        *local_48 = 0;
        *local_48 = *local_40;
        piVar6 = *(int **)(param_1 + 0x610);
        if ((piVar6 != (int *)0x0) && (iVar3 = *piVar6, *piVar6 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x43766f8);
          (*pcVar2)();
        }
        lVar4 = 0;
        if (*(long *)(param_1 + 0x618) != 0) {
          lVar4 = *(long *)(param_1 + 0x618) + 0x68;
        }
        lVar4 = FUN_0473c4dc(&local_48,piVar6,lVar4);
        plVar1 = local_48;
        if (*(long *)(param_1 + 0x288) != lVar4) {
          *(long *)(param_1 + 0x288) = lVar4;
        }
        lVar4 = 0;
        local_48 = (long *)0x0;
        if (plVar1 != (long *)0x0) {
          _objc_destroyWeak(plVar1);
          lVar4 = FUN_00005810(plVar1,8);
        }
      }
      plVar1 = local_40;
      local_40 = (long *)0x0;
      if (plVar1 != (long *)0x0) {
        _objc_destroyWeak(plVar1);
        lVar4 = FUN_00005810(plVar1,8);
      }
    }
  }
  return lVar4;
}


