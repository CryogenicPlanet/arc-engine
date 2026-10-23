// ==== 0x10052bbcc  _TtC3ADK15ChromiumBrowser::-[ChromiumBrowser_browser:getAllTabGroupsWithCompletion:]  (444 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtC3ADK15ChromiumBrowser::__ChromiumBrowser_browser_getAllTabGroupsWithCompletion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,undefined4 param_5)

{
  void *aBlock;
  undefined *puVar1;
  long lVar2;
  long lVar3;
  undefined *puVar4;
  NSArray *pNVar5;
  long lVar6;
  code *pcVar7;
  undefined1 auStack_78 [24];
  
  aBlock = __Block_copy((void *)param_4);
  puVar1 = &DAT_10418cae0;
  _swift_allocObject(&DAT_10418cae0,0x18,7);
  *(void **)(puVar1 + 0x10) = aBlock;
  lVar3 = param_1 + browserDelegate;
  _swift_beginAccess(lVar3,auStack_78,0,0);
  lVar2 = lVar3;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar2 == 0) {
    __Block_copy(aBlock);
    FUN_1000391c0(0,&DAT_1044c85b0,&PTR__OBJC_CLASS___ArcTabGroupInfo_10446ffc0);
    pNVar5 = (extension_Foundation)::Swift::Array<undefined>::_bridgeToObjectiveC();
    (**(code **)((long)aBlock + 0x10))(aBlock,pNVar5);
    _swift_release(puVar1);
  }
  else {
    lVar6 = *(long *)(lVar3 + 8);
    lVar3 = lVar2;
    _swift_getObjectType();
    puVar4 = &DAT_10418cb08;
    _swift_allocObject(&DAT_10418cb08,0x28,7);
    *(code **)(puVar4 + 0x10) = FUN_10052eb34;
    *(undefined **)(puVar4 + 0x18) = puVar1;
    *(ID *)(puVar4 + 0x20) = param_1;
    pcVar7 = *(code **)(lVar6 + 0x70);
    __Block_copy(aBlock);
    _swift_retain(puVar1);
    (*pcVar7)(param_1,&PTR_PTR_10418c868,thunk_FUN_10052e350,puVar4,lVar3,lVar6);
    _swift_release(puVar1);
    _swift_unknownObjectRelease(lVar2);
    _swift_release(puVar4);
  }
  __Block_release(aBlock);
  return;
}


