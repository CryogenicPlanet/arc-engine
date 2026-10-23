// ==== 0x22f2830  FUN_022f2830  (232 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

long * FUN_022f2830(long param_1,undefined8 *param_2)

{
  code *pcVar1;
  long *plVar2;
  long *plVar3;
  long local_58;
  long *local_50;
  long local_48;
  
  **(undefined8 **)(param_1 + 0x78) = *(undefined8 *)*param_2;
  plVar2 = (long *)FUN_022f2940(&local_58,param_1);
  for (plVar3 = local_50; plVar3 != &local_58; plVar3 = (long *)plVar3[1]) {
    FUN_022f2a74(plVar3[2],**(undefined8 **)(param_1 + 0x78));
    plVar2 = (long *)0x0;
  }
  if (local_48 != 0) {
    plVar2 = *(long **)(local_58 + 8);
    *(long **)(*local_50 + 8) = plVar2;
    *plVar2 = *local_50;
    local_48 = 0;
    plVar2 = local_50;
    while (plVar2 != &local_58) {
      if ((plVar2 == (long *)0xfffffffffffffff0) || (plVar2 == (long *)0x0)) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(1,0x22f2918);
        (*pcVar1)();
      }
      plVar3 = (long *)plVar2[1];
      FUN_00005810(plVar2,0x18);
      plVar2 = plVar3;
    }
  }
  return plVar2;
}


