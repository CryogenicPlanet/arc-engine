// ==== 0x43abdf8  FUN_043abdf8  (216 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043abdf8(undefined8 *param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 local_b0;
  undefined8 uStack_a8;
  ulong local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 local_40 [2];
  
  local_40[0] = 0;
  uStack_58 = 0;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  uStack_78 = 0;
  local_80 = 0;
  uStack_68 = 0;
  uStack_70 = 0;
  uStack_98 = 0;
  local_a0 = 0;
  uStack_88 = 0;
  uStack_90 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  FUN_0312f86c(&local_b0,param_2);
  if ((char)uStack_98 == '\0') {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
  }
  else {
    puVar1 = (undefined8 *)FUN_007cf864(&local_b0);
    if (*(char *)((long)puVar1 + 0x17) < '\0') {
      FUN_00060a60(param_1,*puVar1,puVar1[1]);
    }
    else {
      uVar3 = puVar1[1];
      uVar2 = *puVar1;
      param_1[2] = puVar1[2];
      param_1[1] = uVar3;
      *param_1 = uVar2;
    }
  }
  FUN_000282bc(local_40,0);
  FUN_0002827c(&uStack_90);
  if ((long)local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 & 0x7fffffffffffffff);
  }
  return 0;
}


