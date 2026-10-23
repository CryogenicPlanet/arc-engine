// ==== 0x442db28  ArcMostVisitedURL::.cxx_destruct  (92 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcMostVisitedURL::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x38) != '\0') {
    if (*(char *)(param_1 + 0x37) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x20),*(long *)(param_1 + 0x30) << 1);
    }
    if (*(char *)(param_1 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 8),*(ulong *)(param_1 + 0x18) & 0x7fffffffffffffff);
      return;
    }
  }
  return;
}


