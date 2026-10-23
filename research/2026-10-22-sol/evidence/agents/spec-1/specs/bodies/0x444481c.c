// ==== 0x444481c  ArcWebContents::userActivityObserver  (32 bytes)

/* Function Stack Size: 0x10 bytes */

shared_ptr<arc::WebContentsObserver> ArcWebContents::userActivityObserver(ID param_1,SEL param_2)

{
  long *plVar1;
  shared_ptr<arc::WebContentsObserver> sVar2;
  
  sVar2 = *(shared_ptr<arc::WebContentsObserver> *)(param_1 + 0xf8);
  if (*(long *)(param_1 + 0x100) != 0) {
    plVar1 = (long *)(*(long *)(param_1 + 0x100) + 8);
    *plVar1 = *plVar1 + 1;
  }
  return sVar2;
}


