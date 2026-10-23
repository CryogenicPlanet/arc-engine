// ==== 0x4405db4  FUN_04405db4  (348 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04405db4(undefined8 *param_1,long param_2)

{
  long lVar1;
  undefined8 uVar2;
  ID IVar3;
  long *plVar4;
  code *pcVar5;
  long *plVar6;
  undefined **local_50;
  undefined8 local_48;
  long *plStack_40;
  char local_38;
  
  plVar4 = (long *)*param_1;
  local_50 = (undefined **)((ulong)local_50 & 0xffffffffffffff00);
  local_38 = '\0';
  if ((*(byte *)(param_2 + 0x18) & 1) == 0) {
    lVar1 = *plVar4;
    uVar2 = (**(code **)(lVar1 + 0x10))(lVar1,0);
  }
  else {
    local_50 = &PTR_FUN_0da7eeb0;
    plStack_40 = *(long **)(param_2 + 0x10);
    local_48 = *(undefined8 *)(param_2 + 8);
    *(undefined8 *)(param_2 + 8) = 0;
    *(undefined8 *)(param_2 + 0x10) = 0;
    local_38 = '\x01';
    lVar1 = *plVar4;
    IVar3 = _objc_alloc(&objc::class_t::ArcImageRep);
    IVar3 = ArcImageRep::initWithArcImageRep_(IVar3,"initWithArcImageRep:",&local_50);
    (**(code **)(lVar1 + 0x10))(lVar1,IVar3);
    uVar2 = 0;
  }
  plVar4 = plStack_40;
  if ((local_38 != '\0') && (local_50 = &PTR_FUN_0da7eeb0, plStack_40 != (long *)0x0)) {
    LOAcquire();
    lVar1 = plStack_40[1];
    plStack_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      plVar6 = plStack_40 + 2;
      uVar2 = (**(code **)(*plStack_40 + 0x10))(plStack_40);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return uVar2;
        }
      }
      pcVar5 = *(code **)(*plVar4 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        uVar2 = FUN_00005810(plVar4,0x20);
      }
      else {
        uVar2 = (*pcVar5)(plVar4);
      }
    }
  }
  return uVar2;
}


