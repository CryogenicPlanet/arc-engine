// ==== 0x442f0d0  ArcExtensionRuntimeError::extension_runtime_error  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcExtensionRuntimeError::extension_runtime_error
               (optional<arc::ExtensionRuntimeError> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x80 = 0;
  if (*(char *)(param_1 + 0x90) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::ExtensionRuntimeError> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442f118);
      (*pcVar1)();
    }
    FUN_04335698(__return_storage_ptr__,param_1 + 0x10);
    *(undefined1 *)&__return_storage_ptr__->field1_0x80 = 1;
  }
  return;
}


