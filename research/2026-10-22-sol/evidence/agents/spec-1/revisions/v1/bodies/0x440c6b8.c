// ==== 0x440c6b8  ArcTracingConfiguration::tracingConfiguration  (88 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcTracingConfiguration::tracingConfiguration
               (TracingConfiguration *__return_storage_ptr__,ID param_1,SEL param_2)

{
  char *pcVar1;
  
  if (*(char *)(param_1 + 0x2f) < '\0') {
    FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 0x18),
                 *(undefined8 *)(param_1 + 0x20));
  }
  else {
    pcVar1 = *(char **)(param_1 + 0x18);
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field1_0x8 =
         *(unsigned_long_long *)(param_1 + 0x20);
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field0_0x0 = pcVar1;
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field2_0x10 =
         *(AnonymousBitField1 *)(param_1 + 0x28);
  }
  __return_storage_ptr__->field1_0x18 = *(double *)(param_1 + 0x30);
  *(undefined1 *)&__return_storage_ptr__->field2_0x20 = *(undefined1 *)(param_1 + 0x38);
  return;
}


