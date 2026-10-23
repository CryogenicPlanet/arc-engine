// ==== 0x43decac  ArcAccessibilityActionData::actionData  (104 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAccessibilityActionData::actionData
               (AccessibilityActionData *__return_storage_ptr__,ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  __return_storage_ptr__->field0_0x0 = *(int *)(param_1 + 8);
  puVar1 = (undefined8 *)((long)&(__return_storage_ptr__->field1_0x4).field0_0x0 + 4);
  *(undefined1 *)puVar1 = 0;
  *(undefined1 *)&__return_storage_ptr__[1].field0_0x0 = 0;
  if (*(char *)(param_1 + 0x28) != '\0') {
    if (*(char *)(param_1 + 0x27) < '\0') {
      FUN_00060a60(puVar1,*(undefined8 *)(param_1 + 0x10),*(undefined8 *)(param_1 + 0x18));
    }
    else {
      uVar2 = *(undefined8 *)(param_1 + 0x10);
      *(undefined8 *)((long)&(__return_storage_ptr__->field1_0x4).field0_0x0 + 0xc) =
           *(undefined8 *)(param_1 + 0x18);
      *puVar1 = uVar2;
      *(undefined8 *)((long)&(__return_storage_ptr__->field1_0x4).field0_0x0 + 0x14) =
           *(undefined8 *)(param_1 + 0x20);
    }
    *(undefined1 *)&__return_storage_ptr__[1].field0_0x0 = 1;
  }
  return;
}


