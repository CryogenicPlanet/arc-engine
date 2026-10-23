// ==== 0x444458c  ArcWebContents::web_contents_top_band_color_delegate_bridge  (32 bytes)

/* Function Stack Size: 0x10 bytes */

shared_ptr<ArcWebContentsTopBandColorDelegateBridge>
ArcWebContents::web_contents_top_band_color_delegate_bridge(ID param_1,SEL param_2)

{
  long *plVar1;
  shared_ptr<ArcWebContentsTopBandColorDelegateBridge> sVar2;
  
  sVar2 = *(shared_ptr<ArcWebContentsTopBandColorDelegateBridge> *)(param_1 + 0xd8);
  if (*(long *)(param_1 + 0xe0) != 0) {
    plVar1 = (long *)(*(long *)(param_1 + 0xe0) + 8);
    *plVar1 = *plVar1 + 1;
  }
  return sVar2;
}


