// ==== 0x442c50c  ArcSearchEngine::init:keyword:url:faviconUrl:  (828 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

ID ArcSearchEngine::init_keyword_url_faviconUrl_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  undefined8 local_d0;
  undefined8 uStack_c8;
  ulong local_c0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  ulong local_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  long local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  long local_78;
  undefined8 local_70;
  long *plStack_68;
  ID local_60;
  class_t *pcStack_58;
  
  pcStack_58 = &objc::class_t::ArcSearchEngine;
  local_60 = param_1;
  IVar2 = NSObject::init((ID)&local_60,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (param_3 == 0) {
    local_88 = 0;
    uStack_80 = 0;
    local_78 = 0;
  }
  else {
    FUN_083b1228(&local_88,param_3);
  }
  if (param_4 == 0) {
    local_a0 = 0;
    uStack_98 = 0;
    local_90 = 0;
  }
  else {
    FUN_083b1228(&local_a0,param_4);
  }
  if (param_5 == 0) {
    local_b8 = 0;
    uStack_b0 = 0;
    local_a8 = 0;
  }
  else {
    FUN_01b356b8(&local_b8,param_5);
  }
  if (param_6 == 0) {
    local_d0 = 0;
    uStack_c8 = 0;
    local_c0 = 0;
  }
  else {
    FUN_01b356b8(&local_d0,param_6);
  }
  FUN_0433ffd8(&local_70,&local_88,&local_a0,&local_b8,&local_d0);
  if (*(char *)(IVar2 + 0x18) != '\0') {
    plVar4 = *(long **)(IVar2 + 0x10);
    if (plVar4 != (long *)0x0) {
      LOAcquire();
      lVar1 = plVar4[1];
      plVar4[1] = lVar1 + -1;
      LORelease();
      if (lVar1 == 0) {
        (**(code **)(*plVar4 + 0x10))(plVar4);
        if (plVar4[2] != 0) {
          LOAcquire();
          lVar1 = plVar4[2];
          plVar4[2] = lVar1 + -1;
          LORelease();
          if (lVar1 != 0) goto LAB_0442c668;
        }
        pcVar3 = *(code **)(*plVar4 + 0x20);
        if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
          FUN_00005810(plVar4,0x20);
        }
        else {
          (*pcVar3)(plVar4);
        }
      }
    }
LAB_0442c668:
    *(undefined1 *)(IVar2 + 0x18) = 0;
  }
  *(long **)(IVar2 + 0x10) = plStack_68;
  *(undefined8 *)(IVar2 + 8) = local_70;
  if (plStack_68 == (long *)0x0) {
    *(undefined1 *)(IVar2 + 0x18) = 1;
  }
  else {
    plVar4 = plStack_68 + 1;
    *plVar4 = *plVar4 + 1;
    *(undefined1 *)(IVar2 + 0x18) = 1;
    LOAcquire();
    lVar1 = *plVar4;
    *plVar4 = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plStack_68 + 0x10))(plStack_68);
      if (plStack_68[2] != 0) {
        LOAcquire();
        lVar1 = plStack_68[2];
        plStack_68[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0442c6f8;
      }
      pcVar3 = *(code **)(*plStack_68 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plStack_68,0x20);
      }
      else {
        (*pcVar3)(plStack_68);
      }
    }
  }
LAB_0442c6f8:
  if ((long)local_c0 < 0) {
    FUN_00005810(local_d0,local_c0 & 0x7fffffffffffffff);
  }
  if ((long)local_a8 < 0) {
    FUN_00005810(local_b8,local_a8 & 0x7fffffffffffffff);
  }
  if (local_90 < 0) {
    FUN_00005810(local_a0,local_90 << 1);
  }
  if (local_78 < 0) {
    FUN_00005810(local_88,local_78 << 1);
  }
  return IVar2;
}


