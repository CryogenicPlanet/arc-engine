// ==== 0x432bdc0  FUN_0432bdc0  (104 bytes)

undefined1  [16] FUN_0432bdc0(long param_1)

{
  int iVar1;
  ulong uVar2;
  undefined1 auVar3 [16];
  ulong local_30;
  long lStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  lStack_28 = 0;
  local_30 = 0;
  uStack_20 = 0;
  uStack_18 = 0xffffffff00000000;
  iVar1 = (**(code **)(**(long **)(param_1 + 0x10) + 0x88))
                    (*(long **)(param_1 + 0x10),*(undefined8 *)(param_1 + 0x18),&local_30);
  if (iVar1 == 0) {
    uVar2 = 0;
  }
  else {
    uVar2 = local_30 & 0xff00 | lStack_28 << 0x20 | local_30 & 0xff;
  }
  auVar3[8] = iVar1 != 0;
  auVar3._0_8_ = uVar2;
  auVar3._9_7_ = 0;
  return auVar3;
}


