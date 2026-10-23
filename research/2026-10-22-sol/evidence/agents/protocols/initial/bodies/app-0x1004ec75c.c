// ==== 0x1004ec75c  FUN_1004ec75c  (392 bytes)

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


