// ==== 0x43e6374  ArcBrowserApplicationDebugProperty::property  (112 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplicationDebugProperty::property
               (DebugProperty *__return_storage_ptr__,ID param_1,SEL param_2)

{
  char *pcVar1;
  
  if (*(char *)(param_1 + 0x1f) < '\0') {
    FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 8),*(undefined8 *)(param_1 + 0x10)
                );
  }
  else {
    pcVar1 = *(char **)(param_1 + 8);
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field1_0x8 =
         *(unsigned_long_long *)(param_1 + 0x10);
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field0_0x0 = pcVar1;
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field2_0x10 =
         *(AnonymousBitField1 *)(param_1 + 0x18);
  }
  if (*(char *)(param_1 + 0x37) < '\0') {
    FUN_00060a60(&__return_storage_ptr__->field1_0x18,*(undefined8 *)(param_1 + 0x20),
                 *(undefined8 *)(param_1 + 0x28));
  }
  else {
    pcVar1 = *(char **)(param_1 + 0x20);
    (__return_storage_ptr__->field1_0x18).field0_0x0.field1.field1_0x8 =
         *(unsigned_long_long *)(param_1 + 0x28);
    (__return_storage_ptr__->field1_0x18).field0_0x0.field1.field0_0x0 = pcVar1;
    (__return_storage_ptr__->field1_0x18).field0_0x0.field1.field2_0x10 =
         *(AnonymousBitField1 *)(param_1 + 0x30);
  }
  return;
}


