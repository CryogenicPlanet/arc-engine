// ==== 0x1004ae4a0  ADK.ADKApplication.method_instance  (204 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ID ADK_ADKApplication_method_instance(ID param_1,undefined8 param_2)

{
  undefined *puVar1;
  void *aBlock;
  ID IVar2;
  undefined *local_60;
  undefined8 local_58;
  code *local_50;
  undefined *puStack_48;
  code *local_40;
  undefined *local_38;
  
  IVar2 = param_1;
  ADK_ADKApplication_arcBrowserApplication_getter();
  puVar1 = &DAT_104184908;
  _swift_allocObject(&DAT_104184908,0x20,7);
  *(ID *)(puVar1 + 0x10) = param_1;
  *(undefined8 *)(puVar1 + 0x18) = param_2;
  local_40 = FUN_1000939c8;
  local_60 = PTR___NSConcreteStackBlock_10415f2f0;
  local_58 = 0x42000000;
  local_50 = FUN_10040a3f8;
  puStack_48 = &DAT_104184920;
  local_38 = puVar1;
  aBlock = __Block_copy(&local_60);
  puVar1 = local_38;
  _swift_retain(param_2);
  _swift_release(puVar1);
  IVar2 = objc_stub::tryToTerminate_(IVar2,aBlock);
  __Block_release(aBlock);
  return IVar2;
}


