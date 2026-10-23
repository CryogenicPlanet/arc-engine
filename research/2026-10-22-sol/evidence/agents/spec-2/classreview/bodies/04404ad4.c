// ==== 0x4404ad4  ArcImage::dangerouslyDecodeFromData:scale:  (564 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x1c bytes */

ID ArcImage::dangerouslyDecodeFromData_scale_(ID param_1,SEL param_2,ID param_3,float param_4)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  ID IVar4;
  code *pcVar5;
  long *plVar6;
  undefined4 in_register_00005004;
  undefined **local_78 [2];
  long *local_68;
  undefined **local_60;
  undefined8 uStack_58;
  long *local_50;
  ulong local_48;
  
  uStack_58 = 0;
  local_60 = (undefined **)0x0;
  local_48 = 0;
  local_50 = (long *)0x0;
  IVar3 = _objc_retainAutorelease(param_3);
  IVar3 = _objc_msgSend(IVar3,"bytes");
  IVar4 = _objc_msgSend(param_3,"length");
  FUN_04337b04(&local_60,CONCAT44(in_register_00005004,param_4),IVar3,IVar4);
  if ((char)local_48 == '\0') {
    return 0;
  }
  IVar3 = _objc_alloc(&objc::class_t::ArcImage);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  FUN_043386f8(local_78,&local_60);
  IVar3 = initWithArcImage_(IVar3,"initWithArcImage:",local_78);
  local_78[0] = &PTR_FUN_0da7eed0;
  if (local_68 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_68[1];
    local_68[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_68 + 0x10))(local_68);
      if (local_68[2] != 0) {
        LOAcquire();
        lVar2 = local_68[2];
        local_68[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_04404ba8;
      }
      pcVar5 = *(code **)(*local_68 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(local_68,0x20);
      }
      else {
        (*pcVar5)(local_68);
      }
    }
  }
LAB_04404ba8:
  plVar1 = local_50;
  if (((char)local_48 != '\0') && (local_60 = &PTR_FUN_0da7eeb0, local_50 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar6 = local_50 + 2;
      (**(code **)(*local_50 + 0x10))(local_50);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return IVar3;
        }
      }
      pcVar5 = *(code **)(*plVar1 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar5)(plVar1);
      }
    }
  }
  return IVar3;
}


