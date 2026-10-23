// ==== 0x440c320  ArcSpake2Context::.cxx_destruct  (44 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcSpake2Context::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = *(long *)(param_1 + 8);
  *(undefined8 *)(param_1 + 8) = 0;
  if (lVar1 != 0) {
    uVar2 = FUN_04341b9c();
    FUN_00005810(uVar2,8);
    return;
  }
  return;
}


