// ==== 0x100401e54  FUN_100401e54  (200 bytes)

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_100401e54(ID param_1)

{
  ID self;
  NSString *pNVar1;
  void *aBlock;
  undefined *local_60;
  undefined8 local_58;
  code *local_50;
  undefined *puStack_48;
  code *local_40;
  undefined8 uStack_38;
  
  self = objc_stub::extensions(param_1);
  pNVar1 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
  local_40 = ARCUI_MovableWindowController_method_instance;
  uStack_38 = 0;
  local_60 = PTR___NSConcreteStackBlock_10415f2f0;
  local_58 = 0x42000000;
  local_50 = FUN_100401f1c;
  puStack_48 = &DAT_10417fc18;
  aBlock = __Block_copy(&local_60);
  objc_stub::installExtensionFromWebstore_unattended_completion_(self,pNVar1,1,aBlock);
  __Block_release(aBlock);
  return 0;
}


