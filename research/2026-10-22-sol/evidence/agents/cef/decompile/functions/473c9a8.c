// ==== 0x473c9a8  FUN_0473c9a8  (284 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */

bool FUN_0473c9a8(ID *param_1)

{
  ID IVar1;
  ID IVar2;
  bool bVar3;
  
  IVar2 = *param_1;
  IVar1 = _objc_msgSend((ID)&objc::class_t::WebContentsHostView,"class");
  IVar1 = _objc_msgSend(IVar2,"isMemberOfClass:",IVar1);
  if ((int)IVar1 == 0) {
    bVar3 = false;
  }
  else {
    IVar1 = _objc_msgSend(IVar2,"knownSubviews");
    IVar1 = _objc_msgSend(IVar1,"firstObject");
    IVar2 = _objc_msgSend(IVar1,"conformsToProtocol:",&objc::protocol_t::ViewsHostable);
    if ((int)IVar2 == 0) {
      bVar3 = false;
    }
    else {
      IVar1 = _objc_msgSend(IVar1,"viewsHostableView");
      bVar3 = IVar1 != 0 && IVar1 != 0x20;
    }
  }
  return bVar3;
}


