// ==== 0x4378aa0  FUN_04378aa0  (204 bytes)

undefined8 FUN_04378aa0(long param_1)

{
  long lVar1;
  ulong uVar2;
  undefined8 uVar3;
  long *plVar4;
  
  if (*(long *)(param_1 + 0x130) != 0) {
    lVar1 = FUN_018efdf0(*(undefined8 *)(*(long *)(DAT_0e481618 + 0x2a0) + 8));
    if (lVar1 == 0) {
      return 0;
    }
    lVar1 = FUN_0031dab4();
    if (lVar1 == 0) {
      return 0;
    }
    if ((((*(long *)(lVar1 + 0x28) != 0) && (*(char *)(*(long *)(lVar1 + 0x28) + 4) == '\0')) &&
        (*(long *)(lVar1 + 0x30) != 0)) &&
       (((*(long *)(lVar1 + 0x28) != 0 && (*(char *)(*(long *)(lVar1 + 0x28) + 4) == '\0')) &&
        (plVar4 = *(long **)(lVar1 + 0x30),
        plVar4 != (long *)0x0 && plVar4 != (long *)segment_command_00000020.segname)))) {
      uVar2 = FUN_03195130(*(undefined8 *)(param_1 + 0x130));
      if ((uVar2 & 1) != 0) {
        return 1;
      }
      uVar3 = (**(code **)(*plVar4 + 0x18))(plVar4);
      lVar1 = FUN_004969fc(uVar3,&PTR_s_UnloadController_kDataKeyImpl_0df54710);
      if ((*(byte *)(lVar1 + 0xa8) & 1) == 0) {
        uVar3 = FUN_07d6176c(lVar1,*(undefined8 *)(param_1 + 0x130));
        return uVar3;
      }
    }
  }
  return 0;
}


