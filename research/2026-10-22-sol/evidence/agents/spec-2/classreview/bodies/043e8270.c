// ==== 0x43e8270  ArcBrowserApplication::setStringFeatureValue:value:  (136 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcBrowserApplication::setStringFeatureValue_value_
               (ID param_1,SEL param_2,unsigned_long_long param_3,ID param_4)

{
  undefined8 local_38;
  undefined8 uStack_30;
  ulong local_28;
  
  if (param_4 == 0) {
    local_38 = 0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    FUN_01b356b8(&local_38,param_4);
  }
  FUN_042b88c8(param_3,&local_38);
  if ((long)local_28 < 0) {
    FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
  }
  return;
}


