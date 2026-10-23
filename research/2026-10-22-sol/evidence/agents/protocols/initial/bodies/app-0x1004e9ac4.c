// ==== 0x1004e9ac4  FUN_1004e9ac4  (704 bytes)

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

void FUN_1004e9ac4(ID param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,long param_5,
                  void *param_6)

{
  code *pcVar1;
  bool bVar2;
  undefined *puVar3;
  long lVar4;
  ID self;
  ID IVar5;
  NSString *pNVar6;
  long lVar7;
  undefined8 uVar8;
  _StringGuts _Var9;
  undefined8 uVar10;
  long lVar11;
  long lVar12;
  String SVar13;
  undefined8 local_c8 [3];
  undefined8 local_b0;
  undefined **ppuStack_a8;
  undefined1 auStack_a0 [24];
  undefined1 local_88 [8];
  String local_80;
  undefined8 local_70;
  long lStack_68;
  
  puVar3 = &DAT_104187ef8;
  _swift_allocObject(&DAT_104187ef8,0x18,7);
  *(void **)(puVar3 + 0x10) = param_6;
  param_5 = param_5 + _TtC3ADK36BrowserContextDownloadDelegateBridge::delegate;
  _swift_beginAccess(param_5,auStack_a0,0,0);
  lVar4 = param_5;
  _swift_unknownObjectWeakLoadStrong();
  lVar11 = *(long *)(param_5 + 8);
  __Block_copy(param_6);
  if (lVar4 == 0) {
    pNVar6 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
    (**(code **)((long)param_6 + 0x10))(param_6,pNVar6);
    _swift_release(puVar3);
    return;
  }
  self = objc_stub::id(param_1);
  IVar5 = objc_stub::isIncognito(self);
  objc_stub::directoryBasename(self);
  SVar13 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  _Var9.unknown = SVar13.bridgeObject;
  if (DAT_1044c8240 != -1) {
    _swift_once(&DAT_1044c8240,FUN_1004aa82c);
  }
  lVar12 = ((undefined8 *)(DAT_1044c8248 + _TtC3ADK14ADKApplication::machineID))[1];
  if (lVar12 == 0) {
    __Block_release(param_6);
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x1004e9d84);
    (*pcVar1)();
  }
  uVar10 = *(undefined8 *)(DAT_1044c8248 + _TtC3ADK14ADKApplication::machineID);
  if ((SVar13.str == (undefined *)0x746c7561666544) && (_Var9.unknown == &DAT_e700000000000000)) {
    _swift_bridgeObjectRelease(&DAT_e700000000000000);
  }
  else {
    bVar2 = Swift::_stringCompareWithSmolCheck
                      ((_StringGuts)SVar13.str,_Var9,(_StringComparisonResult)0x44);
    _swift_bridgeObjectRetain(lVar12);
    if (!bVar2) goto LAB_1004e9c90;
    _swift_bridgeObjectRelease(_Var9.unknown);
    _swift_bridgeObjectRelease(lVar12);
  }
  SVar13 = (String)ZEXT816(0);
  uVar10 = 0;
  lVar12 = 0;
LAB_1004e9c90:
  lVar7 = lVar4;
  _swift_getObjectType(lVar4);
  local_88[0] = (undefined1)IVar5;
  uVar8 = 0;
  local_80 = SVar13;
  local_70 = uVar10;
  lStack_68 = lVar12;
  FUN_10052e98c();
  _objc_allocWithZone();
  FUN_10052db40();
  ppuStack_a8 = &PTR_PTR_10418ca38;
  local_c8[0] = param_2;
  local_b0 = uVar8;
  (**(code **)(lVar11 + 0x28))(local_88,local_c8,param_3,param_4,FUN_1004e9da8,puVar3,lVar7);
  _swift_unknownObjectRelease(lVar4);
  FUN_10000825c(SVar13.str,SVar13.bridgeObject,uVar10,lVar12);
  FUN_100012e70(local_c8);
  _swift_release(puVar3);
  return;
}


