// ==== 0x43f8954  FUN_043f8954  (96 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043f8954(undefined8 *param_1,long param_2)

{
  undefined8 uVar1;
  long lVar2;
  
  lVar2 = *(long *)*param_1;
  if ((**(long **)(param_2 + 8) == 0) || (*(long *)(**(long **)(param_2 + 8) + 0x18) == 0)) {
    uVar1 = 0;
  }
  else {
    uVar1 = FUN_02a02ccc();
  }
  (**(code **)(lVar2 + 0x10))(lVar2,uVar1);
  return 0;
}


