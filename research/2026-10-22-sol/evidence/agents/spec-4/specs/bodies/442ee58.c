// ==== 0x442ee58  ArcExtensionRuntimeErrorStackFrame::.cxx_destruct  (92 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcExtensionRuntimeErrorStackFrame::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x40) != '\0') {
    if (*(char *)(param_1 + 0x3f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x28),*(long *)(param_1 + 0x38) << 1);
    }
    if (*(char *)(param_1 + 0x27) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x10),*(long *)(param_1 + 0x20) << 1);
      return;
    }
  }
  return;
}


