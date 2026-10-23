// ==== 0x433e240  FUN_0433e240  (248 bytes)

void FUN_0433e240(undefined8 param_1,long param_2,undefined8 *param_3)

{
  undefined1 *local_b0;
  long lStack_a8;
  undefined7 local_a0;
  char cStack_99;
  char *local_98;
  undefined8 uStack_90;
  char *local_88;
  undefined8 uStack_80;
  undefined1 *local_78;
  long lStack_70;
  char *local_68;
  undefined8 uStack_60;
  undefined8 *local_58;
  long lStack_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  local_98 = "i";
  if (*(char *)(param_2 + 0x48) == '\0') {
    local_98 = "r";
  }
  uStack_90 = 1;
  local_88 = "|";
  uStack_80 = 1;
  FUN_02d7a2f4(&local_b0);
  local_78 = local_b0;
  if (-1 < (long)cStack_99) {
    local_78 = (undefined1 *)&local_b0;
  }
  lStack_70 = lStack_a8;
  if (-1 < cStack_99) {
    lStack_70 = (long)cStack_99;
  }
  local_68 = "|";
  uStack_60 = 1;
  lStack_50 = (long)*(char *)((long)param_3 + 0x17);
  local_58 = param_3;
  if (lStack_50 < 0) {
    local_58 = (undefined8 *)*param_3;
    lStack_50 = param_3[1];
  }
  FUN_00217a74(param_1,&local_98,5);
  if (cStack_99 < '\0') {
    FUN_00005810(local_b0,CONCAT17(cStack_99,local_a0) & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


