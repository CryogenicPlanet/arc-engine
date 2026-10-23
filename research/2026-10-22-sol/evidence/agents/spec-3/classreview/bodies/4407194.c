// ==== 0x4407194  FUN_04407194  (68 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04407194(undefined8 *param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  long lVar2;
  
  lVar2 = *(long *)*param_1;
  uVar1 = FUN_01c391bc(*param_2,param_2[1]);
  (**(code **)(lVar2 + 0x10))(lVar2,uVar1);
  return 0;
}


