// ==== 0x4411cac  objc::Categories::ArcWebContents_Extensions_::thumbnailsGenerator  (516 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcWebContents_Extensions_::thumbnailsGenerator(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  ID in_x4;
  code *pcVar3;
  ThumbnailsGenerator TVar4;
  long local_68;
  long *local_60;
  byte local_58;
  Impl aIStack_50 [8];
  long *local_48;
  
  IVar2 = _objc_getAssociatedObject(param_1,"thumbnailsGenerator");
  if (IVar2 != 0) {
    return IVar2;
  }
  IVar2 = _objc_alloc(&class_t::ArcWebContentsThumbnailsGenerator);
  _objc_msgSend(param_1,"web_contents",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4411eb0);
    (*pcVar3)();
  }
  FUN_042aec00(aIStack_50,*(undefined8 *)(local_68 + 0x2e8));
  TVar4.field0_0x0.field1_0x8 = (__shared_weak_count *)param_1;
  TVar4.field0_0x0.field0_0x0 = aIStack_50;
  IVar2 = ArcWebContentsThumbnailsGenerator::init_forWebContents_
                    (IVar2,"init:forWebContents:",TVar4,in_x4);
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04411d54;
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
LAB_04411d54:
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_60[1];
    local_60[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar1 = local_60[2];
        local_60[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04411d74;
      }
      pcVar3 = *(code **)(*local_60 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar3)(local_60);
      }
    }
  }
LAB_04411d74:
  _objc_setAssociatedObject(param_1,"thumbnailsGenerator",IVar2,1);
  return IVar2;
}


