// ==== 0x44443f0  ArcWebContents::web_contents_actions_delegate_bridge  (32 bytes)

/* Function Stack Size: 0x10 bytes */

shared_ptr<ArcWebContentsActionsDelegateBridge>
ArcWebContents::web_contents_actions_delegate_bridge(ID param_1,SEL param_2)

{
  long *plVar1;
  shared_ptr<ArcWebContentsActionsDelegateBridge> sVar2;
  
  sVar2 = *(shared_ptr<ArcWebContentsActionsDelegateBridge> *)(param_1 + 200);
  if (*(long *)(param_1 + 0xd0) != 0) {
    plVar1 = (long *)(*(long *)(param_1 + 0xd0) + 8);
    *plVar1 = *plVar1 + 1;
  }
  return sVar2;
}


