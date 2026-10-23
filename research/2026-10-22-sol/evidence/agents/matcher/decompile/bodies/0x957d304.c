// ==== 0x957d304  BrowserAccessibilityCocoa::accessibilityActionDescription:  (216 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID BrowserAccessibilityCocoa::accessibilityActionDescription_(ID param_1,SEL param_2,ID param_3)

{
  ID IVar1;
  ID local_58;
  ID local_50;
  ID *local_48;
  ID *pIStack_40;
  undefined8 *local_38;
  undefined8 local_30;
  undefined8 **local_28;
  
  local_30 = 1;
  local_48 = &local_50;
  pIStack_40 = &local_58;
  local_38 = &local_30;
  local_28 = &local_48;
  local_58 = param_3;
  local_50 = param_1;
  if (DAT_0e4a4451 != '\0') {
    FUN_0957f444(&local_28);
  }
  IVar1 = _objc_msgSend(local_50,"instanceActive");
  if ((IVar1 & 1) == 0) {
    IVar1 = 0;
  }
  else {
    IVar1 = _NSAccessibilityActionDescription(local_58);
  }
  local_28 = (undefined8 **)0x1;
  if (DAT_0e4a4451 != '\0') {
    local_48 = (ID *)0x0;
    thunk_FUN_0000a280(DAT_0e4a4451,&local_28,&local_48,2);
  }
  return IVar1;
}


