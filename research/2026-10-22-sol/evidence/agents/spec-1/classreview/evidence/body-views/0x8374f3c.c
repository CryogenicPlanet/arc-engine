// ==== 0x8374f3c  ArcLoginCredentials::password  (128 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcLoginCredentials::password(ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  code *pcVar2;
  ID IVar3;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_06bd82ac(&local_38,*(long *)(param_1 + 8) + 0x2e0);
    pppuVar1 = (undefined8 ***)local_38;
    if (-1 < (long)cStack_21) {
      pppuVar1 = &local_38;
    }
    if (-1 < cStack_21) {
      lStack_30 = (long)cStack_21;
    }
    IVar3 = FUN_01d9fb90(pppuVar1,lStack_30);
    if (cStack_21 < '\0') {
      thunk_FUN_00005810(local_38,CONCAT17(cStack_21,local_28) << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x8374fbc);
  (*pcVar2)();
}


