// ==== 0x440b880  ArcSiteSettingInfo::init  (324 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcSiteSettingInfo::init(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  long *plVar4;
  code *pcVar5;
  long *local_50;
  long *local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcSiteSettingInfo;
  local_40 = param_1;
  IVar3 = NSObject::init((ID)&local_40,"init");
  if (IVar3 != 0) {
    plVar4 = (long *)FUN_00008790(0xb0);
    plVar4[1] = 0;
    plVar4[2] = 0;
    *plVar4 = (long)&PTR_viewWillStartLiveResize_0da7f770;
    plVar1 = plVar4 + 3;
    if (plVar1 == (long *)0x0) {
                    /* WARNING: Does not return */
      pcVar5 = (code *)SoftwareBreakpoint(1,0x440b9c4);
      (*pcVar5)();
    }
    *(undefined4 *)(plVar4 + 3) = 0;
    FUN_0bd3c0b4(plVar4 + 4);
    if ((*(byte *)(IVar3 + 0x18) & 1) == 0) {
      *(long **)(IVar3 + 8) = plVar1;
      *(long **)(IVar3 + 0x10) = plVar4;
      *(undefined1 *)(IVar3 + 0x18) = 1;
    }
    else {
      local_50 = plVar1;
      local_48 = plVar4;
      FUN_04331f00(IVar3 + 8,&local_50);
      plVar1 = local_48;
      if (local_48 != (long *)0x0) {
        LOAcquire();
        lVar2 = local_48[1];
        local_48[1] = lVar2 + -1;
        LORelease();
        if (lVar2 == 0) {
          plVar4 = local_48 + 2;
          (**(code **)(*local_48 + 0x10))(local_48);
          if (*plVar4 != 0) {
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
    }
  }
  return IVar3;
}


