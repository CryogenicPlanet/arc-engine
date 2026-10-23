// ==== 0x43eb604  FUN_043eb604  (108 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043eb604(undefined8 *param_1,const_void__ param_2)

{
  ID self;
  ID IVar1;
  
  self = *(ID *)*param_1;
  if (self != 0) {
    IVar1 = _objc_alloc(&objc::class_t::ArcAdhocHistogramSamples);
    IVar1 = ArcAdhocHistogramSamples::initWithAdhocHistogramSample_
                      (IVar1,"initWithAdhocHistogramSample:",param_2);
    _objc_msgSend(self,"adhocHistogramSamplesAvailable:",IVar1);
  }
  return 0;
}


