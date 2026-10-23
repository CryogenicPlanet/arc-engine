// ==== 0x4739114  FUN_04739114  (160 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04739114(ID param_1,undefined8 *param_2)

{
  ID IVar1;
  undefined8 local_28;
  
  IVar1 = _objc_msgSend((ID)&objc::class_t::WebContentsPaneHostView,"class");
  IVar1 = _objc_msgSend(param_1,"isMemberOfClass:",IVar1);
  if ((int)IVar1 != 0) {
    local_28 = *param_2;
    _objc_msgSend(param_1,"attachWebContentsHostView:",&local_28);
  }
  return 0;
}


