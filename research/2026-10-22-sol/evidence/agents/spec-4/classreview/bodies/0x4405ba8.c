// ==== 0x4405ba8  ArcImageRep::encodeToPNGWithMethod:  (180 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcImageRep::encodeToPNGWithMethod_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  code *pcVar1;
  ID IVar2;
  undefined8 uVar3;
  long local_40;
  ulong uStack_38;
  undefined8 local_30;
  undefined8 local_28;
  
  if (param_3 == 0) {
    uVar3 = 0;
  }
  else {
    if (param_3 != 1) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x4405c5c);
      (*pcVar1)();
    }
    uVar3 = 1;
  }
  uStack_38 = 0;
  local_40 = 0;
  local_28 = 0;
  local_30 = 0;
  FUN_043385c0(&local_40,param_1 + 8,uVar3);
  if ((char)local_28 == '\0') {
    IVar2 = 0;
  }
  else {
    IVar2 = NSData::dataWithBytes_length_
                      ((ID)&_OBJC_CLASS___NSData,"dataWithBytes:length:",local_40,uStack_38);
    if (((char)local_28 != '\0') && (local_40 != 0)) {
      if ((ulong)-local_40 < uStack_38) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(1,0x4405c30);
        (*pcVar1)();
      }
      uStack_38 = 0;
      FUN_00005810(local_40,local_30);
    }
  }
  return IVar2;
}


