// ==== 0x440d834  ArcURLLoadConfiguration::setPayloadData:  (124 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setPayloadData_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  ID IVar2;
  ID IVar3;
  
  IVar2 = _objc_retainAutorelease(param_3);
  IVar2 = _objc_msgSend(IVar2,"bytes");
  IVar3 = _objc_msgSend(param_3,"length");
  if (IVar2 != 0 || IVar3 == 0) {
    FUN_0043b8b0(param_1 + 0x20,IVar2,IVar3);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440d8b0);
  (*pcVar1)();
}


