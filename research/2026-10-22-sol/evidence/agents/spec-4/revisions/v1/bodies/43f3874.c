// ==== 0x43f3874  objc::Categories::ArcBrowserContext_Extensions_::import:withCategories:withAllowedExtensions:withBookmarkImportTargets:skipExistingCookies:withDelegate:error:  (1732 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Removing unreachable block (ram,0x043f3a18) */
/* WARNING: Removing unreachable block (ram,0x043f3e94) */
/* WARNING: Removing unreachable block (ram,0x043f3ed8) */
/* WARNING: Removing unreachable block (ram,0x043f3ee8) */
/* WARNING: Removing unreachable block (ram,0x043f3eb0) */
/* WARNING: Removing unreachable block (ram,0x043f3eec) */
/* WARNING: Removing unreachable block (ram,0x043f3efc) */
/* WARNING: Removing unreachable block (ram,0x043f3ec8) */
/* WARNING: Removing unreachable block (ram,0x043f3f0c) */
/* Function Stack Size: 0x44 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::
     import_withCategories_withAllowedExtensions_withBookmarkImportTargets_skipExistingCookies_withDelegate_error_
               (ID param_1,SEL param_2,ID param_3,long_long param_4,ID param_5,ID param_6,
               bool param_7,ID param_8,ID *param_9)

{
  void *pvVar1;
  long *plVar2;
  ulong uVar3;
  code *pcVar4;
  uint uVar5;
  long lVar6;
  bool bVar7;
  ID IVar8;
  ID IVar9;
  undefined8 *puVar10;
  void *pvVar11;
  uint *puVar12;
  long *plVar13;
  undefined8 *puVar14;
  size_t sVar15;
  uint *local_178;
  ulong local_170;
  long *plStack_168;
  undefined8 uStack_160;
  long *plStack_158;
  char local_150;
  void *local_148;
  long local_140;
  ID IStack_138;
  char local_130;
  uint *local_128;
  undefined8 local_120;
  long *plStack_118;
  ulong local_110;
  ulong local_100;
  long *plStack_f8;
  byte local_f0;
  ulong local_e0;
  long *plStack_d8;
  undefined8 local_d0;
  long *plStack_c8;
  undefined8 local_c0;
  undefined *local_b8;
  undefined8 uStack_b0;
  undefined1 *local_a8;
  undefined *puStack_a0;
  void **local_98;
  void *local_90;
  long lStack_88;
  ID local_80;
  undefined8 local_78;
  
  lStack_88 = 0;
  local_90 = (void *)0x0;
  local_78 = 0;
  local_80 = 0;
  if (param_5 != 0) {
    local_90 = (void *)0x0;
    lStack_88 = 0;
    local_80 = 0;
    local_78 = 1;
    IVar8 = _objc_msgSend(param_5,"count");
    IVar9 = local_80;
    lVar6 = lStack_88;
    if (local_80 < IVar8) {
      if (0xaaaaaaaaaaaaaaa < IVar8) {
                    /* WARNING: Subroutine does not return */
        FUN_04047380();
      }
      pvVar11 = (void *)FUN_00008790(IVar8 * 0x18);
      pvVar1 = local_90;
      sVar15 = lVar6 * 0x18;
      if (sVar15 != 0) {
        _memcpy(pvVar11,local_90,sVar15);
      }
      lStack_88 = ((long)sVar15 >> 3) * -0x5555555555555555;
      local_90 = pvVar11;
      local_80 = IVar8;
      if (pvVar1 != (void *)0x0) {
        FUN_00005810(pvVar1,IVar9 * 0x18);
      }
    }
    local_b8 = PTR___NSConcreteGlobalBlock_0d824130;
    uStack_b0 = 0xd0800000;
    local_a8 = &LAB_043f4214;
    puStack_a0 = &DAT_0df6b218;
    local_98 = &local_90;
    _objc_msgSend(param_5,"enumerateObjectsUsingBlock:",&local_b8);
  }
  local_c0 = 0;
  plStack_d8 = (long *)0x0;
  local_e0 = 0;
  plStack_c8 = (long *)0x0;
  local_d0 = 0;
  if (param_6 != 0) {
    IVar9 = _objc_msgSend(param_6,"targetNodeForBookmarkBarBookmarks");
    if ((((IVar9 == 0) || (_objc_msgSend(IVar9,"node",&local_100), (local_f0 & 1) == 0)) ||
        (IVar9 = _objc_msgSend(param_6,"targetNodeForOtherBookmarks"), IVar9 == 0)) ||
       (_objc_msgSend(IVar9,"node",&local_120), (local_110 & 1) == 0)) goto LAB_043f3e18;
    if ((char)local_c0 != '\0') {
      FUN_04333708(&local_e0);
    }
    plStack_d8 = plStack_f8;
    local_e0 = local_100;
    plStack_c8 = plStack_118;
    local_d0 = local_120;
    local_100 = 0;
    plStack_f8 = (long *)0x0;
    local_120 = 0;
    plStack_118 = (long *)0x0;
    local_c0 = CONCAT71(local_c0._1_7_,1);
  }
  local_128 = (uint *)0x0;
  _objc_msgSend(param_1,"browser_context",&local_100);
  if ((local_f0 & 1) == 0) {
LAB_043f3e18:
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43f3e1c);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_120 = 0;
    plStack_118 = (long *)0x0;
    local_110 = 0;
LAB_043f3f20:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  _objc_msgSend(param_3,"profile",&local_120);
  IVar9 = local_80;
  lVar6 = lStack_88;
  if ((local_110 & 1) == 0) goto LAB_043f3f20;
  local_148 = (void *)((ulong)local_148 & 0xffffffffffffff00);
  local_130 = (char)local_78 != '\0';
  if ((bool)local_130) {
    local_148 = local_90;
    local_90 = (void *)0x0;
    lStack_88 = 0;
    local_80 = 0;
    IStack_138 = IVar9;
    local_140 = lVar6;
  }
  puVar10 = (undefined8 *)FUN_00008790(0x10);
  *puVar10 = &PTR_FUN_0da87958;
  _objc_initWeak(puVar10 + 1,param_8);
  plVar2 = plStack_d8;
  uVar3 = local_e0;
  local_170 = local_170 & 0xffffffffffffff00;
  local_150 = (char)local_c0 != '\0';
  if ((bool)local_150) {
    local_e0 = 0;
    plStack_d8 = (long *)0x0;
    plStack_168 = plVar2;
    local_170 = uVar3;
    plStack_158 = plStack_c8;
    uStack_160 = local_d0;
    local_d0 = 0;
    plStack_c8 = (long *)0x0;
  }
  FUN_04333d7c(&local_128,&local_100,&local_120,(uint)param_4 & 0xfff,&local_148,puVar10,&local_170,
               param_7);
  if (local_150 != '\0') {
    FUN_04333708(&local_170);
  }
  pvVar1 = local_148;
  if ((local_130 != '\0') && (local_148 != (void *)0x0)) {
    if (local_140 * 0x18 != 0) {
      lVar6 = local_140 * -0x18;
      puVar10 = (undefined8 *)(local_140 * 0x18 + (long)local_148);
      do {
        puVar14 = puVar10 + -3;
        if (puVar14 == (undefined8 *)0x0) goto LAB_043f3e18;
        if (*(char *)((long)puVar10 + -1) < '\0') {
          FUN_00005810(*puVar14,puVar10[-1] & 0x7fffffffffffffff);
        }
        lVar6 = lVar6 + 0x18;
        puVar10 = puVar14;
      } while (lVar6 != 0);
    }
    local_140 = ((long)pvVar1 - (long)local_148 >> 3) * -0x5555555555555555;
    FUN_00005810(local_148,IStack_138 * 0x18);
  }
  plVar2 = plStack_118;
  if (((char)local_110 != '\0') && (plStack_118 != (long *)0x0)) {
    LOAcquire();
    lVar6 = plStack_118[1];
    plStack_118[1] = lVar6 + -1;
    LORelease();
    if (lVar6 == 0) {
      plVar13 = plStack_118 + 2;
      (**(code **)(*plStack_118 + 0x10))(plStack_118);
      if (*plVar13 != 0) {
        LOAcquire();
        lVar6 = plVar2[2];
        plVar2[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) goto LAB_043f3b54;
      }
      pcVar4 = *(code **)(*plVar2 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar4)(plVar2);
      }
    }
  }
LAB_043f3b54:
  plVar2 = plStack_f8;
  if ((local_f0 != 0) && (plStack_f8 != (long *)0x0)) {
    LOAcquire();
    lVar6 = plStack_f8[1];
    plStack_f8[1] = lVar6 + -1;
    LORelease();
    if (lVar6 == 0) {
      plVar13 = plStack_f8 + 2;
      (**(code **)(*plStack_f8 + 0x10))(plStack_f8);
      if (*plVar13 != 0) {
        LOAcquire();
        lVar6 = plVar2[2];
        plVar2[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) goto LAB_043f3b78;
      }
      pcVar4 = *(code **)(*plVar2 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar4)(plVar2);
      }
    }
  }
LAB_043f3b78:
  local_178 = local_128;
  puVar12 = local_128;
  if (((ulong)local_128 & 1) == 0) {
    uVar5 = *local_128;
    *local_128 = uVar5 + 1;
    puVar12 = (uint *)(ulong)uVar5;
  }
  bVar7 = FUN_04402548(puVar12,&local_178,param_9);
  if (((ulong)local_178 & 1) == 0) {
    FUN_04b35898();
  }
  if (((ulong)local_128 & 1) == 0) {
    FUN_04b35898();
  }
  if ((char)local_c0 != '\0') {
    FUN_04333708(&local_e0);
  }
  pvVar1 = local_90;
  if (((char)local_78 != '\0') && (local_90 != (void *)0x0)) {
    if (lStack_88 * 0x18 != 0) {
      lVar6 = lStack_88 * -0x18;
      puVar10 = (undefined8 *)(lStack_88 * 0x18 + (long)local_90);
      do {
        puVar14 = puVar10 + -3;
        if (puVar14 == (undefined8 *)0x0) goto LAB_043f3e18;
        if (*(char *)((long)puVar10 + -1) < '\0') {
          FUN_00005810(*puVar14,puVar10[-1] & 0x7fffffffffffffff);
        }
        lVar6 = lVar6 + 0x18;
        puVar10 = puVar14;
      } while (lVar6 != 0);
    }
    lStack_88 = ((long)pvVar1 - (long)local_90 >> 3) * -0x5555555555555555;
    FUN_00005810(local_90,local_80 * 0x18);
  }
  return bVar7;
}


