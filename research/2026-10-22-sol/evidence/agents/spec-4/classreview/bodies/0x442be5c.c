// ==== 0x442be5c  ArcFormControlInfo::form_control_info  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcFormControlInfo::form_control_info
               (optional<arc::FormControlInfo> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 8) = 0;
  if (*(char *)(param_1 + 0x98) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::FormControlInfo> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442bea4);
      (*pcVar1)();
    }
    FUN_04294be4(__return_storage_ptr__,param_1 + 0x10);
    *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 8) = 1;
  }
  return;
}


