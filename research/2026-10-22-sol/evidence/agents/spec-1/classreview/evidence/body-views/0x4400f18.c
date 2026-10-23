// ==== 0x4400f18  ArcAnnotatedVisit::.cxx_destruct  (52 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAnnotatedVisit::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  undefined8 uVar2;
  
  if (*(char *)(param_1 + 0x10) != '\0') {
    lVar1 = *(long *)(param_1 + 8);
    *(undefined8 *)(param_1 + 8) = 0;
    if (lVar1 != 0) {
      uVar2 = FUN_04336e98();
      FUN_00005810(uVar2,0x150);
      return;
    }
  }
  return;
}


