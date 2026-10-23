// ==== 0x4738ad4  FUN_04738ad4  (172 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04738ad4(long param_1)

{
  undefined8 local_38;
  undefined8 local_30;
  long local_28;
  
  local_28 = *(long *)(param_1 + 0x78);
  if (local_28 != 0) {
    FUN_0473d534(&local_28,0);
    local_30 = *(undefined8 *)(param_1 + 0x78);
    FUN_0473d3cc(&local_30,0);
    local_38 = *(undefined8 *)(param_1 + 0x78);
    FUN_0473d128(&local_38);
    *(undefined8 *)(param_1 + 0x78) = 0;
    *(undefined1 *)(param_1 + 0x90) = 0;
  }
  return 0;
}


