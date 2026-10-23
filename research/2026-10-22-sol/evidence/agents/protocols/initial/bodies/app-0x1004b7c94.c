// ==== 0x1004b7c94  _TtC3ADK14ADKApplication::-[ADKApplication_didRequestDevToolsDebugging:]  (188 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void _TtC3ADK14ADKApplication::__ADKApplication_didRequestDevToolsDebugging__
               (ID param_1,SEL param_2,ID param_3)

{
  undefined8 *puVar1;
  code *pcVar2;
  undefined8 uVar3;
  undefined1 auStack_58 [24];
  
  puVar1 = (undefined8 *)(param_1 + onDevToolsDebuggingRequested);
  _swift_beginAccess(puVar1,auStack_58,0,0);
  pcVar2 = (code *)*puVar1;
  if (pcVar2 == (code *)0x0) {
    objc_stub::deny(param_3);
  }
  else {
    uVar3 = puVar1[1];
    FUN_1000072f8(pcVar2,uVar3);
    (*pcVar2)(param_3);
    FUN_10000a510(pcVar2,uVar3);
  }
  return;
}


