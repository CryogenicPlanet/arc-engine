// ==== 0x44034ac  ArcAdhocHistogramSample::initWithType:value:  (80 bytes)

/* Function Stack Size: 0x1c bytes */

ID ArcAdhocHistogramSample::initWithType_value_
             (ID param_1,SEL param_2,unsigned_long_long param_3,int param_4)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAdhocHistogramSample;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    *(unsigned_long_long *)(IVar1 + 0x10) = param_3;
    *(int *)(IVar1 + 8) = param_4;
  }
  return IVar1;
}


