// ==== 0x4401520  ArcBrowserSidePanel::webContents  (80 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserSidePanel::webContents(ID param_1,SEL param_2)

{
  ID IVar1;
  
  if (*(long *)(*(long *)(param_1 + 8) + 0x60) != 0) {
    IVar1 = _objc_alloc(&objc::class_t::ArcWebContents);
    IVar1 = ArcWebContents::initWithWebContents_
                      (IVar1,"initWithWebContents:",*(const_void__ *)(*(long *)(param_1 + 8) + 0x60)
                      );
    return IVar1;
  }
  return 0;
}


