// ==== 0x433487c  FUN_0433487c  (168 bytes)

void FUN_0433487c(long param_1)

{
  long lVar1;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  
  if (*(long *)(param_1 + 0x20) != 0) {
    if ((((*(char *)(*(long *)(param_1 + 0x20) + 4) == '\0') && (*(long *)(param_1 + 0x28) != 0)) &&
        (*(long *)(param_1 + 0x20) != 0)) &&
       ((*(char *)(*(long *)(param_1 + 0x20) + 4) == '\0' &&
        (lVar1 = *(long *)(param_1 + 0x28), lVar1 != 0)))) {
      FUN_07c72f90(lVar1);
      lVar1 = FUN_000805e0(lVar1 + 8,&DAT_0c9c03dc);
      if ((lVar1 != 0) && (lVar1 = lVar1 + -0x48, lVar1 != 0)) {
        uStack_38 = 0;
        local_40 = 0;
        uStack_28 = 0;
        uStack_30 = 0;
        uStack_58 = 0;
        local_60 = 0;
        uStack_48 = 0;
        uStack_50 = 0;
        uStack_78 = 0;
        local_80 = 0;
        uStack_68 = 0;
        uStack_70 = 0;
        FUN_07c7d4cc(lVar1,&local_80);
        FUN_0093fa9c(&local_80);
        FUN_07c7dca0(lVar1,1);
      }
    }
  }
  return;
}


