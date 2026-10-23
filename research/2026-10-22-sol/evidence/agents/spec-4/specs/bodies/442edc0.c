// ==== 0x442edc0  ArcExtensionRuntimeErrorStackFrame::extension_runtime_error_stack_frame  (152 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcExtensionRuntimeErrorStackFrame::extension_runtime_error_stack_frame
               (optional<arc::StackFrame> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x38 = 0;
  if (*(char *)(param_1 + 0x40) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::StackFrame> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442ee58);
      (*pcVar1)();
    }
    puVar2 = (undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 8);
    *(undefined8 *)&__return_storage_ptr__->field0_0x0 = *(undefined8 *)(param_1 + 8);
    if (*(char *)(param_1 + 0x27) < '\0') {
      FUN_00054420(puVar2,*(undefined8 *)(param_1 + 0x10),*(undefined8 *)(param_1 + 0x18));
    }
    else {
      uVar3 = *(undefined8 *)(param_1 + 0x10);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x10) =
           *(undefined8 *)(param_1 + 0x18);
      *puVar2 = uVar3;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x18) =
           *(undefined8 *)(param_1 + 0x20);
    }
    if (*(char *)(param_1 + 0x3f) < '\0') {
      FUN_00054420((undefined1 *)((long)&__return_storage_ptr__->field0_0x0 + 0x20),
                   *(undefined8 *)(param_1 + 0x28),*(undefined8 *)(param_1 + 0x30));
    }
    else {
      uVar3 = *(undefined8 *)(param_1 + 0x28);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x28) =
           *(undefined8 *)(param_1 + 0x30);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x20) = uVar3;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x30) =
           *(undefined8 *)(param_1 + 0x38);
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x38 = 1;
  }
  return;
}


