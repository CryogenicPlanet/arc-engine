// ==== 0x4432be0  ArcCommandBarRankingInput::mutableCommandBarRankingInput  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void * ArcCommandBarRankingInput::mutableCommandBarRankingInput(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (void *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}


