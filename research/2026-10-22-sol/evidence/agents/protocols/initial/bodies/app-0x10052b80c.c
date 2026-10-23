// ==== 0x10052b80c  _TtC3ADK15ChromiumBrowser::-[ChromiumBrowser_browser:getTabGroupWithId:completion:]  (432 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void _TtC3ADK15ChromiumBrowser::__ChromiumBrowser_browser_getTabGroupWithId_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,undefined4 param_6)

{
  void *aBlock;
  undefined *puVar1;
  long lVar2;
  long lVar3;
  undefined *puVar4;
  void *pvVar5;
  long lVar6;
  code *pcVar7;
  String SVar8;
  undefined1 auStack_78 [24];
  
  aBlock = __Block_copy((void *)param_5);
  SVar8 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  pvVar5 = SVar8.bridgeObject;
  puVar1 = &DAT_10418cb30;
  _swift_allocObject(&DAT_10418cb30,0x18,7);
  *(void **)(puVar1 + 0x10) = aBlock;
  lVar3 = param_1 + browserDelegate;
  _swift_beginAccess(lVar3,auStack_78,0,0);
  lVar2 = lVar3;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar2 == 0) {
    __Block_copy(aBlock);
    (**(code **)((long)aBlock + 0x10))(aBlock,0);
    _swift_release(puVar1);
    _swift_bridgeObjectRelease(pvVar5);
    __Block_release(aBlock);
  }
  else {
    lVar6 = *(long *)(lVar3 + 8);
    lVar3 = lVar2;
    _swift_getObjectType();
    puVar4 = &DAT_10418cb58;
    _swift_allocObject(&DAT_10418cb58,0x28,7);
    *(code **)(puVar4 + 0x10) = FUN_10052eb3c;
    *(undefined **)(puVar4 + 0x18) = puVar1;
    *(ID *)(puVar4 + 0x20) = param_1;
    pcVar7 = *(code **)(lVar6 + 0x68);
    __Block_copy(aBlock);
    _swift_retain(puVar1);
    (*pcVar7)(param_1,&PTR_PTR_10418c868,SVar8.str,pvVar5,thunk_FUN_10052e344,puVar4,lVar3,lVar6);
    _swift_release(puVar1);
    _swift_bridgeObjectRelease(pvVar5);
    __Block_release(aBlock);
    _swift_unknownObjectRelease(lVar2);
    _swift_release(puVar4);
  }
  return;
}


