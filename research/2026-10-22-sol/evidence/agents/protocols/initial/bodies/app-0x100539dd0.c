// ==== 0x100539dd0  _TtC3ADK18WebContentChromium::-[WebContentChromium_webContents:promptForArtifactLinkPermissionToURL:willNavigate:forSessionID:folderName:completion:]  (544 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x3c bytes */

void _TtC3ADK18WebContentChromium::
     __WebContentChromium_webContents_promptForArtifactLinkPermissionToURL_willNavigate_forSessionID_folderName_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,bool param_5,ID param_6,ID param_7,
               ID param_8,undefined4 param_9)

{
  long lVar1;
  void *pvVar2;
  URL UVar3;
  void *aBlock;
  undefined *puVar4;
  long lVar5;
  long lVar6;
  void *pvVar7;
  long extraout_x8;
  long lVar8;
  long lVar9;
  code *pcVar10;
  String SVar11;
  long alStack_c0 [4];
  void *local_a0;
  long local_98;
  char *local_90;
  char *local_88;
  bool local_7c;
  undefined1 auStack_78 [24];
  
  local_7c = param_5;
  UVar3 = Foundation::URL::typeMetadataAccessor();
  lVar9 = *(long *)(UVar3.unknown + -8);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(lVar9 + 0x40));
  lVar1 = -(extraout_x8 + 0xfU & 0xfffffffffffffff0);
  aBlock = __Block_copy((void *)param_8);
  Foundation::URL::__unconditionallyBridgeFromObjectiveC((URL)param_4);
  SVar11 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  pvVar7 = SVar11.bridgeObject;
  local_88 = SVar11.str;
  SVar11 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  local_90 = SVar11.str;
  puVar4 = &DAT_10418d4a8;
  _swift_allocObject(&DAT_10418d4a8,0x18,7);
  *(void **)(puVar4 + 0x10) = aBlock;
  lVar6 = param_1 + uiDelegate;
  _swift_beginAccess(lVar6,auStack_78,0,0);
  lVar5 = lVar6;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar5 == 0) {
    __Block_copy(aBlock);
    (**(code **)((long)aBlock + 0x10))(aBlock,0);
    _swift_bridgeObjectRelease(pvVar7);
    _swift_bridgeObjectRelease(SVar11.bridgeObject);
    __Block_release(aBlock);
    _swift_release(puVar4);
  }
  else {
    lVar8 = *(long *)(lVar6 + 8);
    lVar6 = lVar5;
    local_a0 = SVar11.bridgeObject;
    _swift_getObjectType();
    pcVar10 = *(code **)(lVar8 + 0x1f8);
    local_98 = lVar9;
    __Block_copy(aBlock);
    *(long *)((long)alStack_c0 + lVar1 + 0x10) = lVar6;
    *(long *)((long)alStack_c0 + lVar1 + 0x18) = lVar8;
    *(code **)((long)alStack_c0 + lVar1) = FUN_1004e80e0;
    *(undefined **)((long)alStack_c0 + lVar1 + 8) = puVar4;
    pvVar2 = local_a0;
    (*pcVar10)(param_1,&PTR_PTR_10418d648,(long)&local_a0 + lVar1,local_7c,local_88,pvVar7,local_90,
               local_a0);
    lVar9 = local_98;
    _swift_release(puVar4);
    _swift_unknownObjectRelease(lVar5);
    _swift_bridgeObjectRelease(pvVar7);
    _swift_bridgeObjectRelease(pvVar2);
    __Block_release(aBlock);
  }
  (**(code **)(lVar9 + 8))((long)&local_a0 + lVar1,UVar3.unknown);
  return;
}


