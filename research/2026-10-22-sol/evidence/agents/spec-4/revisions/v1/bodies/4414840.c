// ==== 0x4414840  ArcWebContentsBoostIDE::boostIDEDelegateBridge  (32 bytes)

/* Function Stack Size: 0x10 bytes */

shared_ptr<BoostIDEDelegateBridge>
ArcWebContentsBoostIDE::boostIDEDelegateBridge(ID param_1,SEL param_2)

{
  long *plVar1;
  shared_ptr<BoostIDEDelegateBridge> sVar2;
  
  sVar2 = *(shared_ptr<BoostIDEDelegateBridge> *)(param_1 + 8);
  if (*(long *)(param_1 + 0x10) != 0) {
    plVar1 = (long *)(*(long *)(param_1 + 0x10) + 8);
    *plVar1 = *plVar1 + 1;
  }
  return sVar2;
}


