// ==== 0x44328a4  ArcCommandBarRankingResult::command_bar_ranking_result  (112 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingResult::command_bar_ranking_result
               (optional<arc::CommandBarRankingResult> *__return_storage_ptr__,ID param_1,
               SEL param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  __return_storage_ptr__[1].field0_0x0.field0 = '\0';
  if (*(char *)(param_1 + 0x28) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::CommandBarRankingResult> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4432940);
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
    (__return_storage_ptr__->field0_0x0).field1.field1_0x18 = *(float *)(param_1 + 0x20);
    __return_storage_ptr__[1].field0_0x0.field0 = '\x01';
  }
  return;
}


