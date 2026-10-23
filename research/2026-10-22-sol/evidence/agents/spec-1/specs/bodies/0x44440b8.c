// ==== 0x44440b8  ArcWebContents::web_contents_ui_delegate_bridge  (32 bytes)

/* Function Stack Size: 0x10 bytes */

shared_ptr<ArcWebContentsUIDelegateBridge>
ArcWebContents::web_contents_ui_delegate_bridge(ID param_1,SEL param_2)

{
  long *plVar1;
  shared_ptr<ArcWebContentsUIDelegateBridge> sVar2;
  
  sVar2 = *(shared_ptr<ArcWebContentsUIDelegateBridge> *)(param_1 + 0xa8);
  if (*(long *)(param_1 + 0xb0) != 0) {
    plVar1 = (long *)(*(long *)(param_1 + 0xb0) + 8);
    *plVar1 = *plVar1 + 1;
  }
  return sVar2;
}


