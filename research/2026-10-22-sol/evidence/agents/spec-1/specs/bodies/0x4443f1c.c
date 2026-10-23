// ==== 0x4443f1c  ArcWebContents::web_contents_navigation_delegate_bridge  (32 bytes)

/* Function Stack Size: 0x10 bytes */

shared_ptr<ArcWebContentsNavigationDelegateBridge>
ArcWebContents::web_contents_navigation_delegate_bridge(ID param_1,SEL param_2)

{
  long *plVar1;
  shared_ptr<ArcWebContentsNavigationDelegateBridge> sVar2;
  
  sVar2 = *(shared_ptr<ArcWebContentsNavigationDelegateBridge> *)(param_1 + 0x98);
  if (*(long *)(param_1 + 0xa0) != 0) {
    plVar1 = (long *)(*(long *)(param_1 + 0xa0) + 8);
    *plVar1 = *plVar1 + 1;
  }
  return sVar2;
}


