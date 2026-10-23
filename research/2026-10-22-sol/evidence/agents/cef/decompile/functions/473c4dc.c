// ==== 0x473c4dc  FUN_0473c4dc  (1144 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */

WebContentsHostView *
FUN_0473c4dc(undefined8 *param_1,int *param_2,WeakReference param_3,undefined8 param_4,
            WebContentsViewObserver *param_5)

{
  code *pcVar1;
  int iVar2;
  ID IVar3;
  ID IVar4;
  WebContentsHostView *this;
  undefined8 uVar5;
  ID IVar6;
  WeakPtr<arc::browser::WebContentsViewObserver> WVar7;
  ID local_88;
  ID local_80;
  ID local_78;
  ID local_70;
  long local_68;
  
  local_68 = *(long *)PTR____stack_chk_guard_0d824020;
  IVar6 = *(ID *)*param_1;
  IVar3 = _objc_msgSend(IVar6,"superview");
  IVar4 = _objc_msgSend((ID)&objc::class_t::WebContentsHostView,"class");
  IVar3 = _objc_msgSend(IVar3,"isMemberOfClass:",IVar4);
  if ((int)IVar3 == 0) {
    IVar3 = _objc_alloc(&objc::class_t::WebContentsHostView);
    _objc_msgSend(IVar6,"frame");
    this = (WebContentsHostView *)_objc_msgSend(IVar3,"initWithFrame:");
    if (((param_2 != (int *)0x0) && ((char)param_2[1] == '\0')) &&
       (param_3.field0_0x0.field0_0x0 != (Flag *)0x0)) {
      iVar2 = *param_2;
      *param_2 = iVar2 + 1;
      if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x473c9a0);
        (*pcVar1)();
      }
      if (this == (WebContentsHostView *)0x0) {
        LOAcquire();
        iVar2 = *param_2;
        *param_2 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          FUN_00005810(param_2,8);
        }
      }
      else {
        WVar7.field1_0x8 = param_5;
        WVar7.field0_0x0.field0_0x0.field0_0x0 = param_3.field0_0x0.field0_0x0;
        WebContentsHostView::setViewObserver_(this,(ID)"setViewObserver:",(SEL)param_2,WVar7);
      }
    }
    WebContentsHostView::setIsKnownSubview_((ID)this,"setIsKnownSubview:",IVar6);
    _objc_msgSend((ID)this,"addSubview:",IVar6);
    _objc_msgSend(IVar6,"setTranslatesAutoresizingMaskIntoConstraints:",0);
    IVar3 = _objc_msgSend(IVar6,"leadingAnchor");
    IVar4 = _objc_msgSend((ID)this,"leadingAnchor");
    local_88 = _objc_msgSend(IVar3,"constraintEqualToAnchor:",IVar4);
    IVar3 = _objc_msgSend(IVar6,"trailingAnchor");
    IVar4 = _objc_msgSend((ID)this,"trailingAnchor");
    local_80 = _objc_msgSend(IVar3,"constraintEqualToAnchor:",IVar4);
    IVar3 = _objc_msgSend(IVar6,"topAnchor");
    IVar4 = _objc_msgSend((ID)this,"topAnchor");
    local_78 = _objc_msgSend(IVar3,"constraintEqualToAnchor:",IVar4);
    IVar6 = _objc_msgSend(IVar6,"bottomAnchor");
    IVar3 = _objc_msgSend((ID)this,"bottomAnchor");
    local_70 = _objc_msgSend(IVar6,"constraintEqualToAnchor:",IVar3);
    IVar6 = NSArray::arrayWithObjects_count_
                      ((ID)&_OBJC_CLASS___NSArray,"arrayWithObjects:count:",&local_88,4);
    _objc_msgSend((ID)this,"addConstraints:",IVar6);
    uVar5 = 0;
    if (param_2 == (int *)0x0) goto LAB_0473c900;
  }
  else {
    this = (WebContentsHostView *)_objc_msgSend(IVar6,"superview");
    iVar2 = FUN_020cf42c(0x1d);
    if ((iVar2 != 0) && (IVar3 = _objc_msgSend((ID)this,"isKnownSubview:",IVar6), (IVar3 & 1) == 0))
    {
      _objc_msgSend((ID)this,"setIsKnownSubview:",IVar6);
    }
    if (param_2 == (int *)0x0) {
      uVar5 = 0;
      goto LAB_0473c900;
    }
    if (((char)param_2[1] == '\0') && (param_3.field0_0x0.field0_0x0 != (Flag *)0x0)) {
      iVar2 = *param_2;
      *param_2 = iVar2 + 1;
      if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x473c994);
        (*pcVar1)();
      }
      if (this == (WebContentsHostView *)0x0) {
        LOAcquire();
        iVar2 = *param_2;
        *param_2 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          FUN_00005810(param_2,8);
        }
      }
      else {
        _objc_msgSend((ID)this,"setViewObserver:",param_2,param_3.field0_0x0.field0_0x0);
      }
    }
  }
  uVar5 = 0;
  LOAcquire();
  iVar2 = *param_2;
  *param_2 = iVar2 + -1;
  LORelease();
  if (iVar2 == 1) {
    uVar5 = FUN_00005810(param_2,8);
  }
LAB_0473c900:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_68) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(uVar5);
  }
  return this;
}


