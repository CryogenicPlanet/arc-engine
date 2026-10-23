// ==== 0x442eff4  ArcExtensionRuntimeError::stackTrace  (220 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcExtensionRuntimeError::stackTrace(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  ID IVar3;
  const_void__ pvVar4;
  long lVar5;
  
  IVar2 = *(ID *)(param_1 + 8);
  if (IVar2 == 0) {
    if ((*(byte *)(param_1 + 0x90) & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442f0d0);
      (*pcVar1)();
    }
    IVar2 = NSMutableArray::arrayWithCapacity_
                      ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",
                       *(undefined8 *)(param_1 + 0x80));
    lVar5 = *(long *)(param_1 + 0x80) * 0x38;
    if (lVar5 != 0) {
      pvVar4 = *(const_void__ *)(param_1 + 0x78);
      do {
        IVar3 = _objc_alloc(&objc::class_t::ArcExtensionRuntimeErrorStackFrame);
        IVar3 = ArcExtensionRuntimeErrorStackFrame::initWithExtensionRuntimeErrorStackFrame_
                          (IVar3,"initWithExtensionRuntimeErrorStackFrame:",pvVar4);
        NSMutableArray::addObject_(IVar2,"addObject:",IVar3);
        pvVar4 = (const_void__)((long)pvVar4 + 0x38);
        lVar5 = lVar5 + -0x38;
      } while (lVar5 != 0);
    }
    *(ID *)(param_1 + 8) = IVar2;
    IVar2 = *(ID *)(param_1 + 8);
  }
  return IVar2;
}


