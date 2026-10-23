// ==== 0x442ead8  ArcExtractedPageContent::rootNode  (96 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcExtractedPageContent::rootNode(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  IVar2 = *(ID *)(param_1 + 8);
  if (IVar2 == 0) {
    IVar2 = _objc_alloc(&objc::class_t::ArcContentNode);
    if ((*(byte *)(param_1 + 0x1e8) & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442eb38);
      (*pcVar1)();
    }
    IVar2 = ArcContentNode::initWithContentNode_
                      (IVar2,"initWithContentNode:",(const_void__)(param_1 + 0x48));
    *(ID *)(param_1 + 8) = IVar2;
    IVar2 = *(ID *)(param_1 + 8);
  }
  return IVar2;
}


