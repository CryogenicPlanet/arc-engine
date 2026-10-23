// ==== 0x440a1f8  ArcNotification::didReceiveResponse:  (180 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcNotification::didReceiveResponse_(ID param_1,SEL param_2,ID param_3)

{
  int iVar1;
  undefined8 local_30;
  int local_28;
  
  local_28 = 0;
  local_30 = 0;
  FUN_0473f138(&local_30,param_3);
  if (local_28 == 0) {
    didClose_(param_1,"didClose:",1);
  }
  else if (local_28 == 2) {
    didClickSettings(param_1,"didClickSettings");
  }
  else if (local_28 == 1) {
    iVar1 = (int)local_30;
    if (local_30._4_1_ == '\0') {
      iVar1 = 0xffffffff;
    }
    didClick_input_(param_1,"didClick:input:",iVar1,0);
  }
  return;
}


