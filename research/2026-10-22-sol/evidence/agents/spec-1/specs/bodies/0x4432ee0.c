// ==== 0x4432ee0  ArcCommandBarRankingInput::command_bar_ranking_input  (140 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingInput::command_bar_ranking_input
               (optional<arc::CommandBarRankingInput> *__return_storage_ptr__,ID param_1,SEL param_2
               )

{
  code *pcVar1;
  float *pfVar2;
  void *pvVar3;
  ulong uVar4;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x18 = 0;
  if (*(char *)(param_1 + 0x20) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::CommandBarRankingInput> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4432f68);
      (*pcVar1)();
    }
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field0_0x0 = (float *)0x0;
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field1_0x8 = 0;
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field2_0x10 = 0;
    uVar4 = *(ulong *)(param_1 + 0x10);
    if (uVar4 != 0) {
      if (uVar4 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
        FUN_04047380();
      }
      pvVar3 = *(void **)(param_1 + 8);
      pfVar2 = (float *)FUN_00008790(uVar4 << 2);
      (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field0_0x0 = pfVar2;
      (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field2_0x10 = uVar4;
      _memcpy(pfVar2,pvVar3,uVar4 << 2);
      (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field1_0x8 =
           (long)(uVar4 << 2) >> 2;
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x18 = 1;
  }
  return;
}


