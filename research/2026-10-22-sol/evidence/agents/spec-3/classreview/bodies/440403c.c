// ==== 0x440403c  ArcImportSourceProfile::initWithSafariExportedData:profileName:name:categories:  (1080 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

ID ArcImportSourceProfile::initWithSafariExportedData_profileName_name_categories_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,long_long param_6)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  long *plVar4;
  undefined8 *****pppppuVar5;
  code *pcVar6;
  long *plVar7;
  long *plVar8;
  undefined1 auStack_220 [136];
  undefined8 ****local_198;
  long lStack_190;
  undefined8 local_188;
  ulong local_180;
  undefined8 uStack_178;
  ulong local_170;
  undefined8 uStack_168;
  ulong local_160;
  undefined8 uStack_158;
  ulong local_150;
  undefined8 uStack_148;
  undefined8 uStack_140;
  ulong local_138;
  undefined8 local_130;
  undefined8 ****local_128;
  undefined8 uStack_120;
  ulong local_118;
  ulong uStack_110;
  undefined8 local_108;
  ulong uStack_100;
  ID local_f8;
  class_t *pcStack_f0;
  ulong local_e8;
  undefined8 uStack_e0;
  ulong local_d8;
  
  pcStack_f0 = &objc::class_t::ArcImportSourceProfile;
  local_f8 = param_1;
  IVar3 = NSObject::init((ID)&local_f8,"init");
  if (IVar3 == 0) {
    return 0;
  }
  uStack_148 = 0;
  local_150 = 0;
  local_138 = 0;
  uStack_140 = 0;
  local_170 = 0;
  uStack_158 = 0;
  local_160 = 0;
  uStack_178 = 0;
  local_180 = 0;
  uStack_168 = 0xffffffff;
  uStack_120 = 0;
  local_128 = (undefined8 *****)0x0;
  uStack_110 = 0;
  local_118 = 0;
  uStack_100 = 0;
  local_108 = 0;
  local_130 = 0;
  if (param_5 == 0) {
    local_e8 = 0;
    uStack_e0 = 0;
    local_d8 = 0;
  }
  else {
    FUN_083b1228(&local_e8,param_5);
  }
  if ((long)local_170 < 0) {
    FUN_00005810(local_180,local_170 << 1);
  }
  uStack_178 = uStack_e0;
  local_180 = local_e8;
  local_170 = local_d8;
  if (param_4 == 0) {
    local_e8 = 0;
    uStack_e0 = 0;
    local_d8 = 0;
  }
  else {
    FUN_083b1228(&local_e8,param_4);
  }
  if ((long)uStack_100 < 0) {
    FUN_00005810(uStack_110,uStack_100 << 1);
  }
  local_108 = uStack_e0;
  uStack_110 = local_e8;
  uStack_100 = local_d8;
  uStack_168 = CONCAT44(uStack_168._4_4_,0xd);
  if (param_3 == 0) {
    local_198 = (undefined8 *****)0x0;
    lStack_190 = 0;
    local_188 = 0;
  }
  else {
    FUN_01b356b8(&local_198,param_3);
  }
  pppppuVar5 = (undefined8 *****)local_198;
  if (-1 < (long)local_188._7_1_) {
    pppppuVar5 = &local_198;
  }
  lVar2 = lStack_190;
  if (-1 < (long)local_188) {
    lVar2 = (long)local_188._7_1_;
  }
  FUN_008a7fc4(&local_e8,pppppuVar5,lVar2);
  if ((long)local_150 < 0) {
    FUN_00005810(local_160,local_150 & 0x7fffffffffffffff);
  }
  uStack_158 = uStack_e0;
  local_160 = local_e8;
  local_150 = local_d8;
  local_d8 = local_d8 & 0xffffffffffffff;
  local_e8 = local_e8 & 0xffffffffffffff00;
  if ((long)local_188 < 0) {
    FUN_00005810(local_198,local_188 & 0x7fffffffffffffff);
  }
  if ((long)local_138 < 0) {
    FUN_00005810(uStack_148,local_138 & 0x7fffffffffffffff);
  }
  uStack_148 = 0;
  uStack_140 = 0;
  local_138 = 0;
  local_130 = CONCAT62(local_130._2_6_,(short)param_6);
  if ((long)local_118 < 0) {
    uStack_120 = 0;
    pppppuVar5 = (undefined8 *****)local_128;
  }
  else {
    local_118 = local_118 & 0xffffffffffffff;
    pppppuVar5 = &local_128;
  }
  *(undefined1 *)pppppuVar5 = 0;
  FUN_07040958(auStack_220,&local_180);
  plVar4 = (long *)FUN_00008790(0xa0);
  plVar8 = plVar4 + 1;
  *plVar8 = 0;
  plVar4[2] = 0;
  *plVar4 = (long)&PTR_viewWillStartLiveResize_0da7ef98;
  plVar1 = plVar4 + 3;
  if (plVar1 == (long *)0x0) {
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(1,0x4404474);
    (*pcVar6)();
  }
  FUN_07040958(&local_e8,auStack_220);
  FUN_07040958(plVar1,&local_e8);
  FUN_07040a20(&local_e8);
  FUN_07040a20(auStack_220);
  if (*(char *)(IVar3 + 0x18) != '\0') {
    plVar7 = *(long **)(IVar3 + 0x10);
    if (plVar7 != (long *)0x0) {
      LOAcquire();
      lVar2 = plVar7[1];
      plVar7[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        (**(code **)(*plVar7 + 0x10))(plVar7);
        if (plVar7[2] != 0) {
          LOAcquire();
          lVar2 = plVar7[2];
          plVar7[2] = lVar2 + -1;
          LORelease();
          if (lVar2 != 0) goto LAB_0440434c;
        }
        pcVar6 = *(code **)(*plVar7 + 0x20);
        if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
          FUN_00005810(plVar7,0x20);
        }
        else {
          (*pcVar6)(plVar7);
        }
      }
    }
LAB_0440434c:
    *(undefined1 *)(IVar3 + 0x18) = 0;
  }
  *(long **)(IVar3 + 8) = plVar1;
  *(long **)(IVar3 + 0x10) = plVar4;
  if (plVar4 == (long *)0x0) {
    *(undefined1 *)(IVar3 + 0x18) = 1;
  }
  else {
    *plVar8 = *plVar8 + 1;
    *(undefined1 *)(IVar3 + 0x18) = 1;
    LOAcquire();
    lVar2 = *plVar8;
    *plVar8 = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar2 = plVar4[2];
        plVar4[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_04404360;
      }
      pcVar6 = *(code **)(*plVar4 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar6)(plVar4);
      }
    }
  }
LAB_04404360:
  FUN_07040a20(&local_180);
  return IVar3;
}


