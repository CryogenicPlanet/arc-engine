// ==== 0x443c0a8  ArcGPULogMessage::initWithGPULogMessage:  (88 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcGPULogMessage::initWithGPULogMessage_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcGPULogMessage;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    FUN_0441e818(IVar1 + 8,param_3);
  }
  return IVar1;
}


