// ==== 0x44311e0  ArcContentNode::formControlInfo  (128 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcContentNode::formControlInfo(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  IVar2 = *(ID *)(param_1 + 0x10);
  if (IVar2 == 0) {
    if ((*(byte *)(param_1 + 0x1b8) & 1) == 0) {
LAB_0443125c:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4431260);
      (*pcVar1)();
    }
    if (*(char *)(param_1 + 0x150) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcFormControlInfo);
      if ((*(byte *)(param_1 + 0x150) & 1) == 0) goto LAB_0443125c;
      IVar2 = ArcFormControlInfo::initWithFormControlInfo_
                        (IVar2,"initWithFormControlInfo:",(const_void__)(param_1 + 200));
      *(ID *)(param_1 + 0x10) = IVar2;
      IVar2 = *(ID *)(param_1 + 0x10);
    }
  }
  return IVar2;
}


