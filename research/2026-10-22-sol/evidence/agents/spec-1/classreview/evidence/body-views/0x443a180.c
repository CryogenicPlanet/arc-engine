// ==== 0x443a180  ArcAIInferenceModel::ai_inference_model  (112 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAIInferenceModel::ai_inference_model
               (optional<arc::AIInferenceModel> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  __return_storage_ptr__[1].field0_0x0.field0 = '\0';
  if (*(char *)(param_1 + 0x28) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::AIInferenceModel> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x443a21c);
      (*pcVar1)();
    }
    puVar2 = (undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 8);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0 = *(int *)(param_1 + 8);
    if (*(char *)(param_1 + 0x27) < '\0') {
      FUN_00060a60(puVar2,*(undefined8 *)(param_1 + 0x10),*(undefined8 *)(param_1 + 0x18));
    }
    else {
      uVar3 = *(undefined8 *)(param_1 + 0x10);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x10) =
           *(undefined8 *)(param_1 + 0x18);
      *puVar2 = uVar3;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x18) =
           *(undefined8 *)(param_1 + 0x20);
    }
    __return_storage_ptr__[1].field0_0x0.field0 = '\x01';
  }
  return;
}


