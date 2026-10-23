// ==== 0x4432388  ArcCommandBarRankingTensor::command_bar_ranking_tensor  (64 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingTensor::command_bar_ranking_tensor
               (optional<arc::CommandBarRankingTensor> *__return_storage_ptr__,ID param_1,
               SEL param_2)

{
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x48 = 0;
  if (*(char *)(param_1 + 0x50) != '\0') {
    FUN_042a5760(__return_storage_ptr__,param_1 + 8);
    *(undefined1 *)&__return_storage_ptr__->field1_0x48 = 1;
  }
  return;
}


