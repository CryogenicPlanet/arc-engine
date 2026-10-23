// ==== 0x9f2536c  FUN_09f2536c  (204 bytes)

void FUN_09f2536c(long param_1)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 uVar3;
  long *plVar4;
  long lVar5;
  long lVar6;
  
  iVar2 = FUN_041c98e8(param_1 + 0x48);
  if (iVar2 == 0) {
    return;
  }
  uVar1 = *(undefined8 *)(param_1 + 0x20);
  uVar3 = FUN_03dabfb8(*(undefined8 *)(param_1 + 0x18));
  FUN_02ec6e70(uVar1,9,uVar3,*(undefined8 *)(param_1 + 0x170));
  FUN_0bf0ee2c(*(undefined8 *)(param_1 + 0x18),*(undefined4 *)(param_1 + 8));
  lVar6 = (long)*(char *)(param_1 + 0x67);
  if (lVar6 < 0) {
    lVar5 = *(long *)(param_1 + 0x50);
    lVar6 = *(long *)(param_1 + 0x58);
  }
  else {
    lVar5 = param_1 + 0x50;
  }
  FUN_09f21620(param_1 + 0x48,lVar5,lVar6);
  if (((*(long *)(param_1 + 0x30) == 0) || (*(char *)(*(long *)(param_1 + 0x30) + 4) != '\0')) ||
     (*(long *)(param_1 + 0x38) == 0)) {
    plVar4 = *(long **)(param_1 + 0x40);
  }
  else if ((*(long *)(param_1 + 0x30) == 0) || (*(char *)(*(long *)(param_1 + 0x30) + 4) != '\0')) {
    plVar4 = (long *)0x0;
  }
  else {
    plVar4 = *(long **)(param_1 + 0x38);
  }
  (**(code **)(*plVar4 + 0x68))();
                    /* WARNING: Could not recover jumptable at 0x09f2540c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(long **)(param_1 + 0x10) + 0x48))();
  return;
}


