// ==== 0x4431c4c  ArcCommandBarRankingTrainingResult::command_bar_ranking_training_result  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingTrainingResult::command_bar_ranking_training_result
               (optional<arc::CommandBarRankingTrainingResult> *__return_storage_ptr__,ID param_1,
               SEL param_2)

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  __return_storage_ptr__[1].field0_0x0.field0 = '\0';
  if (*(char *)(param_1 + 0x78) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::CommandBarRankingTrainingResult> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4431c94);
      (*pcVar1)();
    }
    FUN_043dc67c(__return_storage_ptr__,param_1 + 8);
    __return_storage_ptr__[1].field0_0x0.field0 = '\x01';
  }
  return;
}


