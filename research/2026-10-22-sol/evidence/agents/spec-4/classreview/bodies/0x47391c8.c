// ==== 0x47391c8  FUN_047391c8  (96 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_047391c8(ID param_1)

{
  ID IVar1;
  
  IVar1 = _objc_msgSend((ID)&objc::class_t::WebContentsPaneHostView,"class");
  IVar1 = _objc_msgSend(param_1,"isMemberOfClass:",IVar1);
  if ((int)IVar1 != 0) {
    _objc_msgSend(param_1,"detachWebContentsHostView");
  }
  return 0;
}


