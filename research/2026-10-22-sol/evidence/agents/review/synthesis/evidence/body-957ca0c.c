// ==== 0x957ca0c  BrowserAccessibilityCocoa::accessibilityIsAttributeSettable:  (464 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool BrowserAccessibilityCocoa::accessibilityIsAttributeSettable_(ID param_1,SEL param_2,ID param_3)

{
  bool bVar1;
  ID IVar2;
  ulong uVar3;
  ID local_58;
  ID local_50;
  ID *local_48;
  ID *pIStack_40;
  undefined8 *local_38;
  undefined8 local_30;
  ID **local_28;
  
  local_30 = 1;
  local_48 = &local_50;
  pIStack_40 = &local_58;
  local_38 = &local_30;
  local_28 = &local_48;
  local_58 = param_3;
  local_50 = param_1;
  if (DAT_0e4a4451 != '\0') {
    FUN_0957f1d4(&local_28);
  }
  IVar2 = _objc_msgSend(local_50,"instanceActive");
  if ((IVar2 & 1) != 0) {
    IVar2 = _objc_msgSend(local_50,"class");
    IVar2 = _objc_msgSend(IVar2,"isAttributeAvailableThroughNewAccessibilityAPI:",local_58);
    if ((IVar2 & 1) == 0) {
      IVar2 = _objc_msgSend(local_58,"isEqualToString:",
                            *(undefined8 *)PTR__NSAccessibilityFocusedAttribute_0d8277b0);
      if ((int)IVar2 == 0) {
        IVar2 = _objc_msgSend(local_58,"isEqualToString:",
                              *(undefined8 *)PTR__NSAccessibilityValueAttribute_0d827a70);
        if ((int)IVar2 == 0) {
          IVar2 = _objc_msgSend(local_58,"isEqualToString:",
                                *(undefined8 *)
                                 PTR__NSAccessibilitySelectedTextRangeAttribute_0d827a50);
          if (((int)IVar2 == 0) ||
             (uVar3 = FUN_095831c0(*(undefined8 *)(local_50 + (long)_owner),4), (uVar3 & 1) == 0)) {
            IVar2 = _objc_msgSend(local_58,"isEqualToString:",&cf_AXSelectedTextMarkerRange);
            bVar1 = (bool)IVar2;
          }
          else {
            bVar1 = 1;
          }
        }
        else {
          bVar1 = FUN_09583fb8(*(undefined8 *)(local_50 + (long)_owner),0x1e);
        }
        goto LAB_0957cadc;
      }
      IVar2 = _objc_msgSend(local_50,"internalRole");
      if ((int)IVar2 != 0x1c) {
        bVar1 = FUN_09583174(*(undefined8 *)(local_50 + (long)_owner));
        goto LAB_0957cadc;
      }
    }
  }
  bVar1 = 0;
LAB_0957cadc:
  local_28 = (ID **)0x1;
  if (DAT_0e4a4451 != '\0') {
    local_48 = (ID *)0x0;
    thunk_FUN_0000a280(DAT_0e4a4451,&local_28,&local_48,2);
  }
  return bVar1;
}


