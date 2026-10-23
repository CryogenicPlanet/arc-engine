// ==== 0x3b60ee8  FUN_03b60ee8  (68 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_03b60ee8(undefined8 *param_1,long param_2)

{
  if (param_2 == 0) {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
  }
  else {
    FUN_01b356b8(param_1,param_2);
  }
  return 0;
}


