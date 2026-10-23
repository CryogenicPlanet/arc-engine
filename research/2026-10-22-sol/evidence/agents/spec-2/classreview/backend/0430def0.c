// ==== 0x430def0  FUN_0430def0  (204 bytes)

double FUN_0430def0(long param_1)

{
  long lVar1;
  long lVar2;
  double dVar3;
  
  lVar2 = (**(code **)(**(long **)(param_1 + 0x18) + 0xf8))();
  dVar3 = 0.0;
  if ((lVar2 != 0) && (lVar2 = (**(code **)(**(long **)(param_1 + 0x18) + 0x148))(), lVar2 != 0)) {
    if (lVar2 + 0x8000000000000001U < 2) {
      dVar3 = -INFINITY;
      if (-1 < lVar2) {
        dVar3 = INFINITY;
      }
    }
    else {
      lVar1 = -0x8000000000000000;
      if (!SCARRY8(lVar2,-0x295e9648864000)) {
        lVar1 = lVar2 + -0x295e9648864000;
      }
      if (lVar1 == -0x8000000000000000) {
        dVar3 = -INFINITY;
      }
      else {
        dVar3 = (double)lVar1 / 1000000.0;
      }
    }
  }
  return dVar3;
}


