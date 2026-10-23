// ==== 0x473d7e4  FUN_0473d7e4  (424 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0473d7e4(ID *param_1)

{
  ID IVar1;
  ID IVar2;
  long lVar3;
  
  IVar2 = *param_1;
  if (((IVar2 == 0) || (IVar1 = _objc_msgSend(IVar2,"window"), IVar1 == 0)) ||
     (IVar1 = _objc_msgSend(IVar2,"acceptsFirstResponder"), (int)IVar1 == 0)) {
    IVar1 = *param_1;
    IVar2 = _objc_msgSend((ID)&objc::class_t::WebContentsHostView,"class");
    IVar2 = _objc_msgSend(IVar1,"isMemberOfClass:",IVar2);
    if ((IVar2 & 1) != 0) {
      IVar2 = _objc_msgSend(IVar1,"knownSubviews");
      IVar2 = _objc_msgSend(IVar2,"firstObject");
      IVar1 = _objc_msgSend(IVar2,"conformsToProtocol:",&objc::protocol_t::ViewsHostable);
      if ((int)IVar1 == 0) {
        lVar3 = 0;
      }
      else {
        IVar2 = _objc_msgSend(IVar2,"viewsHostableView");
        lVar3 = 0;
        if (IVar2 != 0) {
          lVar3 = IVar2 - 0x20;
        }
      }
      if (lVar3 != 0) {
        FUN_06854a88(lVar3);
      }
    }
  }
  else {
    IVar1 = _objc_msgSend(IVar2,"window");
    _objc_msgSend(IVar1,"makeFirstResponder:",IVar2);
  }
  return 0;
}


