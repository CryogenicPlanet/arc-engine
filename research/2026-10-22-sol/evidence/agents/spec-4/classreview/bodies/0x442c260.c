// ==== 0x442c260  ArcAIInferenceModel::mutableAIInferenceModel  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void * ArcAIInferenceModel::mutableAIInferenceModel(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return (void *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}


