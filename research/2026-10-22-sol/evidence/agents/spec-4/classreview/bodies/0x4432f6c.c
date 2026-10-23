// ==== 0x4432f6c  ArcCommandBarRankingInput::.cxx_destruct  (84 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingInput::_cxx_destruct(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  if ((*(char *)(param_1 + 0x20) != '\0') && (lVar2 = *(long *)(param_1 + 8), lVar2 != 0)) {
    lVar4 = *(long *)(param_1 + 0x10) * 4;
    if (lVar4 != 0) {
      lVar3 = *(long *)(param_1 + 0x10) * -4;
      lVar4 = lVar4 + lVar2;
      do {
        lVar4 = lVar4 + -4;
        if (lVar4 == 0) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x4432fc0);
          (*pcVar1)();
        }
        lVar3 = lVar3 + 4;
      } while (lVar3 != 0);
    }
    *(undefined8 *)(param_1 + 0x10) = 0;
    FUN_00005810(lVar2,*(long *)(param_1 + 0x18) << 2);
    return;
  }
  return;
}


