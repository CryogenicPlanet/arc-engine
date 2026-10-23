// ==== 0x442ebe0  ArcExtractedPageContent::.cxx_destruct  (100 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcExtractedPageContent::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x1e8) != '\0') {
    FUN_04295208(param_1 + 0x48);
    if (*(char *)(param_1 + 0x3f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x28),*(ulong *)(param_1 + 0x38) & 0x7fffffffffffffff);
    }
    if (*(char *)(param_1 + 0x27) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x10),*(ulong *)(param_1 + 0x20) & 0x7fffffffffffffff);
    }
  }
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}


