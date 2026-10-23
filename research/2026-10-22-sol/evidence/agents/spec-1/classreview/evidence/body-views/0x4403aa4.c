// ==== 0x4403aa4  ArcHistoryQueryOptions::setMatchingAlgorithm:  (44 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcHistoryQueryOptions::setMatchingAlgorithm_(ID param_1,SEL param_2,long_long param_3)

{
  undefined4 uVar1;
  long lVar2;
  
  if (param_3 == 0) {
    uVar1 = 0;
  }
  else {
    if (param_3 != 1) {
      return;
    }
    uVar1 = 1;
  }
  lVar2 = *(long *)(param_1 + 8);
  *(undefined4 *)(lVar2 + 0x34) = uVar1;
  *(undefined1 *)(lVar2 + 0x38) = 1;
  return;
}


