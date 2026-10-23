// ==== 0x440c8ac  ArcTracingConfiguration::.cxx_destruct  (60 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcTracingConfiguration::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x2f) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x18),*(ulong *)(param_1 + 0x28) & 0x7fffffffffffffff);
  }
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}


