// ==== 0x443a2fc  ArcAIInferenceConfiguration::mutableAIInferenceConfiguration  (28 bytes)

/* Function Stack Size: 0x10 bytes */

AIStreamedInferenceConfiguration *
ArcAIInferenceConfiguration::mutableAIInferenceConfiguration(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0x10) & 1) != 0) {
    return (AIStreamedInferenceConfiguration *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}


