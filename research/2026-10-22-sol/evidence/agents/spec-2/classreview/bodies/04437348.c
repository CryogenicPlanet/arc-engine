// ==== 0x4437348  ArcBrowserContextDevTools::webContents  (84 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContextDevTools::webContents(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  const_void__ pvVar3;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    pvVar3 = *(const_void__ *)(*(long *)(param_1 + 8) + 0x98);
    if (pvVar3 == (const_void__)0x0) {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcWebContents);
      IVar2 = ArcWebContents::initWithWebContents_(IVar2,"initWithWebContents:",pvVar3);
    }
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443739c);
  (*pcVar1)();
}


