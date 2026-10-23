// ==== 0x10132dc34  _TtCC15URLLoaderClient19URLLoaderClientImpl12DataDelegate::-[URLLoaderClientImpl.DataDelegate_didReceiveHeaders:withFinalURL:withCode:withHeaders:]  (440 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x2c bytes */

void _TtCC15URLLoaderClient19URLLoaderClientImpl12DataDelegate::
     __URLLoaderClientImpl_DataDelegate_didReceiveHeaders_withFinalURL_withCode_withHeaders__
               (ID param_1,SEL param_2,ID param_3,ID param_4,int param_5,ID param_6)

{
  undefined *puVar1;
  URL UVar2;
  Dictionary<undefined,undefined> DVar3;
  long extraout_x8;
  long extraout_x8_00;
  long lVar4;
  long lVar5;
  long lVar6;
  String SVar7;
  int aiStack_80 [2];
  undefined1 auStack_78 [24];
  
  puVar1 = &DAT_1045499f0;
  aiStack_80[1] = param_5;
  FUN_100005134(&DAT_1045499f0,&DAT_1036fc428);
  (*(code *)PTR____chkstk_darwin_10415f308)
            (*(long *)(*(long *)(puVar1 + -8) + 0x40) + 0xfU & 0xfffffffffffffff0);
  lVar5 = (long)aiStack_80 - extraout_x8;
  UVar2 = Foundation::URL::typeMetadataAccessor();
  lVar4 = *(long *)(UVar2.unknown + -8);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(lVar4 + 0x40));
  lVar6 = lVar5 - (extraout_x8_00 + 0xfU & 0xfffffffffffffff0);
  SVar7 = (extension_Foundation)::Swift::String::__unconditionallyBridgeFromObjectiveC();
  DVar3 = (extension_Foundation)::Swift::Dictionary::__unconditionallyBridgeFromObjectiveC();
  FUN_101328928(lVar6,SVar7.str,SVar7.bridgeObject,param_1 + request);
  _swift_bridgeObjectRelease(SVar7.bridgeObject);
  (**(code **)(lVar4 + 0x20))(lVar5,lVar6,UVar2.unknown);
  lVar4 = 0;
  FUN_1013239e0();
  *(int *)(lVar5 + *(int *)(lVar4 + 0x14)) = aiStack_80[1];
  *(undefined **)(lVar5 + *(int *)(lVar4 + 0x18)) = DVar3.unknown;
  (**(code **)(*(long *)(lVar4 + -8) + 0x38))(lVar5,0,1,lVar4);
  lVar4 = response;
  _swift_beginAccess(param_1 + response,auStack_78,0x21,0);
  FUN_100086c74(lVar5,param_1 + lVar4,&DAT_1045499f0,&DAT_1036fc428);
  _swift_endAccess(auStack_78);
  return;
}


