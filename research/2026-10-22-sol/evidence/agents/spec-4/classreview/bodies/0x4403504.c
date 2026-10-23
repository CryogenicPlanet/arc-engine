// ==== 0x4403504  ArcAdhocHistogramSamples::initWithAdhocHistogramSample:  (264 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcAdhocHistogramSamples::initWithAdhocHistogramSample_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  uint *puVar1;
  code *pcVar2;
  ID IVar3;
  undefined8 uVar4;
  ID IVar5;
  uint *puVar6;
  ID local_60;
  class_t *pcStack_58;
  
  pcStack_58 = &objc::class_t::ArcAdhocHistogramSamples;
  local_60 = param_1;
  IVar3 = NSObject::init((ID)&local_60,"init");
  if (IVar3 != 0) {
    uVar4 = _objc_alloc_init(&_OBJC_CLASS___NSMutableArray);
    *(undefined8 *)(IVar3 + 8) = uVar4;
    if ((*(ulong *)((long)param_3 + 0x10) & 0x1fffffffffffffff) != 0) {
      puVar6 = *(uint **)((long)param_3 + 8);
      puVar1 = puVar6 + *(ulong *)((long)param_3 + 0x10) * 2;
      do {
        IVar5 = _objc_alloc_init(&objc::class_t::ArcAdhocHistogramSample);
        if (4 < *puVar6) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0x550b,0x440360c);
          (*pcVar2)();
        }
        AlertBridgeHelper::setAlertBridge_(IVar5,"setType:",(void *)(ulong)*puVar6);
        ArcAdhocHistogramSample::setValue_(IVar5,"setValue:",puVar6[1]);
        _objc_msgSend(*(ID *)(IVar3 + 8),"addObject:",IVar5);
        puVar6 = puVar6 + 2;
      } while (puVar6 != puVar1);
    }
  }
  return IVar3;
}


