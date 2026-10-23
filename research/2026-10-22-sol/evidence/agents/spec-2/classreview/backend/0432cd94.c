// ==== 0x432cd94  FUN_0432cd94  (112 bytes)

void FUN_0432cd94(long *param_1,long param_2)

{
  undefined8 *puVar1;
  long *plVar2;
  
  if (param_2 != 0) {
    puVar1 = (undefined8 *)FUN_00008790(0x30);
    *puVar1 = &PTR_FUN_0da7e4e8;
    puVar1[1] = &PTR_LAB_0da7e550;
    puVar1[5] = 0;
    puVar1[4] = 0;
    puVar1[2] = param_2;
    puVar1[3] = puVar1 + 4;
    plVar2 = (long *)*param_1;
    *param_1 = (long)puVar1;
    if (plVar2 != (long *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0432cdf4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*plVar2 + 8))();
      return;
    }
  }
  return;
}


