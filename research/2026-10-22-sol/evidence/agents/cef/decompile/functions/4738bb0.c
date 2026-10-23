// ==== 0x4738bb0  WebContentsPaneHostView::syncHostedWebContentsState  (1088 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void WebContentsPaneHostView::syncHostedWebContentsState(ID param_1,SEL param_2)

{
  undefined1 auVar1 [16];
  ID IVar2;
  ID IVar3;
  ID *pIVar4;
  ID *pIVar5;
  uint uVar6;
  long lVar7;
  double extraout_d0;
  double extraout_d0_00;
  double extraout_d0_01;
  double extraout_d0_02;
  undefined4 in_s1;
  undefined4 in_register_00005024;
  double in_d2;
  double in_d3;
  float fVar8;
  float fVar10;
  undefined1 local_80 [16];
  undefined8 local_70;
  ulong local_68;
  double dVar9;
  
  lVar7 = (long)bridge_;
  if (*(long *)(param_1 + lVar7) == 0) {
    return;
  }
  if (*(long *)(param_1 + (long)attached_web_contents_host_view_) == 0) {
    return;
  }
  IVar2 = _objc_msgSend(param_1,"window");
  if (IVar2 == 0) {
    uVar6 = 0;
  }
  else {
    IVar2 = _objc_msgSend(param_1,"isHiddenOrHasHiddenAncestor");
    uVar6 = (uint)IVar2 ^ 1;
  }
  _objc_msgSend(param_1,"bounds");
  _objc_msgSend(param_1,"bounds");
  _objc_msgSend(param_1,"bounds");
  _objc_msgSend(param_1,"bounds");
  local_70 = (ID *)CONCAT44((float)(double)CONCAT44(in_register_00005024,in_s1),(float)extraout_d0);
  auVar1._8_4_ = SUB84(in_d3,0);
  auVar1._0_8_ = in_d2;
  auVar1._12_4_ = (int)((ulong)in_d3 >> 0x20);
  local_68 = CONCAT44(-(uint)(9.536743732496689e-07 < in_d3),-(uint)(9.536743732496689e-07 < in_d2))
             & CONCAT44((float)auVar1._8_8_,(float)in_d2);
  local_80 = FUN_001d7a7c(&local_70);
  lVar7 = *(long *)(param_1 + lVar7);
  IVar2 = _objc_msgSend(param_1,"window");
  IVar2 = _objc_msgSend(IVar2,"screen");
  if (IVar2 == 0) {
    IVar2 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
    if (IVar2 == 0) {
      fVar8 = 1.0;
      goto LAB_04738d58;
    }
    IVar2 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
    NSScreen::backingScaleFactor(IVar2,"backingScaleFactor");
    dVar9 = extraout_d0_02;
  }
  else {
    IVar2 = _objc_msgSend(param_1,"window");
    IVar2 = _objc_msgSend(IVar2,"screen");
    _objc_msgSend(IVar2,"backingScaleFactor");
    dVar9 = extraout_d0_00;
  }
  fVar8 = (float)dVar9;
LAB_04738d58:
  IVar2 = _objc_msgSend(param_1,"window");
  IVar2 = _objc_msgSend(IVar2,"screen");
  if (IVar2 == 0) {
    IVar2 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
  }
  IVar2 = _objc_msgSend(IVar2,"deviceDescription");
  IVar2 = _objc_msgSend(IVar2,"objectForKeyedSubscript:",&cf_NSScreenNumber);
  if (IVar2 == 0) {
    IVar2 = 0;
  }
  else {
    IVar2 = _objc_msgSend(IVar2,"longLongValue");
  }
  IVar3 = _objc_msgSend(param_1,"window");
  IVar3 = _objc_msgSend(IVar3,"screen");
  if (IVar3 == 0) {
    IVar3 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
  }
  _objc_msgSend(IVar3,"maximumRefreshInterval");
  if (0.0 < extraout_d0_01) {
    fVar10 = (float)(1.0 / extraout_d0_01);
  }
  else {
    fVar10 = 60.0;
  }
  pIVar4 = (ID *)FUN_00008790(8);
  *pIVar4 = param_1;
  if (*(long *)(lVar7 + 0x68) != 0) {
    pIVar5 = (ID *)FUN_00008790(8);
    *pIVar5 = 0;
    *pIVar5 = *pIVar4;
    local_70 = pIVar5;
    FUN_0473a048(lVar7,&local_70);
    if (pIVar5 != (ID *)0x0) {
      FUN_00005810(pIVar5,8);
    }
    FUN_0473a110(fVar8,fVar10,lVar7,local_80,IVar2);
    FUN_0473a3c0(lVar7,uVar6);
  }
  if (pIVar4 != (ID *)0x0) {
    FUN_00005810(pIVar4,8);
  }
  return;
}


