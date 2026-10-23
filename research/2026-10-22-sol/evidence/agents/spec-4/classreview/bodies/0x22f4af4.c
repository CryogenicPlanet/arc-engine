// ==== 0x22f4af4  FUN_022f4af4  (532 bytes)

long FUN_022f4af4(long param_1)

{
  undefined1 auVar1 [16];
  code *pcVar2;
  int iVar3;
  undefined8 uVar4;
  ulong uVar5;
  undefined1 auVar6 [16];
  undefined **local_710;
  undefined8 uStack_708;
  ulong local_700;
  undefined1 auStack_108 [64];
  undefined8 local_c8;
  undefined1 local_c0 [16];
  undefined8 local_a8;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined4 local_88;
  undefined8 local_84;
  undefined8 uStack_7c;
  undefined1 auStack_70 [8];
  undefined1 *local_68;
  undefined8 local_60;
  undefined4 local_58;
  long local_48;
  
  auVar1._8_8_ = local_c0._8_8_;
  auVar1._0_8_ = local_c0._0_8_;
  auVar6._8_8_ = local_c0._8_8_;
  auVar6._0_8_ = local_c0._0_8_;
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  if (((*(int *)(param_1 + 0x1c0) == 1) && (local_c0 = auVar6, *(long *)(param_1 + 0x1d0) == 0)) &&
     (local_c0 = auVar1, *(char *)(param_1 + 0x1b8) != '\0')) {
    local_700 = *(ulong *)(param_1 + 0x1c4);
    uStack_708 = 0x200000006;
    local_710 = (undefined **)0x0;
    uVar5 = -(local_700 >> 0x1f & 1) & 0xfffffffc00000000 | (local_700 & 0xffffffff) << 2;
    if (uVar5 >> 0x1f != 0) {
      uVar5 = 0;
    }
    uVar5 = FUN_01265cfc(param_1 + 0x1d0,&local_710,uVar5);
    if ((uVar5 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_085e9488("../../third_party/skia/src/core/SkBitmap.cpp",0xfc,
                   "assertf(%s): ColorType:%d AlphaType:%d [w:%d h:%d] rb:%zu");
    }
    if (local_710 != (undefined **)0x0) {
      LOAcquire();
      iVar3 = *(int *)local_710;
      *(int *)local_710 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        FUN_00005810(local_710,0x90);
      }
    }
    uStack_708 = *(undefined8 *)(param_1 + 0x1f8);
    local_710 = (undefined **)0x0;
    FUN_01d79700(0,0,0,0,param_1 + 0x1d0,&local_710);
    _bzero(&uStack_708,0x6b8);
    auVar6 = FUN_00cadc90(0);
    local_710 = &PTR_thunk_FUN_00b84158_0d849b48;
    FUN_01274470(auStack_108,0x60,&uStack_708,0x600,1);
    local_c8 = 0;
    local_a8 = 0;
    uStack_90 = 0;
    local_98 = 0;
    local_88 = 0xffffffff;
    local_68 = auStack_70;
    uStack_7c = 0;
    local_84 = 0;
    local_60 = 0x200000000;
    local_58 = 0;
    local_c0 = auVar6;
    uVar4 = FUN_00008790(0x660);
    FUN_01d7a000(uVar4,param_1 + 0x1d0,local_c0,0);
    FUN_01d7a650(&local_710,uVar4);
    if ((*(byte *)(param_1 + 0x1b8) & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x22f4cd0);
      (*pcVar2)();
    }
    FUN_022f4d08(*(undefined8 *)(param_1 + 0x1b0),&local_710);
    if (*(long *)(param_1 + 0x1d0) != 0) {
      *(undefined1 *)(*(long *)(param_1 + 0x1d0) + 0x61) = 2;
    }
    FUN_00b84158(&local_710);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return param_1 + 0x1d0;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


