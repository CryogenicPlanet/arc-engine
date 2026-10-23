// ==== 0x43eb534  FUN_043eb534  (108 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043eb534(undefined8 *param_1,const_void__ param_2)

{
  ID self;
  ID IVar1;
  
  self = *(ID *)*param_1;
  if (self != 0) {
    IVar1 = _objc_alloc(&objc::class_t::ArcHistogramSamples);
    IVar1 = ArcHistogramSamples::initWithPeriodicHistogramSample_
                      (IVar1,"initWithPeriodicHistogramSample:",param_2);
    _objc_msgSend(self,"periodicHistogramSamplesAvailable:",IVar1);
  }
  return 0;
}


