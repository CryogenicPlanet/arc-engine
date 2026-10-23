// ==== 0x440e174  FUN_0440e174  (7468 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0440e174(ID param_1)

{
  long *plVar1;
  undefined ********ppppppppuVar2;
  long lVar3;
  int iVar4;
  undefined8 uVar5;
  ID IVar6;
  undefined8 *puVar7;
  long *extraout_x1;
  ulong uVar8;
  uint uVar9;
  code *pcVar10;
  uint *puVar11;
  long *plVar12;
  float fVar13;
  double dVar14;
  float fVar15;
  ulong local_b0;
  long *local_a8;
  undefined1 local_a0;
  undefined8 local_98;
  long *local_90;
  undefined1 auStack_85 [5];
  ulong local_80;
  long *plStack_78;
  byte local_70;
  undefined *******local_68;
  long *local_60;
  long *local_58;
  
  if (param_1 == 0) goto LAB_0440f264;
  local_68 = (undefined *******)0x0;
  local_60 = (long *)0x0;
  local_58 = (long *)0x0;
  _objc_msgSend(param_1,"web_contents",&local_80);
  if ((local_70 & 1) == 0) goto LAB_0440f264;
  FUN_04376820(&local_68,local_80);
  plVar1 = plStack_78;
  if ((local_70 != 0) && (plStack_78 != (long *)0x0)) {
    LOAcquire();
    lVar3 = plStack_78[1];
    plStack_78[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = plStack_78 + 2;
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e1f0;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e1f0:
  plVar1 = local_60;
  if (-1 < (long)local_58) {
    plVar1 = (long *)((ulong)local_58 >> 0x38);
  }
  if (plVar1 == (long *)0x0) {
    uVar5 = 0;
  }
  else {
    ppppppppuVar2 = (undefined ********)local_68;
    if (-1 < (long)local_58) {
      ppppppppuVar2 = &local_68;
    }
    uVar5 = FUN_01d9fb90(ppppppppuVar2,plVar1);
  }
  _objc_msgSend(param_1,"setTitle:",uVar5);
  if ((long)local_58 < 0) {
    FUN_00005810(local_68,(long)local_58 << 1);
  }
  _objc_msgSend(param_1,"web_contents",&local_80);
  if ((local_70 & 1) == 0) goto LAB_0440f264;
  FUN_04376880(&local_68,local_80);
  uVar5 = FUN_043abd34(&local_68);
  _objc_msgSend(param_1,"setUrl:",uVar5);
  if ((long)local_58 < 0) {
    FUN_00005810(local_68,(ulong)local_58 & 0x7fffffffffffffff);
  }
  plVar1 = plStack_78;
  if ((local_70 != 0) && (plStack_78 != (long *)0x0)) {
    LOAcquire();
    lVar3 = plStack_78[1];
    plStack_78[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = plStack_78 + 2;
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e2e8;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e2e8:
  local_68 = (undefined *******)0x0;
  local_60 = (long *)0x0;
  local_58 = (long *)0x0;
  _objc_msgSend(param_1,"web_contents",&local_80);
  if ((local_70 & 1) == 0) goto LAB_0440f264;
  FUN_0437761c(&local_68,local_80);
  plVar1 = plStack_78;
  if ((local_70 != 0) && (plStack_78 != (long *)0x0)) {
    LOAcquire();
    lVar3 = plStack_78[1];
    plStack_78[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = plStack_78 + 2;
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e334;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e334:
  plVar1 = local_60;
  if (-1 < (long)local_58) {
    plVar1 = (long *)((ulong)local_58 >> 0x38);
  }
  if (plVar1 == (long *)0x0) {
    uVar5 = 0;
  }
  else {
    ppppppppuVar2 = (undefined ********)local_68;
    if (-1 < (long)local_58) {
      ppppppppuVar2 = &local_68;
    }
    uVar5 = FUN_01d9fb90(ppppppppuVar2,plVar1);
  }
  _objc_msgSend(param_1,"setSelectedText:",uVar5);
  if ((long)local_58 < 0) {
    FUN_00005810(local_68,(long)local_58 << 1);
  }
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  if ((undefined *******)local_68[0x26] == (undefined *******)0x0) {
    uVar8 = 0;
  }
  else {
    iVar4 = FUN_00f615e8(local_68[0x26] + 0x46,0);
    uVar8 = (ulong)(iVar4 == 2);
  }
  _objc_msgSend(param_1,"setIsLoading:",uVar8);
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e40c;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e40c:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  _objc_msgSend(param_1,"setLoadProgress:",local_68[0x68]);
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e45c;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e45c:
  _objc_msgSend(param_1,"web_contents",&local_80);
  if ((local_70 & 1) == 0) goto LAB_0440f264;
  FUN_04376700(&local_68,local_80);
  if ((*local_60 == 0) || (*(long *)(*local_60 + 0x18) == 0)) {
    uVar5 = 0;
  }
  else {
    uVar5 = FUN_02a02ccc();
  }
  _objc_msgSend(param_1,"setFavicon:",uVar5);
  plVar1 = local_58;
  local_68 = (undefined *******)&PTR_FUN_0da7eed0;
  if (local_58 != (long *)0x0) {
    LOAcquire();
    lVar3 = local_58[1];
    local_58[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_58 + 2;
      (**(code **)(*local_58 + 0x10))(local_58);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e524;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e524:
  plVar1 = plStack_78;
  if ((local_70 != 0) && (plStack_78 != (long *)0x0)) {
    LOAcquire();
    lVar3 = plStack_78[1];
    plStack_78[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = plStack_78 + 2;
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e544;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e544:
  IVar6 = _objc_alloc(&objc::class_t::ArcBackForwardList);
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  FUN_042aec00(&local_80,local_68[0x5f]);
  IVar6 = ArcBackForwardList::initWithBackForwardList_(IVar6,"initWithBackForwardList:",&local_80);
  _objc_msgSend(param_1,"setBackForwardList:",IVar6);
  plVar1 = plStack_78;
  if (plStack_78 != (long *)0x0) {
    LOAcquire();
    lVar3 = plStack_78[1];
    plStack_78[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = plStack_78 + 2;
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e608;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e608:
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e628;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e628:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  _objc_msgSend(param_1,"setUndoableFieldActive:",(ulong)*(byte *)((long)local_68 + 0x171));
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e678;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e678:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  _objc_msgSend(param_1,"setFreeformTextFieldActive:",(ulong)*(byte *)((long)local_68 + 0x172));
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e6c8;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e6c8:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  _objc_msgSend(param_1,"setPasswordTextFieldActive:",(ulong)*(byte *)((long)local_68 + 0x173));
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e718;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e718:
  _objc_msgSend(param_1,"web_contents",&local_80);
  if ((local_70 & 1) == 0) goto LAB_0440f264;
  FUN_04378e28(&local_68,local_80);
  IVar6 = _objc_alloc_init(&_OBJC_CLASS___NSMutableIndexSet);
  ppppppppuVar2 = (undefined ********)local_68;
  for (lVar3 = (long)local_60 << 2; lVar3 != 0; lVar3 = lVar3 + -4) {
    if (*(uint *)ppppppppuVar2 < 0xe) {
      NSMutableIndexSet::addIndex_(IVar6,"addIndex:");
    }
    ppppppppuVar2 = (undefined ********)((long)ppppppppuVar2 + 4);
  }
  _objc_msgSend(param_1,"setTabTraits:",IVar6);
  if ((undefined ********)local_68 != (undefined ********)0x0) {
    if (((ulong)local_60 & 0x3fffffffffffffff) != 0) {
      lVar3 = (long)local_60 * -4;
      puVar11 = (uint *)((long)local_68 + (long)local_60 * 4);
      do {
        puVar11 = puVar11 + -1;
        if (puVar11 == (uint *)0x0) goto LAB_0440f264;
        lVar3 = lVar3 + 4;
      } while (lVar3 != 0);
    }
    local_60 = (long *)0x0;
    FUN_00005810(local_68,(long)local_58 << 2);
  }
  plVar1 = plStack_78;
  if ((local_70 != 0) && (plStack_78 != (long *)0x0)) {
    LOAcquire();
    lVar3 = plStack_78[1];
    plStack_78[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = plStack_78 + 2;
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e80c;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e80c:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  if (2 < *(uint *)(local_68 + 0x52)) goto LAB_0440feb0;
  _objc_msgSend(param_1,"setState:");
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e864;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e864:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  uVar8 = 0x3f800000;
  if (((undefined *******)local_68[0x26] != (undefined *******)0x0) &&
     (lVar3 = FUN_00882ba0(), lVar3 != 0)) {
    uVar5 = FUN_04597a58();
    dVar14 = (double)_pow(0x3ff3333333333333,uVar5);
    uVar8 = (ulong)(uint)(float)dVar14;
  }
  _objc_msgSend(param_1,"setZoomLevel:",uVar8);
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e8e0;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e8e0:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  FUN_04377810(auStack_85,local_68);
  uVar5 = FUN_04401bc0(auStack_85);
  _objc_msgSend(param_1,"setThemeColor:",uVar5);
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e950;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e950:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  _objc_msgSend(param_1,"setIsReadyToDisplay:",(ulong)*(byte *)((long)local_68 + 0x174));
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440e9a0;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440e9a0:
  _objc_msgSend(param_1,"web_contents",&local_68);
  plVar1 = local_60;
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  uVar5 = NEON_scvtf(local_68[0x42],4);
  fVar13 = (float)uVar5 / *(float *)((long)local_68 + 0x20c);
  fVar15 = (float)((ulong)uVar5 >> 0x20) / *(float *)((long)local_68 + 0x20c);
  uVar8 = CONCAT44(-(uint)(9.536743e-07 < fVar15),-(uint)(9.536743e-07 < fVar13)) &
          CONCAT44(fVar15,fVar13);
  if (local_60 != (long *)0x0) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440ea34;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440ea34:
  _objc_msgSend(param_1,"setPendingRenderFrameViewportSize:",(double)(float)uVar8,
                (double)(float)(uVar8 >> 0x20));
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  _objc_msgSend(param_1,"setNavigationID:",(ulong)*(uint *)((long)local_68 + 0x5c4));
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440ea9c;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440ea9c:
  local_80 = 0;
  plStack_78 = (long *)0x0;
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  FUN_042aec00(&local_b0,local_68[0x5b]);
  FUN_04366938(&local_80,local_b0);
  if (local_a8 != (long *)0x0) {
    LOAcquire();
    lVar3 = local_a8[1];
    local_a8[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_a8 + 0x10))(local_a8);
      if (local_a8[2] != 0) {
        LOAcquire();
        lVar3 = local_a8[2];
        local_a8[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440eb2c;
      }
      pcVar10 = *(code **)(*local_a8 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(local_a8,0x20);
      }
      else {
        (*pcVar10)(local_a8);
      }
    }
  }
LAB_0440eb2c:
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440eb4c;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440eb4c:
  _objc_msgSend(param_1,"setCanGoBack:",(ulong)(local_80 == 1 & (byte)plStack_78));
  local_b0 = 0;
  local_a8 = (long *)0x0;
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  FUN_042aec00(&local_98,local_68[0x5b]);
  FUN_043669c8(&local_b0,local_98);
  if (local_90 != (long *)0x0) {
    LOAcquire();
    lVar3 = local_90[1];
    local_90[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_90 + 0x10))(local_90);
      if (local_90[2] != 0) {
        LOAcquire();
        lVar3 = local_90[2];
        local_90[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440ec00;
      }
      pcVar10 = *(code **)(*local_90 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(local_90,0x20);
      }
      else {
        (*pcVar10)(local_90);
      }
    }
  }
LAB_0440ec00:
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440ec20;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440ec20:
  _objc_msgSend(param_1,"setCanGoForward:",(ulong)(local_b0 == 1 & (byte)local_a8));
  if ((local_b0 & 1) == 0) {
    FUN_04b35898();
  }
  if ((local_80 & 1) == 0) {
    FUN_04b35898();
  }
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  _objc_msgSend(param_1,"setHadFormOrUserEdits:",(ulong)*(byte *)(local_68 + 0xb9));
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440ecac;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440ecac:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  _objc_msgSend(param_1,"setHasDirtyTextField:",(ulong)*(byte *)((long)local_68 + 0x5c9));
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440ecfc;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440ecfc:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  if (*(char *)(local_68 + 0x4d) == '\0') {
    local_b0 = local_b0 & 0xffffffffffffff00;
    local_a0 = 0;
  }
  else {
    plStack_78 = *(long **)((long)local_68 + 0x184);
    local_80 = *(ulong *)((long)local_68 + 0x17c);
    local_70 = 1;
    FUN_0430dba4(&local_b0,&local_80);
  }
  uVar5 = FUN_04401c4c(&local_b0);
  _objc_msgSend(param_1,"setRootBackgroundColor:",uVar5);
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440ed98;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440ed98:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  if ((undefined *******)local_68[0x26] == (undefined *******)0x0) {
    uVar8 = 0;
  }
  else {
    uVar8 = (ulong)*(uint *)((long)local_68 + 0x5cc);
  }
  _objc_msgSend(param_1,"setBlockedResourceCount:",uVar8);
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440edf0;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440edf0:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  uVar9 = 0;
  if (((undefined *******)local_68[0x26] != (undefined *******)0x0) &&
     (uVar9 = *(uint *)(local_68[0x26] + 0x15c), 2 < uVar9)) {
LAB_0440feb0:
                    /* WARNING: Does not return */
    pcVar10 = (code *)SoftwareBreakpoint(0x550b,0x440feb4);
    (*pcVar10)();
  }
  _objc_msgSend(param_1,"setVisibility:",uVar9);
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440ee54;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440ee54:
  _objc_msgSend(param_1,"web_contents",&local_68);
  if (((ulong)local_58 & 1) == 0) goto LAB_0440f264;
  _objc_msgSend(param_1,"setHasFocus:",(ulong)*(byte *)(local_68 + 0xba));
  plVar1 = local_60;
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar12 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar12 != 0) {
        LOAcquire();
        lVar3 = plVar1[2];
        plVar1[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_0440eea4;
      }
      pcVar10 = *(code **)(*plVar1 + 0x20);
      if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar10)(plVar1);
      }
    }
  }
LAB_0440eea4:
  puVar7 = (undefined8 *)FUN_00008790(0x28);
  puVar7[1] = 0;
  puVar7[2] = 0;
  *puVar7 = &PTR_viewWillStartLiveResize_0da87f20;
  if (puVar7 + 3 != (undefined8 *)0x0) {
    puVar7[3] = &PTR_FUN_0da87f58;
    _objc_initWeak(puVar7 + 4,param_1);
    _objc_msgSend(param_1,"setWebContentsObserver:",puVar7 + 3,puVar7);
    _objc_msgSend(param_1,"web_contents",&local_68);
    if (((ulong)local_58 & 1) != 0) {
      IVar6 = _objc_msgSend(param_1,"webContentsObserver");
      if (extraout_x1 == (long *)0x0) {
        FUN_04345dc8(&local_68,IVar6,0);
      }
      else {
        lVar3 = extraout_x1[2];
        extraout_x1[2] = lVar3 + 1;
        FUN_04345dc8(lVar3,&local_68,IVar6,extraout_x1);
        LOAcquire();
        lVar3 = extraout_x1[1];
        extraout_x1[1] = lVar3 + -1;
        LORelease();
        if (lVar3 == 0) {
          (**(code **)(*extraout_x1 + 0x10))(extraout_x1);
          if (extraout_x1[2] != 0) {
            LOAcquire();
            lVar3 = extraout_x1[2];
            extraout_x1[2] = lVar3 + -1;
            LORelease();
            if (lVar3 != 0) goto LAB_0440efc8;
          }
          pcVar10 = *(code **)(*extraout_x1 + 0x20);
          if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40))
          {
            FUN_00005810(extraout_x1,0x20);
          }
          else {
            (*pcVar10)(extraout_x1);
          }
        }
      }
LAB_0440efc8:
      plVar1 = local_60;
      if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
        LOAcquire();
        lVar3 = local_60[1];
        local_60[1] = lVar3 + -1;
        LORelease();
        if (lVar3 == 0) {
          plVar12 = local_60 + 2;
          (**(code **)(*local_60 + 0x10))(local_60);
          if (*plVar12 != 0) {
            LOAcquire();
            lVar3 = plVar1[2];
            plVar1[2] = lVar3 + -1;
            LORelease();
            if (lVar3 != 0) {
              return 0;
            }
          }
          pcVar10 = *(code **)(*plVar1 + 0x20);
          if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) || (pcVar10 == FUN_013efb40))
          {
            FUN_00005810(plVar1,0x20);
          }
          else {
            (*pcVar10)(plVar1);
          }
        }
      }
      return 0;
    }
  }
LAB_0440f264:
                    /* WARNING: Does not return */
  pcVar10 = (code *)SoftwareBreakpoint(1,0x440f268);
  (*pcVar10)();
}


