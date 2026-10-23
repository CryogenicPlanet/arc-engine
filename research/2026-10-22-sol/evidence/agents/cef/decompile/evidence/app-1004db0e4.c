// ==== 0x1004db0e4  FUN_1004db0e4  (256 bytes)

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8
FUN_1004db0e4(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  ID self;
  NSString *pNVar1;
  undefined *puVar2;
  void *aBlock;
  ID unaff_x20;
  undefined *local_70;
  undefined8 local_68;
  code *local_60;
  undefined *puStack_58;
  code *local_50;
  undefined *local_48;
  
  self = objc_stub::extensions(unaff_x20);
  pNVar1 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
  puVar2 = &DAT_104187500;
  _swift_allocObject(&DAT_104187500,0x20,7);
  *(undefined8 *)(puVar2 + 0x10) = param_3;
  *(undefined8 *)(puVar2 + 0x18) = param_4;
  local_50 = thunk_FUN_1004e0100;
  local_70 = PTR___NSConcreteStackBlock_10415f2f0;
  local_68 = 0x42000000;
  local_60 = FUN_100401f1c;
  puStack_58 = &DAT_104187518;
  local_48 = puVar2;
  aBlock = __Block_copy(&local_70);
  puVar2 = local_48;
  _swift_retain(param_4);
  _swift_release(puVar2);
  objc_stub::installExtensionFromWebstore_unattended_completion_(self,pNVar1,1,aBlock);
  __Block_release(aBlock);
  return 0;
}


