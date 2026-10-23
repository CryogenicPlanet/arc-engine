// ==== 0x442da98  ArcMostVisitedURL::most_visited_site  (144 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcMostVisitedURL::most_visited_site
               (optional<arc::MostVisitedURL> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x30 = 0;
  if (*(char *)(param_1 + 0x38) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::MostVisitedURL> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442db28);
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
    *(undefined1 *)&__return_storage_ptr__->field1_0x30 = 1;
  }
  return;
}


