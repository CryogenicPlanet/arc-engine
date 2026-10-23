// ==== 0x100522a80  FUN_100522a80  (228 bytes)

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_100522a80(undefined8 param_1,char param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 param_5)

{
  ID IVar1;
  NSString *pNVar2;
  ID unaff_x20;
  
  if (param_2 == '\0') {
    pNVar2 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
    IVar1 = objc_stub::createBrowser_withDelegate_forWindow_(unaff_x20,param_1,0,pNVar2);
  }
  else if (param_2 == '\x01') {
    IVar1 = objc_stub::createPopupBrowser_withDelegate_(unaff_x20,param_1,0);
  }
  else {
    pNVar2 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
    IVar1 = objc_stub::createBrowser_withDelegate_forWindow_(unaff_x20,param_1,0,pNVar2);
  }
  FUN_10052de00(0);
  _objc_allocWithZone();
  FUN_100525930(IVar1,param_5);
  return;
}


