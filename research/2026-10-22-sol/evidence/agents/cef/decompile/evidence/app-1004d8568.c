// ==== 0x1004d8568  FUN_1004d8568  (368 bytes)

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_1004d8568(code *param_1,undefined8 param_2,long param_3)

{
  undefined *puVar1;
  ID IVar2;
  NSString *pNVar3;
  undefined *puVar4;
  void *aBlock;
  undefined *local_88;
  ulong local_80;
  code *local_78;
  undefined *puStack_70;
  code *local_68;
  undefined *local_60;
  undefined1 auStack_58 [24];
  
  puVar1 = &DAT_104187870;
  _swift_allocObject(&DAT_104187870,0x20,7);
  *(code **)(puVar1 + 0x10) = param_1;
  *(undefined8 *)(puVar1 + 0x18) = param_2;
  _swift_beginAccess(param_3 + 0x10,auStack_58,0,0);
  IVar2 = param_3 + 0x10;
  _swift_unknownObjectWeakLoadStrong();
  _swift_retain(param_2);
  if (IVar2 == 0) {
    _swift_release(puVar1);
    local_88 = (undefined *)0x2;
    local_80 = local_80 & 0xffffffffffffff00;
    (*param_1)(&local_88);
  }
  else {
    IVar2 = objc_stub::extensions(IVar2);
    pNVar3 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
    puVar4 = &DAT_104187898;
    _swift_allocObject(&DAT_104187898,0x20,7);
    *(code **)(puVar4 + 0x10) = thunk_FUN_1004c3708;
    *(undefined **)(puVar4 + 0x18) = puVar1;
    local_68 = FUN_1004e05a4;
    local_88 = PTR___NSConcreteStackBlock_10415f2f0;
    local_80 = 0x42000000;
    local_78 = FUN_100401f1c;
    puStack_70 = &DAT_1041878b0;
    local_60 = puVar4;
    aBlock = __Block_copy(&local_88);
    _swift_release(local_60);
    objc_stub::installExtensionFromWebstore_unattended_completion_(IVar2,pNVar3,1,aBlock);
    __Block_release(aBlock);
  }
  return;
}


