// ==== 0x443dd58  ArcURLUtilities::effectiveTopLevelDomainFromHost:includePrivateRegistries:  (240 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x1c bytes */

ID ArcURLUtilities::effectiveTopLevelDomainFromHost_includePrivateRegistries_
             (ID param_1,SEL param_2,ID param_3,bool param_4)

{
  ulong uVar1;
  undefined8 *****pppppuVar2;
  uint uVar3;
  ID IVar4;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  undefined8 ****local_38;
  ulong local_30;
  undefined8 local_28;
  
  local_38 = (undefined8 *****)0x0;
  local_30 = 0;
  local_28 = 0;
  if (param_3 == 0) {
    local_50 = 0;
    uStack_48 = 0;
    local_40 = 0;
  }
  else {
    FUN_01b356b8(&local_50,param_3);
  }
  FUN_04345480(&local_38,&local_50,param_4);
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  uVar3 = (uint)(char)local_28._7_1_;
  uVar1 = local_30;
  if (-1 < (int)uVar3) {
    uVar1 = (ulong)local_28._7_1_;
  }
  if (uVar1 == 0) {
    IVar4 = 0;
    if ((uVar3 >> 7 & 1) == 0) {
      return 0;
    }
  }
  else {
    pppppuVar2 = (undefined8 *****)local_38;
    if (-1 < (int)uVar3) {
      pppppuVar2 = &local_38;
    }
    IVar4 = FUN_01c391bc(pppppuVar2);
    if (-1 < (long)local_28) {
      return IVar4;
    }
  }
  FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
  return IVar4;
}


