// ==== 0x440d76c  ArcURLLoadConfiguration::setPayload:  (128 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setPayload_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  
  if (param_3 == 0) {
    local_38 = 0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    FUN_01b356b8(&local_38,param_3);
  }
  if (*(char *)(param_1 + 0x37) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x20),*(ulong *)(param_1 + 0x30) & 0x7fffffffffffffff);
  }
  *(undefined8 *)(param_1 + 0x28) = uStack_30;
  *(undefined8 *)(param_1 + 0x20) = local_38;
  *(undefined8 *)(param_1 + 0x30) = local_28;
  return;
}


