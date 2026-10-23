// ==== 0x442c1e8  ArcSelectOption::.cxx_destruct  (108 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcSelectOption::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x50) != '\0') {
    if ((*(char *)(param_1 + 0x40) != '\0') && (*(char *)(param_1 + 0x3f) < '\0')) {
      FUN_00005810(*(undefined8 *)(param_1 + 0x28),*(ulong *)(param_1 + 0x38) & 0x7fffffffffffffff);
    }
    if ((*(char *)(param_1 + 0x20) != '\0') && (*(char *)(param_1 + 0x1f) < '\0')) {
      FUN_00005810(*(undefined8 *)(param_1 + 8),*(ulong *)(param_1 + 0x18) & 0x7fffffffffffffff);
      return;
    }
  }
  return;
}


