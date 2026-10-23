// ==== 0x440c764  ArcTracingConfiguration::setFilePath:  (156 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcTracingConfiguration::setFilePath_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  
  *(ID *)(param_1 + 8) = param_3;
  if (param_3 == 0) {
    local_38 = 0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    FUN_01b356b8(&local_38,param_3);
  }
  if (*(char *)(param_1 + 0x2f) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x18),*(ulong *)(param_1 + 0x28) & 0x7fffffffffffffff);
  }
  *(undefined8 *)(param_1 + 0x20) = uStack_30;
  *(undefined8 *)(param_1 + 0x18) = local_38;
  *(undefined8 *)(param_1 + 0x28) = local_28;
  return;
}


