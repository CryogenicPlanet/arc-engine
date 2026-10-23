// ==== 0x4438824  ArcAutocompleteMatchAdditionalData::init:value:  (464 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcAutocompleteMatchAdditionalData::init_value_(ID param_1,SEL param_2,ID param_3,ID param_4)

{
  undefined8 uVar1;
  undefined8 uVar2;
  ulong uVar3;
  ID IVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  ulong uVar7;
  undefined8 local_a0;
  undefined8 uStack_98;
  ulong local_90;
  undefined8 local_88;
  undefined8 local_80;
  ulong local_78;
  ID local_70;
  class_t *pcStack_68;
  
  pcStack_68 = &objc::class_t::ArcAutocompleteMatchAdditionalData;
  local_70 = param_1;
  IVar4 = NSObject::init((ID)&local_70,"init");
  if (IVar4 != 0) {
    if (param_3 == 0) {
      local_88 = 0;
      local_80 = 0;
      local_78 = 0;
    }
    else {
      FUN_01b356b8(&local_88,param_3);
    }
    if (param_4 == 0) {
      uVar7 = 0;
      uVar6 = 0;
      uVar5 = 0;
    }
    else {
      FUN_01b356b8(&local_a0,param_4);
      uVar5 = local_a0;
      uVar6 = uStack_98;
      uVar7 = local_90;
    }
    uVar3 = local_78;
    uVar2 = local_80;
    uVar1 = local_88;
    local_80 = 0;
    local_78 = 0;
    local_90 = 0;
    local_88 = 0;
    local_a0 = 0;
    uStack_98 = 0;
    if (*(char *)(IVar4 + 0x38) != '\0') {
      if (*(char *)(IVar4 + 0x37) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar4 + 0x20),*(ulong *)(IVar4 + 0x30) & 0x7fffffffffffffff);
      }
      if (*(char *)(IVar4 + 0x1f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar4 + 8),*(ulong *)(IVar4 + 0x18) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar4 + 0x38) = 0;
    }
    if ((long)uVar3 < 0) {
      FUN_00060a60(IVar4 + 8,uVar1,uVar2);
    }
    else {
      *(undefined8 *)(IVar4 + 8) = uVar1;
      *(undefined8 *)(IVar4 + 0x10) = uVar2;
      *(ulong *)(IVar4 + 0x18) = uVar3;
    }
    if ((long)uVar7 < 0) {
      FUN_00060a60(IVar4 + 0x20,uVar5,uVar6);
      *(undefined1 *)(IVar4 + 0x38) = 1;
      FUN_00005810(uVar5,uVar7 & 0x7fffffffffffffff);
    }
    else {
      *(undefined8 *)(IVar4 + 0x20) = uVar5;
      *(undefined8 *)(IVar4 + 0x28) = uVar6;
      *(ulong *)(IVar4 + 0x30) = uVar7;
      *(undefined1 *)(IVar4 + 0x38) = 1;
    }
    if ((long)uVar3 < 0) {
      FUN_00005810(uVar1,uVar3 & 0x7fffffffffffffff);
    }
    if ((long)local_78 < 0) {
      FUN_00005810(local_88,local_78 & 0x7fffffffffffffff);
    }
  }
  return IVar4;
}


