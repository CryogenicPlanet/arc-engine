// ==== 0x4737eb4  WebContentsPaneHostView::attachWebContentsHostView:  (2656 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x18 bytes */

void WebContentsPaneHostView::attachWebContentsHostView_(ID param_1,SEL param_2,NativeView param_3)

{
  undefined1 auVar1 [16];
  float fVar2;
  code *pcVar3;
  long lVar4;
  ulong uVar5;
  ID IVar6;
  ID *pIVar7;
  ID IVar8;
  ID *pIVar9;
  ID *pIVar10;
  ID *pIVar11;
  long *plVar12;
  long lVar13;
  ID IVar14;
  long lVar15;
  uint uVar16;
  double extraout_d0;
  double extraout_d0_00;
  double extraout_d0_01;
  double extraout_d0_02;
  undefined4 in_s1;
  undefined4 in_register_00005024;
  double in_d2;
  double in_d3;
  float fVar17;
  undefined1 local_f0 [16];
  undefined8 local_e0;
  long local_d8;
  undefined8 local_d0;
  ulong local_c8;
  ID *local_c0;
  long local_b8;
  long local_b0;
  long local_a8;
  ID local_a0;
  ID local_98;
  ID local_90;
  ID local_88;
  long local_80;
  double dVar18;
  
  local_80 = *(long *)PTR____stack_chk_guard_0d824020;
  lVar13 = (long)attached_web_contents_host_view_;
  lVar4 = *(long *)param_3.field0_0x0;
  if (*(long *)(param_1 + lVar13) == lVar4) {
    if (*(long *)PTR____stack_chk_guard_0d824020 == local_80) {
      syncHostedWebContentsState(param_1,"syncHostedWebContentsState");
      return;
    }
    goto LAB_0473890c;
  }
  lVar15 = (long)bridge_;
  if (*(long *)(param_1 + lVar13) != 0 && *(long *)(param_1 + lVar15) != 0) {
    local_e0 = *(undefined8 *)(param_1 + lVar13);
    local_d8 = lVar4;
    FUN_0473cad8(&local_d8,&local_e0);
  }
  detachWebContentsHostView(param_1,"detachWebContentsHostView");
  IVar14 = *(ID *)(param_1 + lVar13);
                    /* WARNING: Load size is inaccurate */
  if (IVar14 != *param_3.field0_0x0) {
    *(ID *)(param_1 + lVar13) = *param_3.field0_0x0;
    IVar14 = *(ID *)(param_1 + lVar13);
  }
  if (IVar14 == 0) {
    *(undefined8 *)(param_1 + lVar13) = 0;
  }
  else {
    uVar5 = FUN_00014cc0(&PTR_s_ArcMacCoalescedResizing_0d41f79e_1_0e304a40);
    if ((uVar5 & 1) != 0) {
      _objc_msgSend(IVar14,"setTranslatesAutoresizingMaskIntoConstraints:",0);
      _objc_msgSend(param_1,"bounds");
      _objc_msgSend(IVar14,"setFrame:");
    }
    local_f0._0_8_ = 0;
    local_f0._8_8_ = 0;
    _objc_msgSend(param_1,"bounds");
    _objc_msgSend(param_1,"bounds");
    _objc_msgSend(param_1,"bounds");
    _objc_msgSend(param_1,"bounds");
    local_d0 = (ID *)CONCAT44((float)(double)CONCAT44(in_register_00005024,in_s1),
                              (float)extraout_d0_00);
    auVar1._8_4_ = SUB84(in_d3,0);
    auVar1._0_8_ = in_d2;
    auVar1._12_4_ = (int)((ulong)in_d3 >> 0x20);
    local_c8 = CONCAT44(-(uint)(9.536743732496689e-07 < in_d3),
                        -(uint)(9.536743732496689e-07 < in_d2)) &
               CONCAT44((float)auVar1._8_8_,(float)in_d2);
    local_f0 = FUN_001d7a7c(&local_d0);
    IVar6 = _objc_msgSend(param_1,"window");
    IVar6 = _objc_msgSend(IVar6,"screen");
    if (IVar6 == 0) {
      IVar6 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
      if (IVar6 != 0) {
        IVar6 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
        NSScreen::backingScaleFactor(IVar6,"backingScaleFactor");
        dVar18 = extraout_d0;
        goto LAB_04738204;
      }
      fVar17 = 1.0;
    }
    else {
      IVar6 = _objc_msgSend(param_1,"window");
      IVar6 = _objc_msgSend(IVar6,"screen");
      _objc_msgSend(IVar6,"backingScaleFactor");
      dVar18 = extraout_d0_01;
LAB_04738204:
      fVar17 = (float)dVar18;
    }
    IVar6 = _objc_msgSend(param_1,"window");
    IVar6 = _objc_msgSend(IVar6,"screen");
    if (IVar6 == 0) {
      IVar6 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
    }
    _objc_msgSend(IVar6,"maximumRefreshInterval");
    fVar2 = 60.0;
    if (0.0 < extraout_d0_02) {
      fVar2 = (float)(1.0 / extraout_d0_02);
    }
    IVar6 = _objc_msgSend(param_1,"window");
    IVar6 = _objc_msgSend(IVar6,"screen");
    if (IVar6 == 0) {
      IVar6 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
    }
    IVar6 = _objc_msgSend(IVar6,"deviceDescription");
    IVar6 = _objc_msgSend(IVar6,"objectForKeyedSubscript:",&cf_NSScreenNumber);
    if (IVar6 == 0) {
      IVar6 = 0;
    }
    else {
      IVar6 = _objc_msgSend(IVar6,"longLongValue");
    }
    pIVar7 = (ID *)FUN_00008790(8);
    *pIVar7 = param_1;
    lVar4 = *(long *)(param_1 + lVar15);
    if (lVar4 != 0) {
      lVar13 = *(long *)(param_1 + lVar13);
      IVar8 = _objc_msgSend(param_1,"window");
      if (IVar8 == 0) {
        uVar16 = 0;
      }
      else {
        IVar8 = _objc_msgSend(param_1,"isHiddenOrHasHiddenAncestor");
        uVar16 = (uint)IVar8 ^ 1;
      }
      pIVar9 = (ID *)FUN_00008790(8);
      *pIVar9 = 0;
      *pIVar9 = *pIVar7;
      if (lVar13 == 0) {
                    /* WARNING: Does not return */
        pcVar3 = (code *)SoftwareBreakpoint(0,0x4738a28);
        (*pcVar3)();
      }
      plVar12 = (long *)(lVar4 + 0x78);
      if (*plVar12 == lVar13) {
        pIVar10 = (ID *)FUN_00008790(8);
        *pIVar10 = 0;
        *pIVar10 = *pIVar9;
        if (*(long *)(lVar4 + 0x68) != 0) {
          pIVar11 = (ID *)FUN_00008790(8);
          *pIVar11 = 0;
          *pIVar11 = *pIVar10;
          local_d0 = pIVar11;
          FUN_0473a048(lVar4,&local_d0);
          if (pIVar11 != (ID *)0x0) {
            FUN_00005810(pIVar11,8);
          }
          FUN_0473a110(fVar17,fVar2,lVar4,local_f0,IVar6);
          FUN_0473a3c0(lVar4,uVar16);
        }
      }
      else {
        if (*plVar12 != 0) {
          local_b0 = *plVar12;
          local_a8 = lVar13;
          FUN_0473cad8(&local_a8,&local_b0);
        }
        FUN_04738ad4(lVar4);
        local_b8 = *plVar12;
        if (local_b8 != lVar13) {
          *plVar12 = lVar13;
          local_b8 = *plVar12;
        }
        local_c0 = (ID *)FUN_00008790(8);
        *local_c0 = 0;
        *local_c0 = *pIVar9;
        FUN_0473cf0c(&local_b8,lVar4,local_f0,uVar16,&local_c0);
        pIVar10 = local_c0;
        local_c0 = (ID *)0x0;
        if (pIVar10 != (ID *)0x0) {
          FUN_00005810(pIVar10,8);
        }
        pIVar10 = (ID *)FUN_00008790(8);
        *pIVar10 = 0;
        *pIVar10 = *pIVar9;
        if (*(long *)(lVar4 + 0x68) != 0) {
          pIVar11 = (ID *)FUN_00008790(8);
          *pIVar11 = 0;
          *pIVar11 = *pIVar10;
          local_d0 = pIVar11;
          FUN_0473a048(lVar4,&local_d0);
          if (pIVar11 != (ID *)0x0) {
            FUN_00005810(pIVar11,8);
          }
          FUN_0473a110(fVar17,fVar2,lVar4,local_f0,IVar6);
          FUN_0473a3c0(lVar4,uVar16);
        }
      }
      local_d0 = (ID *)0x0;
      if (pIVar10 != (ID *)0x0) {
        FUN_00005810(pIVar10,8);
      }
      if (pIVar9 != (ID *)0x0) {
        FUN_00005810(pIVar9,8);
      }
    }
    _objc_msgSend(param_1,"addSubview:positioned:relativeTo:",IVar14,0xffffffffffffffff,
                  *(undefined8 *)(param_1 + (long)compositor_overlay_view_));
    _objc_msgSend(IVar14,"setTranslatesAutoresizingMaskIntoConstraints:",0);
    IVar6 = _objc_msgSend(IVar14,"leadingAnchor");
    IVar8 = _objc_msgSend(param_1,"leadingAnchor");
    local_a0 = _objc_msgSend(IVar6,"constraintEqualToAnchor:",IVar8);
    IVar6 = _objc_msgSend(IVar14,"trailingAnchor");
    IVar8 = _objc_msgSend(param_1,"trailingAnchor");
    local_98 = _objc_msgSend(IVar6,"constraintEqualToAnchor:",IVar8);
    IVar6 = _objc_msgSend(IVar14,"topAnchor");
    IVar8 = _objc_msgSend(param_1,"topAnchor");
    local_90 = _objc_msgSend(IVar6,"constraintEqualToAnchor:",IVar8);
    IVar14 = _objc_msgSend(IVar14,"bottomAnchor");
    IVar6 = _objc_msgSend(param_1,"bottomAnchor");
    local_88 = _objc_msgSend(IVar14,"constraintEqualToAnchor:",IVar6);
    IVar14 = NSArray::arrayWithObjects_count_
                       ((ID)&_OBJC_CLASS___NSArray,"arrayWithObjects:count:",&local_a0,4);
    _objc_msgSend(param_1,"addConstraints:",IVar14);
    IVar14 = _objc_msgSend(param_1,"window");
    IVar14 = _objc_msgSend(IVar14,"firstResponder");
    if (((IVar14 == param_1) && (*(long *)(param_1 + lVar15) != 0)) &&
       (lVar4 = *(long *)(*(long *)(param_1 + lVar15) + 0x78), lVar4 != 0)) {
      local_d0 = (ID *)lVar4;
      FUN_0473d7e4(&local_d0);
    }
    syncHostedWebContentsState(param_1,"syncHostedWebContentsState");
    if (pIVar7 != (ID *)0x0) {
      FUN_00005810(pIVar7,8);
    }
  }
  lVar4 = 0;
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_80) {
    return;
  }
LAB_0473890c:
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(lVar4);
}


