// ==== 0x100554564  _TtC3ADK25PerformanceObserverBridge::-[PerformanceObserverBridge_onBatterySaverModeChanged:]  (92 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x14 bytes */

void _TtC3ADK25PerformanceObserverBridge::__PerformanceObserverBridge_onBatterySaverModeChanged__
               (ID param_1,SEL param_2,bool param_3)

{
  undefined1 local_31;
  
  FUN_100554324(param_1);
  local_31 = (undefined1)param_3;
  Combine::PassthroughSubject::send((PassthroughSubject *)param_1,&local_31);
  _swift_release(param_1);
  return;
}


