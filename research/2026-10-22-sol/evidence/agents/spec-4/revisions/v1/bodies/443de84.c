// ==== 0x443de84  ArcURLUtilities::formatHostForDisplay:  (200 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcURLUtilities::formatHostForDisplay_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 ***pppuVar1;
  ID IVar2;
  SEL extraout_x1;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if (param_3 == 0) {
    local_50 = 0;
    uStack_48 = 0;
    local_40 = 0;
  }
  else {
    FUN_01b356b8(&local_50,param_3);
    param_2 = extraout_x1;
  }
  FUN_0434563c(&local_38,&local_50,param_2);
  pppuVar1 = (undefined8 ***)local_38;
  if (-1 < (long)cStack_21) {
    pppuVar1 = &local_38;
  }
  if (-1 < cStack_21) {
    lStack_30 = (long)cStack_21;
  }
  IVar2 = FUN_01c391bc(pppuVar1,lStack_30);
  if (cStack_21 < '\0') {
    FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
  }
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  return IVar2;
}


