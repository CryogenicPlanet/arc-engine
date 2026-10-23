// ==== 0x8371eb8  ArcImageRep::encodeToJPEGWithQuality:  (192 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x14 bytes */

ID ArcImageRep::encodeToJPEGWithQuality_(ID param_1,SEL param_2,float param_3)

{
  uint uVar1;
  long lVar2;
  ulong uVar3;
  code *pcVar4;
  undefined8 uVar5;
  ID IVar6;
  long local_50;
  ulong uStack_48;
  undefined8 local_40;
  undefined8 local_38;
  
  uStack_48 = 0;
  local_50 = 0;
  local_38 = 0;
  local_40 = 0;
  uVar1 = (int)(param_3 * 100.0) & ((int)(param_3 * 100.0) >> 0x1f ^ 0xffffffffU);
  if (99 < (int)uVar1) {
    uVar1 = 100;
  }
  uVar5 = FUN_022f4af4(*(undefined8 *)(param_1 + 0x10));
  FUN_08b0218c(&local_50,uVar5,uVar1);
  uVar3 = uStack_48;
  lVar2 = local_50;
  if ((char)local_38 == '\0') {
    IVar6 = 0;
  }
  else {
    IVar6 = NSData::dataWithBytes_length_
                      ((ID)&_OBJC_CLASS___NSData,"dataWithBytes:length:",local_50,uStack_48);
    if (lVar2 != 0) {
      if ((ulong)-lVar2 < uVar3) {
                    /* WARNING: Does not return */
        pcVar4 = (code *)SoftwareBreakpoint(1,0x8371f68);
        (*pcVar4)();
      }
      thunk_FUN_00005810(lVar2,local_40);
    }
  }
  return IVar6;
}


