// ==== 0x44312a8  ArcContentNode::.cxx_destruct  (64 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcContentNode::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x1b8) != '\0') {
    FUN_04295208(param_1 + 0x18);
  }
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}


