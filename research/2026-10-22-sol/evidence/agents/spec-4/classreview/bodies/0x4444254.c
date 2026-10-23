// ==== 0x4444254  ArcWebContents::web_contents_gesture_handler_bridge  (32 bytes)

/* Function Stack Size: 0x10 bytes */

shared_ptr<ArcWebContentsGestureHandlerBridge>
ArcWebContents::web_contents_gesture_handler_bridge(ID param_1,SEL param_2)

{
  long *plVar1;
  shared_ptr<ArcWebContentsGestureHandlerBridge> sVar2;
  
  sVar2 = *(shared_ptr<ArcWebContentsGestureHandlerBridge> *)(param_1 + 0xb8);
  if (*(long *)(param_1 + 0xc0) != 0) {
    plVar1 = (long *)(*(long *)(param_1 + 0xc0) + 8);
    *plVar1 = *plVar1 + 1;
  }
  return sVar2;
}


