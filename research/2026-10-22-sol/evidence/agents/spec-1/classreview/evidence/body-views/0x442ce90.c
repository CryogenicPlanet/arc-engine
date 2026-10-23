// ==== 0x442ce90  ArcRedirectEntry::redirect_entry  (152 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcRedirectEntry::redirect_entry
               (optional<arc::RedirectEntry> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x38 = 0;
  if (*(char *)(param_1 + 0x40) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::RedirectEntry> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442cf28);
      (*pcVar1)();
    }
    if (*(char *)(param_1 + 0x1f) < '\0') {
      FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 8),
                   *(undefined8 *)(param_1 + 0x10));
    }
    else {
      uVar2 = *(undefined8 *)(param_1 + 8);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 8) =
           *(undefined8 *)(param_1 + 0x10);
      *(undefined8 *)&__return_storage_ptr__->field0_0x0 = uVar2;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x10) =
           *(undefined8 *)(param_1 + 0x18);
    }
    if (*(char *)(param_1 + 0x37) < '\0') {
      FUN_00054420((undefined1 *)((long)&__return_storage_ptr__->field0_0x0 + 0x18),
                   *(undefined8 *)(param_1 + 0x20),*(undefined8 *)(param_1 + 0x28));
    }
    else {
      uVar2 = *(undefined8 *)(param_1 + 0x20);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x20) =
           *(undefined8 *)(param_1 + 0x28);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x18) = uVar2;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x28) =
           *(undefined8 *)(param_1 + 0x30);
    }
    *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x30) =
         *(undefined8 *)(param_1 + 0x38);
    *(undefined1 *)&__return_storage_ptr__->field1_0x38 = 1;
  }
  return;
}


