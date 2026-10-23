// ==== 0x100527668  ADK.ChromiumBrowser.method_instance  (868 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined *
ADK_ChromiumBrowser_method_instance
          (char *param_1,void *param_2,undefined8 param_3,long param_4,long param_5,uint param_6,
          uint param_7)

{
  bool bVar1;
  __int64 *p_Var2;
  __int64 _Var3;
  __int64 _Var4;
  __int64 _Var5;
  __int64 _Var6;
  code *pcVar7;
  CurrentValueSubject *pCVar8;
  CurrentValueSubject *this;
  __int64 _Var9;
  ID IVar10;
  NSString *pNVar11;
  NSString *pNVar12;
  undefined *puVar13;
  undefined *puVar14;
  void *pvVar15;
  AnyPublisher<(extension_in_Combine):Combine_Publisher_eraseToAnyPublisher()__>_Combine_AnyPublisher<A_Output,A_Failure>__extension_in_Combine__Combine_Publisher_eraseToAnyPublisher____>_Combine_AnyPublisher<A_Output,A_Failure>>
  AVar16;
  ulong uVar17;
  long lVar18;
  long unaff_x20;
  String SVar19;
  CurrentValueSubject *local_a0;
  undefined8 uStack_98;
  code *local_90;
  undefined *puStack_88;
  code *local_80;
  undefined *local_78;
  long local_68;
  
  local_68 = *(long *)PTR____stack_chk_guard_10415f368;
  uStack_98 = 0;
  local_a0 = (CurrentValueSubject *)0x0;
  puStack_88 = (undefined *)0x0;
  local_90 = (code *)0x0;
  pCVar8 = (CurrentValueSubject *)&DAT_1044cbe60;
  lVar18 = param_4;
  FUN_100005134(&DAT_1044cbe60,&DAT_10363fef0);
  uVar17 = (ulong)*(ushort *)(pCVar8 + 0x34);
  _swift_allocObject();
  this = (CurrentValueSubject *)&local_a0;
  Combine::CurrentValueSubject::_init(pCVar8);
  SVar19.bridgeObject = param_2;
  SVar19.str = param_1;
  pvVar15 = param_2;
  _Var9 = Swift::String::get_count(SVar19);
  bVar1 = 99 < _Var9;
  if (bVar1 != 0 || (param_7 & 1) != 0) {
    ADK_ChromiumBrowser_getter_instance();
    _swift_bridgeObjectRelease(uVar17);
    _swift_bridgeObjectRelease(lVar18);
    p_Var2 = (__int64 *)
             (unaff_x20 + _TtC3ADK15ChromiumBrowser::___lazy_storage___deletableAutocompleteMatches)
    ;
    _Var3 = *p_Var2;
    _Var5 = p_Var2[1];
    _Var4 = p_Var2[2];
    _Var6 = p_Var2[3];
    *p_Var2 = _Var9;
    p_Var2[1] = (__int64)pvVar15;
    puVar13 = PTR___swiftEmptyDictionarySingleton_104167658;
    p_Var2[2] = (__int64)PTR___swiftEmptyDictionarySingleton_104167658;
    p_Var2[3] = (__int64)puVar13;
    FUN_10052ddb8(_Var3,_Var5,_Var4,_Var6);
  }
  IVar10 = *(ID *)(unaff_x20 + _TtC3ADK15ChromiumBrowser::browser);
  pNVar11 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
  if (param_5 < -0x80000000) {
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(1,0x1005279c4);
    (*pcVar7)();
  }
  if (0x7fffffff < param_5) {
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(1,0x1005279c8);
    (*pcVar7)();
  }
  lVar18 = -0x2000000000000000;
  if (param_4 != 0) {
    lVar18 = param_4;
  }
  _swift_bridgeObjectRetain(param_4);
  pNVar12 = (extension_Foundation)::Swift::String::_bridgeToObjectiveC();
  _swift_bridgeObjectRelease(lVar18);
  puVar13 = &DAT_10418c5d0;
  _swift_allocObject(&DAT_10418c5d0,0x18,7);
  _swift_unknownObjectWeakInit(puVar13 + 0x10);
  puVar14 = &DAT_10418c698;
  _swift_allocObject(&DAT_10418c698,0x30,7);
  *(char **)(puVar14 + 0x10) = param_1;
  *(void **)(puVar14 + 0x18) = param_2;
  *(undefined **)(puVar14 + 0x20) = puVar13;
  *(CurrentValueSubject **)(puVar14 + 0x28) = this;
  local_80 = FUN_10052ddac;
  local_a0 = (CurrentValueSubject *)PTR___NSConcreteStackBlock_10415f2f0;
  uStack_98 = 0x42000000;
  local_90 = FUN_100527d30;
  puStack_88 = &DAT_10418c6b0;
  local_78 = puVar14;
  pvVar15 = __Block_copy(&local_a0);
  puVar13 = local_78;
  _swift_bridgeObjectRetain(param_2);
  _swift_retain(this);
  _swift_release(puVar13);
  local_a0 = (CurrentValueSubject *)0x0;
  IVar10 = objc_stub::
           autocomplete_cursorPosition_preventInlineAutocomplete_startFresh_keyword_error_completion_
                     (IVar10,pNVar11,param_5,(ulong)(param_6 & 1),(ulong)((uint)bVar1 | param_7 & 1)
                      ,pNVar12,&local_a0,pvVar15);
  __Block_release(pvVar15);
  if ((int)IVar10 == 0) {
    pCVar8 = local_a0;
    Foundation::__convertNSErrorToError();
    _swift_willThrow(0);
    local_a0 = (CurrentValueSubject *)CONCAT71(local_a0._1_7_,1);
    Combine::CurrentValueSubject::_send(this);
    _swift_errorRelease(pCVar8);
  }
  local_a0 = this;
  FUN_10000a9c8(&DAT_1044cbe68,&DAT_1044cbe60,&DAT_10363fef0,
                PTR__Combine__protocol_conformance_descriptor_for_Combine_CurrentValueSubject<A,B>___Combine_Publisher_in_Combine_>Combine__Publisher_104160840
               );
  AVar16 = (extension_Combine)::Combine::Publisher::_eraseToAnyPublisher();
  _swift_release(this);
  if (*(long *)PTR____stack_chk_guard_10415f368 == local_68) {
    return AVar16.unknown;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


