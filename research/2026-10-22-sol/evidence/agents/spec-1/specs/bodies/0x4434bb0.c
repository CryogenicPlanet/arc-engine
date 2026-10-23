// ==== 0x4434bb0  ArcCommandBarRankingCandidate::command_bar_ranking_candidate  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingCandidate::command_bar_ranking_candidate
               (optional<arc::CommandBarRankingCandidate> *__return_storage_ptr__,ID param_1,
               SEL param_2)

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)((long)&__return_storage_ptr__->field0_0x0 + 0xa8) = 0;
  if (*(char *)(param_1 + 0xb0) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::CommandBarRankingCandidate> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4434bf8);
      (*pcVar1)();
    }
    FUN_042a3ea8(__return_storage_ptr__,param_1 + 8);
    *(undefined1 *)((long)&__return_storage_ptr__->field0_0x0 + 0xa8) = 1;
  }
  return;
}


