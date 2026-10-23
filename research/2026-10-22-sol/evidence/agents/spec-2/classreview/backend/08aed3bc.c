// ==== 0x8aed3bc  FUN_08aed3bc  (60 bytes)

undefined8 FUN_08aed3bc(long *param_1)

{
  long *plVar1;
  undefined8 uVar2;
  long lVar3;
  
  lVar3 = *param_1;
  if ((lVar3 != 0) && (*(long *)(lVar3 + 0x18) != 0)) {
    plVar1 = (long *)FUN_044bb428(param_1,*(undefined4 *)(lVar3 + 4),1);
                    /* WARNING: Could not recover jumptable at 0x08aed3ec. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (**(code **)(*plVar1 + 0x18))();
    return uVar2;
  }
  return 0;
}


