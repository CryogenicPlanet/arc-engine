// ==== 0x443fd88  ArcMenuItemModel::submenuModel  (316 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcMenuItemModel::submenuModel(ID param_1,SEL param_2)

{
  undefined8 uVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  undefined1 local_50 [16];
  undefined8 local_40;
  undefined1 uStack_31;
  
  if (*(ID *)(param_1 + 0x20) != 0) {
    return *(ID *)(param_1 + 0x20);
  }
  IVar3 = _objc_alloc(&objc::class_t::ArcMenuModel);
  if ((*(byte *)(param_1 + 0x48) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x443fec4);
    (*pcVar4)();
  }
  plVar5 = *(long **)(*(long *)(param_1 + 0x38) + 0x10);
  local_40 = (**(code **)(*plVar5 + 0xe8))(plVar5,*(undefined8 *)(*(long *)(param_1 + 0x38) + 0x18))
  ;
  local_50 = FUN_0433cf10(&uStack_31,&local_40);
  IVar3 = ArcMenuModel::initWithMenuModel_(IVar3,"initWithMenuModel:",local_50);
  uVar1 = local_50._8_8_;
  *(ID *)(param_1 + 0x20) = IVar3;
  if ((long *)local_50._8_8_ != (long *)0x0) {
    LOAcquire();
    lVar2 = *(long *)(local_50._8_8_ + 8);
    *(long *)(local_50._8_8_ + 8) = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = (long *)(local_50._8_8_ + 0x10);
      (**(code **)(*(long *)local_50._8_8_ + 0x10))(local_50._8_8_);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = *(long *)(uVar1 + 0x10);
        *(long *)(uVar1 + 0x10) = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_0443fe24;
      }
      pcVar4 = *(code **)(*(long *)uVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(uVar1,0x20);
      }
      else {
        (*pcVar4)(uVar1);
      }
    }
  }
LAB_0443fe24:
  return *(ID *)(param_1 + 0x20);
}


