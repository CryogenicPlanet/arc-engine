// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1001.c:5367
// ==== 0x1001066ec FUN_1001066ec

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

void FUN_1001066ec(undefined8 param_1)

{
  undefined1 uVar1;
  long lVar2;
  undefined *puVar3;
  void *aBlock;
  long unaff_x20;
  undefined *local_70;
  undefined8 local_68;
  code *local_60;
  undefined *puStack_58;
  code *local_50;
  undefined *local_48;
  
  lVar2 = _TtC3Arc10BrowserApp::isHandlingSendEvent_;
  uVar1 = *(undefined1 *)(unaff_x20 + _TtC3Arc10BrowserApp::isHandlingSendEvent_);
  *(undefined1 *)(unaff_x20 + _TtC3Arc10BrowserApp::isHandlingSendEvent_) = 1;
  puVar3 = &DAT_10416fa08;
  _swift_allocObject(&DAT_10416fa08,0x20,7);
  *(long *)(puVar3 + 0x10) = unaff_x20;
  *(undefined8 *)(puVar3 + 0x18) = param_1;
  local_50 = FUN_1001079c8;
  local_70 = PTR___NSConcreteStackBlock_10415f2f0;
  local_68 = 0x42000000;
  local_60 = FUN_100213ee4;
  puStack_58 = &DAT_10416fa20;
  local_48 = puVar3;
  aBlock = __Block_copy(&local_70);
  _swift_release(local_48);
  _CallWithEHFrame(aBlock);
  __Block_release(aBlock);
  *(undefined1 *)(unaff_x20 + lVar2) = uVar1;
  return;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1001.c:5422
// ==== 0x100106810 FUN_100106810

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

undefined1 FUN_100106810(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  undefined1 uVar1;
  undefined *puVar2;
  undefined *puVar3;
  void *aBlock;
  undefined8 unaff_x20;
  undefined1 *puVar4;
  undefined *local_c0;
  undefined8 local_b8;
  code *local_b0;
  undefined *puStack_a8;
  code *local_a0;
  undefined *local_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  
  puVar2 = &DAT_10416f990;
  _swift_allocObject(&DAT_10416f990,0x11,7);
  puVar4 = puVar2 + 0x10;
  *puVar4 = 0;
  FUN_100008168(param_2,&local_70,&DAT_1044b3890,&DAT_10361b4a0);
  FUN_100008168(param_3,&local_90,&DAT_1044b3890,&DAT_10361b4a0);
  puVar3 = &DAT_10416f9b8;
  _swift_allocObject(&DAT_10416f9b8,0x68,7);
  *(undefined8 *)(puVar3 + 0x30) = uStack_68;
  *(undefined8 *)(puVar3 + 0x28) = local_70;
  *(undefined8 *)(puVar3 + 0x40) = uStack_58;
  *(undefined8 *)(puVar3 + 0x38) = uStack_60;
  *(undefined8 *)(puVar3 + 0x50) = uStack_88;
  *(undefined8 *)(puVar3 + 0x48) = local_90;
  *(undefined **)(puVar3 + 0x10) = puVar2;
  *(undefined8 *)(puVar3 + 0x18) = unaff_x20;
  *(undefined8 *)(puVar3 + 0x20) = param_1;
  *(undefined8 *)(puVar3 + 0x60) = uStack_78;
  *(undefined8 *)(puVar3 + 0x58) = uStack_80;
  local_a0 = FUN_100107988;
  local_c0 = PTR___NSConcreteStackBlock_10415f2f0;
  local_b8 = 0x42000000;
  local_b0 = FUN_100213ee4;
  puStack_a8 = &DAT_10416f9d0;
  local_98 = puVar3;
  aBlock = __Block_copy(&local_c0);
  puVar3 = local_98;
  _swift_retain(puVar2);
  _swift_release(puVar3);
  _CallWithEHFrame(aBlock);
  __Block_release(aBlock);
  _swift_beginAccess(puVar4,&local_c0,0,0);
  uVar1 = *puVar4;
  _swift_release(puVar2);
  return uVar1;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1001.c:5598
// ==== 0x100106c60 FUN_100106c60

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */

undefined8
FUN_100106c60(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined4 param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  void *aBlock;
  long extraout_x8;
  ulong uVar3;
  undefined8 uVar4;
  undefined8 unaff_x20;
  undefined8 *puVar5;
  long lVar6;
  ulong uVar7;
  ulong uVar8;
  long lVar9;
  undefined8 local_a0;
  undefined4 local_94;
  undefined *local_90;
  undefined8 local_88;
  code *local_80;
  undefined *puStack_78;
  code *local_70;
  undefined *local_68;
  
  puVar1 = &DAT_1044b15e0;
  local_a0 = param_1;
  local_94 = param_4;
  FUN_100005134(&DAT_1044b15e0,&DAT_103618550);
  lVar6 = *(long *)(puVar1 + -8);
  lVar9 = *(long *)(lVar6 + 0x40);
  (*(code *)PTR____chkstk_darwin_10415f308)(lVar9 + 0xfU & 0xfffffffffffffff0);
  puVar1 = &DAT_10416f918;
  _swift_allocObject(&DAT_10416f918,0x18,7);
  puVar5 = (undefined8 *)(puVar1 + 0x10);
  *puVar5 = 0;
  FUN_100008168(param_2,(long)&local_a0 - extraout_x8,&DAT_1044b15e0,&DAT_103618550);
  uVar3 = (ulong)*(byte *)(lVar6 + 0x50);
  uVar7 = uVar3 + 0x28 & (uVar3 ^ 0xffffffffffffffff);
  uVar8 = lVar9 + uVar7 + 7 & 0xfffffffffffffff8;
  puVar2 = &DAT_10416f940;
  _swift_allocObject(&DAT_10416f940,uVar8 + 9,uVar3 | 7);
  *(undefined **)(puVar2 + 0x10) = puVar1;
  *(undefined8 *)(puVar2 + 0x18) = unaff_x20;
  *(undefined8 *)(puVar2 + 0x20) = local_a0;
  FUN_100094938((long)&local_a0 - extraout_x8,puVar2 + uVar7);
  *(undefined8 *)(puVar2 + uVar8) = param_3;
  *(char *)((long)(puVar2 + uVar8) + 8) = (char)local_94;
  local_70 = FUN_1001078b4;
  local_90 = PTR___NSConcreteStackBlock_10415f2f0;
  local_88 = 0x42000000;
  local_80 = FUN_100213ee4;
  puStack_78 = &DAT_10416f958;
  local_68 = puVar2;
  aBlock = __Block_copy(&local_90);
  puVar2 = local_68;
  _swift_retain(puVar1);
  _swift_release(puVar2);
  _CallWithEHFrame(aBlock);
  __Block_release(aBlock);
  _swift_beginAccess(puVar5,&local_90,0,0);
  uVar4 = *puVar5;
  _swift_release(puVar1);
  return uVar4;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1004.c:129535
// ==== 0x1004ae4a0 ADK.ADKApplication.method_instance

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



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1004.c:130065
// ==== 0x1004aefe4 ADK.ADKApplication.method_instance

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void ADK_ADKApplication_method_instance(undefined8 param_1,ID param_2)

{
  undefined8 uVar1;
  ID IVar2;
  long lVar3;
  long lVar4;
  long unaff_x20;
  long lVar5;
  undefined8 local_80 [5];
  undefined1 auStack_58 [24];
  
  lVar3 = _TtC3ADK14ADKApplication::contexts;
  _swift_beginAccess(unaff_x20 + _TtC3ADK14ADKApplication::contexts,auStack_58,0x21,0);
  FUN_1004c1d74(param_1,local_80);
  uVar1 = *(undefined8 *)(unaff_x20 + lVar3);
  _swift_isUniquelyReferenced_nonNull_native(uVar1);
  local_80[0] = *(undefined8 *)(unaff_x20 + lVar3);
  *(undefined8 *)(unaff_x20 + lVar3) = 0x8000000000000000;
  FUN_1004e5680(param_2,param_1,uVar1);
  FUN_1004c39f0(param_1);
  *(undefined8 *)(unaff_x20 + lVar3) = local_80[0];
  _swift_endAccess(auStack_58);
  uVar1 = *(undefined8 *)(unaff_x20 + _TtC3ADK14ADKApplication::contextDelegateBridge);
  objc_stub::setDelegate_(param_2,uVar1);
  IVar2 = objc_stub::preferences(param_2);
  objc_stub::addObserver_(IVar2,uVar1);
  IVar2 = objc_stub::history(param_2);
  objc_stub::setDelegate_
            (IVar2,*(undefined8 *)
                    (unaff_x20 + _TtC3ADK14ADKApplication::contextHistoryDelegateBridge));
  lVar3 = *(long *)(unaff_x20 + _TtC3ADK14ADKApplication::downloadDelegateBridge);
  IVar2 = objc_stub::downloads(param_2);
  objc_stub::setDelegate_(IVar2,lVar3);
  lVar3 = lVar3 + _TtC3ADK36BrowserContextDownloadDelegateBridge::delegate;
  _swift_beginAccess(lVar3,local_80,0,0);
  lVar4 = lVar3;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar4 != 0) {
    lVar5 = *(long *)(lVar3 + 8);
    lVar3 = lVar4;
    _swift_getObjectType();
    (**(code **)(lVar5 + 0x38))(param_1,lVar3,lVar5);
    _swift_unknownObjectRelease(lVar4);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1004.c:182611
// ==== 0x1004e9748 _TtC3ADK36BrowserContextDownloadDelegateBridge::-[BrowserContextDownloadDelegateBridge_browserContext:onDeterminingFilename:defaultFilename:completionTimeout:completion:]

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

void _TtC3ADK36BrowserContextDownloadDelegateBridge::
     __BrowserContextDownloadDelegateBridge_browserContext_onDeterminingFilename_defaultFilename_completionTimeout_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,double param_6,ID param_7,
               undefined4 param_8)

{
  void *aBlock;
  String SVar1;
  
  aBlock = __Block_copy((void *)param_7);
  SVar1 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  __Block_copy(aBlock);
  FUN_1004e9ac4(param_3,param_4,SVar1.str,SVar1.bridgeObject,param_1,aBlock);
  __Block_release(aBlock);
  __Block_release(aBlock);
  _swift_bridgeObjectRelease(SVar1.bridgeObject);
  return;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1004.c:182794
// ==== 0x1004e9ac4 FUN_1004e9ac4

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



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1004.c:182909
// ==== 0x1004e9da8 FUN_1004e9da8

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_1004e9da8(void)

{
  NSString *pNVar1;
  long lVar2;
  long unaff_x20;
  
  lVar2 = *(long *)(unaff_x20 + 0x10);
  pNVar1 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
  (**(code **)(lVar2 + 0x10))(lVar2,pNVar1);
  return 0;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1004.c:184992
// ==== 0x1004ec75c FUN_1004ec75c

void FUN_1004ec75c(undefined8 param_1,MainActor *param_2,undefined *param_3,char *param_4,
                  void *param_5)

{
  code *pcVar1;
  MainActor *pMVar2;
  undefined *puVar3;
  undefined *puVar4;
  _StringGuts _Var5;
  char *pcVar6;
  MainActor *unaff_x20;
  long unaff_x21;
  String SVar7;
  String SVar8;
  String SVar9;
  String SVar10;
  String SVar11;
  String SVar12;
  
  _Var5.unknown = param_3;
  pcVar6 = param_4;
  pMVar2 = Swift::MainActor::get_shared(unaff_x20,param_2);
  _swift_release();
  _swift_task_getMainExecutor();
  _swift_task_isCurrentExecutor();
  if (((ulong)pMVar2 & 1) == 0) {
    Swift::_StringGuts::grow(0x42,_Var5);
    SVar7.bridgeObject = (void *)0x800000010391c080;
    SVar7.str = (char *)0xd00000000000003f;
    SVar10.bridgeObject = param_5;
    SVar10.str = pcVar6;
    Swift::String::append(SVar7,SVar10);
    SVar7 = Swift::__typeName(SUB81(unaff_x20,0));
    SVar11.bridgeObject = param_5;
    SVar11.str = pcVar6;
    Swift::String::append(SVar7,SVar11);
    _swift_bridgeObjectRelease(SVar7.bridgeObject);
    SVar8.bridgeObject = &DAT_e100000000000000;
    SVar8.str = (char *)0x2e;
    SVar12.bridgeObject = param_5;
    SVar12.str = pcVar6;
    Swift::String::append(SVar8,SVar12);
    SVar9.bridgeObject = &DAT_00000002;
    SVar9.str = &SUB_0000000b;
    Swift::_assertionFailure
              ((StaticString)0x103921730,SVar9,(StaticString)0x0,0xe000000000000000,
               (__uint32)param_4);
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x1004ec8e4);
    (*pcVar1)();
  }
  puVar3 = &DAT_1041880e8;
  _swift_allocObject(&DAT_1041880e8,0x20,7);
  *(MainActor **)(puVar3 + 0x10) = param_2;
  *(undefined **)(puVar3 + 0x18) = param_3;
  (*(code *)param_2)(param_1);
  puVar4 = puVar3;
  _swift_isEscapingClosureAtFileLocation(puVar3,"",0,0,0,0);
  _swift_release(puVar3);
  if (unaff_x21 == 0) {
    if (((ulong)puVar4 & 1) != 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x1004ec848);
      (*pcVar1)();
    }
  }
  else if (((ulong)puVar4 & 1) != 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x1004ec81c);
    (*pcVar1)();
  }
  return;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1004.c:190064
// ==== 0x1004f2640 FUN_1004f2640

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_1004f2640(long param_1,long param_2,undefined8 param_3)

{
  code *pcVar1;
  undefined8 uVar2;
  String SVar3;
  
  pcVar1 = *(code **)(param_1 + 0x20);
  uVar2 = *(undefined8 *)(param_1 + 0x28);
  if (param_2 == 0) {
    SVar3 = (String)ZEXT816(0);
  }
  else {
    SVar3 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  }
  _swift_retain(uVar2);
  (*pcVar1)(SVar3.str,SVar3.bridgeObject,param_3);
  _swift_release(uVar2);
  _swift_bridgeObjectRelease(SVar3.bridgeObject);
  return;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1004.c:190213
// ==== 0x1004f285c FUN_1004f285c

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined * FUN_1004f285c(undefined8 param_1,undefined8 param_2)

{
  undefined *puVar1;
  void *aBlock;
  ID IVar2;
  undefined *puVar3;
  ID unaff_x20;
  undefined *unaff_x24;
  undefined *local_88;
  undefined8 local_80;
  code *local_78;
  undefined *puStack_70;
  code *local_68;
  undefined *local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_10415f368;
  puVar1 = &DAT_104188698;
  _swift_allocObject(&DAT_104188698,0x20,7);
  *(undefined8 *)(puVar1 + 0x10) = param_1;
  *(undefined8 *)(puVar1 + 0x18) = param_2;
  local_68 = FUN_1004f2ad0;
  local_88 = PTR___NSConcreteStackBlock_10415f2f0;
  local_80 = 0x42000000;
  local_78 = FUN_1004f2640;
  puStack_70 = &DAT_1041886b0;
  local_60 = puVar1;
  aBlock = __Block_copy(&local_88);
  puVar1 = local_60;
  _swift_retain(param_2);
  _swift_release(puVar1);
  local_88 = (undefined *)0x0;
  IVar2 = objc_stub::openProtocolConnection_error_(unaff_x20,aBlock,&local_88);
  __Block_release(aBlock);
  if (IVar2 == 0) {
    Foundation::__convertNSErrorToError();
    _swift_willThrow(0);
  }
  else {
    puVar1 = &DAT_1041886e8;
    _swift_allocObject(&DAT_1041886e8,0x20,7);
    *(ID *)(puVar1 + 0x10) = unaff_x20;
    *(ID *)(puVar1 + 0x18) = IVar2;
    puVar3 = &DAT_104188710;
    _swift_allocObject(&DAT_104188710,0x20,7);
    *(ID *)(puVar3 + 0x10) = unaff_x20;
    *(ID *)(puVar3 + 0x18) = IVar2;
    unaff_x24 = puVar3;
    FUN_1004f2ca0();
    _swift_allocObject();
    unaff_x24[0x30] = 0;
    *(code **)(unaff_x24 + 0x10) = FUN_1004f2c80;
    *(undefined **)(unaff_x24 + 0x18) = puVar1;
    *(code **)(unaff_x24 + 0x20) = FUN_1004f2c98;
    *(undefined **)(unaff_x24 + 0x28) = puVar3;
  }
  if (*(long *)PTR____stack_chk_guard_10415f368 == local_58) {
    return unaff_x24;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1004.c:190285
// ==== 0x1004f2a24 FUN_1004f2a24

void FUN_1004f2a24(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 param_5)

{
  MainActor *pMVar1;
  undefined1 auStack_80 [16];
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 local_50;
  
  pMVar1 = Swift::MainActor::typeMetadataAccessor();
  local_70 = param_3;
  uStack_68 = param_4;
  local_60 = param_5;
  uStack_58 = param_1;
  local_50 = param_2;
  FUN_1004ec75c(FUN_1004f2d38,auStack_80,"ADK/WebContentDevTools+Connection.swift",0x27,2,0x2e,
                pMVar1);
  return;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1005.c:44808
// ==== 0x100535984 _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:decidePolicyForNavigationAction:]

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

long_long _TtC3ADK18WebContentChromium::
          __WebContentChromium_webContents_decidePolicyForNavigationAction__
                    (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  FUN_10053605c();
  return param_4;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1005.c:45079
// ==== 0x100536028 FUN_100536028

undefined8 FUN_100536028(undefined8 param_1,long param_2)

{
  if (param_2 - 2U < 9) {
    return *(undefined8 *)(&DAT_103640410 + (param_2 - 2U) * 8);
  }
  FUN_1003568ec();
  return 10;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1005.c:45092
// ==== 0x10053605c FUN_10053605c

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ID FUN_10053605c(ID param_1)

{
  DefaultStringInterpolation stringInterpolation;
  undefined *puVar1;
  long lVar2;
  URL UVar3;
  URL UVar4;
  long lVar5;
  ID IVar6;
  undefined8 extraout_x1;
  undefined8 uVar7;
  long *plVar8;
  undefined *puVar9;
  undefined **ppuVar10;
  undefined8 uVar11;
  char *pcVar12;
  long lVar13;
  void *pvVar14;
  ID IVar15;
  long extraout_x8;
  long extraout_x8_00;
  long extraout_x8_01;
  long extraout_x12;
  long extraout_x12_00;
  ID unaff_x20;
  long lVar16;
  long lVar17;
  code *pcVar18;
  long lVar19;
  long lVar20;
  URL url;
  String SVar21;
  String SVar22;
  long lStack_240;
  undefined4 auStack_238 [2];
  void *local_230;
  undefined8 local_228;
  undefined8 local_220;
  ID local_218;
  undefined1 auStack_1e0 [96];
  undefined1 auStack_180 [24];
  undefined8 local_168;
  long lStack_160;
  undefined1 local_158 [8];
  undefined *local_150;
  undefined *puStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  undefined8 local_130;
  undefined8 uStack_128;
  String local_120;
  String local_110;
  undefined8 local_100;
  undefined1 auStack_f8 [24];
  undefined1 auStack_e0 [24];
  DefaultStringInterpolation local_c8;
  undefined *local_c0;
  undefined *puStack_b8;
  undefined8 local_b0;
  long lStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  String local_90;
  String local_80;
  undefined8 local_70;
  
  puVar1 = &DAT_1044c87d0;
  FUN_100005134(&DAT_1044c87d0,&DAT_10363af50);
  (*(code *)PTR____chkstk_darwin_10415f308)
            (*(long *)(*(long *)(puVar1 + -8) + 0x40) + 0xfU & 0xfffffffffffffff0);
  lVar13 = (long)&local_230 - extraout_x8;
  lVar2 = 0;
  FUN_100551100();
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(*(long *)(lVar2 + -8) + 0x40));
  lVar20 = lVar13 - (extraout_x8_00 + 0xfU & 0xfffffffffffffff0);
  UVar3 = Foundation::URL::typeMetadataAccessor();
  lVar19 = *(long *)(UVar3.unknown + -8);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(lVar19 + 0x40));
  url.unknown = (undefined *)(lVar20 - (extraout_x8_01 + 0xfU & 0xfffffffffffffff0));
  (*(code *)PTR____chkstk_darwin_10415f308)();
  (*(code *)PTR____chkstk_darwin_10415f308)();
  lVar16 = ((long)url.unknown - extraout_x12) - extraout_x12_00;
  UVar4.unknown = (undefined *)objc_stub::url(param_1);
  if (UVar4.unknown == (undefined *)0x0) {
    unaff_x20 = 0;
  }
  else {
    Foundation::URL::__unconditionallyBridgeFromObjectiveC(UVar4);
    (**(code **)(lVar19 + 0x20))(lVar16,(long)url.unknown - extraout_x12,UVar3.unknown);
    lVar17 = unaff_x20 + _TtC3ADK18WebContentChromium::navigationDelegate;
    uVar11 = 0;
    pvVar14 = (void *)0x0;
    _swift_beginAccess(lVar17,auStack_e0);
    lVar5 = lVar17;
    _swift_unknownObjectWeakLoadStrong();
    if (lVar5 == 0) {
      pcVar12 = &DAT_00000002;
      SVar21 = Swift::StaticString::get_description((StaticString)0x103932230);
      plVar8 = SVar21.bridgeObject;
      local_c8.unknown = &SUB_d000000000000013;
      local_c0 = (undefined *)0x8000000103918510;
      SVar22.bridgeObject = pvVar14;
      SVar22.str = pcVar12;
      Swift::String::append(SVar21,SVar22);
      _swift_bridgeObjectRelease();
      puVar1 = local_c0;
      stringInterpolation.unknown = local_c8.unknown;
      FUN_100734924();
      lVar20 = *plVar8;
      SVar21 = Swift::StaticString::get_description((StaticString)0x103932200);
      puVar9 = SVar21.bridgeObject;
      SVar22 = Swift::StaticString::get_description((StaticString)0x103932290);
      pvVar14 = SVar22.bridgeObject;
      _swift_beginAccess(lVar20 + 0x20,auStack_f8,0,0);
      FUN_100012c10(lVar20 + 0x20,&local_c8);
      lVar2 = lStack_a8;
      uVar11 = local_b0;
      FUN_100012c54(&local_c8,local_b0);
      (**(code **)(lVar2 + 0x70))(uVar11,lVar2);
      FUN_100012e70(&local_c8);
      if (((uint)uVar11 & 0xff) < 6) {
        FUN_100012c10(lVar20 + 0x20,auStack_180);
        FUN_100012c54(auStack_180,local_168);
        FUN_1004c0f68();
        _swift_bridgeObjectRetain(puVar1);
        (extension_Swift)::Swift::ExpressibleByStringInterpolation::_init(stringInterpolation);
        puStack_b8 = local_c0;
        local_c0 = local_c8.unknown;
        local_c8.unknown._0_1_ = 5;
        lStack_a8 = 0;
        local_b0 = 0;
        uStack_98 = 0;
        local_a0 = 0;
        local_70 = 0x11;
        local_158[0] = 5;
        local_150 = local_c0;
        puStack_148 = puStack_b8;
        uStack_138 = 0;
        local_140 = 0;
        uStack_128 = 0;
        local_130 = 0;
        local_100 = 0x11;
        local_120 = SVar21;
        local_110 = SVar22;
        local_90 = SVar21;
        local_80 = SVar22;
        _swift_bridgeObjectRetain(puVar9);
        _swift_bridgeObjectRetain(pvVar14);
        FUN_1004c0fa8(&local_c8,auStack_1e0);
        FUN_100012cf4(local_158);
        (**(code **)(lStack_160 + 0x28))(&local_c8,local_168,lStack_160);
        _swift_bridgeObjectRelease(pvVar14);
        _swift_bridgeObjectRelease(puVar9);
        FUN_100012cf4(&local_c8);
        FUN_100012e70(auStack_180);
      }
      else {
        _swift_bridgeObjectRelease(puVar1);
        _swift_bridgeObjectRelease(pvVar14);
        puVar1 = puVar9;
      }
      _swift_bridgeObjectRelease(puVar1);
      unaff_x20 = objc_stub::FUN_1035c6500(param_1);
      (**(code **)(lVar19 + 8))(lVar16,UVar3.unknown);
    }
    else {
      lVar17 = *(long *)(lVar17 + 8);
      IVar6 = objc_stub::FUN_1035c6500(param_1);
      uVar7 = extraout_x1;
      FUN_100535eb8();
      local_230 = pvVar14;
      local_228 = uVar11;
      local_220 = uVar7;
      local_218 = IVar6;
      UVar4.unknown = (undefined *)objc_stub::FUN_1035ca260(param_1);
      if (UVar4.unknown != (undefined *)0x0) {
        Foundation::URL::__unconditionallyBridgeFromObjectiveC(UVar4);
      }
      pcVar18 = *(code **)(lVar19 + 0x38);
      (*pcVar18)(lVar13,UVar4.unknown == (undefined *)0x0,1,UVar3.unknown);
      IVar6 = objc_stub::navigationType(param_1);
      if (4 < IVar6) {
        *(undefined4 *)(lVar16 + -8) = 0;
        *(undefined8 *)(lVar16 + -0x10) = 0xce;
        SVar21.bridgeObject = &DAT_00000002;
        SVar21.str = &SUB_0000000b;
        Swift::_assertionFailure
                  ((StaticString)0x103921730,SVar21,(StaticString)0x0,0xe000000000000000,0x3932200);
                    /* WARNING: Does not return */
        pcVar18 = (code *)SoftwareBreakpoint(1,0x10053661c);
        (*pcVar18)();
      }
      (**(code **)(lVar19 + 0x10))(url.unknown,lVar16,UVar3.unknown);
      Foundation::URLRequest::init(url,(NSURLRequestCachePolicy)0x0,60.0);
      (*pcVar18)(lVar20,1,1,UVar3.unknown);
      FUN_1004f8578(lVar13,lVar20);
      *(char *)(lVar20 + *(int *)(lVar2 + 0x14)) = (char)(0x503020400 >> ((IVar6 & 7) << 3));
      lVar2 = lVar5;
      _swift_getObjectType(lVar5);
      pcVar18 = *(code **)(lVar17 + 8);
      *(long *)(lVar16 + -0x10) = lVar17;
      IVar6 = local_218;
      uVar11 = local_228;
      pvVar14 = local_230;
      ppuVar10 = &PTR_PTR_10418d648;
      lVar13 = lVar20;
      IVar15 = local_218;
      (*pcVar18)(unaff_x20,&PTR_PTR_10418d648,lVar20,local_218,local_220,local_228,local_230,lVar2);
      FUN_1003568ec(IVar6,local_220,uVar11,pvVar14);
      FUN_100536028(unaff_x20,ppuVar10,lVar13,IVar15);
      _swift_unknownObjectRelease(lVar5);
      (**(code **)(lVar19 + 8))(lVar16,UVar3.unknown);
      FUN_100536bfc(lVar20);
    }
  }
  return unaff_x20;
}



// SOURCE /home/exedev/work/inputs/app168/ghidra/decomp/0x1005.c:48799
// ==== 0x10053b870 ADK.WebContentChromium.navigationDelegate.setter

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void ADK_WebContentChromium_navigationDelegate_setter(undefined8 param_1,undefined8 param_2)

{
  long lVar1;
  long unaff_x20;
  undefined1 auStack_48 [24];
  
  lVar1 = unaff_x20 + _TtC3ADK18WebContentChromium::navigationDelegate;
  _swift_beginAccess(lVar1,auStack_48,1,0);
  *(undefined8 *)(lVar1 + 8) = param_2;
  _swift_unknownObjectWeakAssign(lVar1,param_1);
  _swift_unknownObjectWeakLoadStrong();
  if (lVar1 != 0) {
    _swift_unknownObjectRelease();
    objc_stub::setNavigationDelegate_(*(ID *)(unaff_x20 + _TtC3ADK18WebContentChromium::webView));
  }
  _swift_unknownObjectRelease(param_1);
  return;
}



