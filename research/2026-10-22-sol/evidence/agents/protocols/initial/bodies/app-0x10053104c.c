// ==== 0x10053104c  _TtC3ADK25TracingControllerChromium::-[TracingControllerChromium_tracingControllerDidAbortTracing:]  (220 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void _TtC3ADK25TracingControllerChromium::
     __TracingControllerChromium_tracingControllerDidAbortTracing__
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  ID local_80 [3];
  undefined8 local_68;
  undefined **ppuStack_60;
  undefined1 auStack_58 [24];
  
  lVar2 = param_1 + delegate;
  _swift_beginAccess(lVar2,auStack_58,0,0);
  lVar1 = lVar2;
  _swift_unknownObjectWeakLoadStrong();
  if (lVar1 != 0) {
    lVar4 = *(long *)(lVar2 + 8);
    lVar2 = lVar1;
    _swift_getObjectType();
    uVar3 = 0;
    FUN_100531a1c();
    ppuStack_60 = &PTR_PTR_10418d0a8;
    local_80[0] = param_1;
    local_68 = uVar3;
    (**(code **)(lVar4 + 0x18))(local_80,lVar2,lVar4);
    _swift_unknownObjectRelease(lVar1);
    FUN_100012e70(local_80);
  }
  return;
}


