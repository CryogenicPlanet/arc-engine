// ==== 0x4402a3c  ArcHistogramSample::initWithHistogramSample:  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcHistogramSample::initWithHistogramSample_
             (ID param_1,SEL param_2,const_HistogramSample__ param_3)

{
  ID IVar1;
  ID IVar2;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcHistogramSample;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    IVar2 = NSNumber::numberWithInt_
                      ((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:",(ulong)param_3->field0_0x0);
    *(ID *)(IVar1 + 0x10) = IVar2;
    *(byte *)(IVar1 + 8) = *(byte *)&(param_3->field1_0x4).field0_0x0 & 1;
    *(byte *)(IVar1 + 9) = *(byte *)&(param_3->field1_0x4).field0_0x0 >> 1 & 1;
    *(byte *)(IVar1 + 10) = *(byte *)&(param_3->field1_0x4).field0_0x0 >> 2 & 1;
  }
  return IVar1;
}


