// ==== 0x443a318  ArcAIInferenceConfiguration::init:streamAfterEveryNTokens:  (92 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcAIInferenceConfiguration::init_streamAfterEveryNTokens_
             (ID param_1,SEL param_2,int param_3,int param_4)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAIInferenceConfiguration;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    *(undefined8 *)(IVar1 + 8) = 0;
    *(undefined1 *)(IVar1 + 0x10) = 1;
    *(int *)(IVar1 + 8) = param_3;
    *(short *)(IVar1 + 0xc) = (short)param_4;
  }
  return IVar1;
}


