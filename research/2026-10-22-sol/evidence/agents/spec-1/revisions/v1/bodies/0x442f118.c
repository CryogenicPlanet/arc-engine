// ==== 0x442f118  ArcExtensionRuntimeError::.cxx_destruct  (52 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcExtensionRuntimeError::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x90) != '\0') {
    FUN_0433587c(param_1 + 0x10);
  }
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}


