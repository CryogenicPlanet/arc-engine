// ==== 0x443a838  ArcAIDistilBertClassifier::ai_distil_bert_classifier  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAIDistilBertClassifier::ai_distil_bert_classifier
               (optional<arc::AIDistilBertClassifier> *__return_storage_ptr__,ID param_1,SEL param_2
               )

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 2) = 0;
  if (*(char *)(param_1 + 0x70) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::AIDistilBertClassifier> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x443a880);
      (*pcVar1)();
    }
    FUN_0443a9fc(__return_storage_ptr__,param_1 + 8);
    *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 2) = 1;
  }
  return;
}


