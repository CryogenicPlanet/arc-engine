// ==== 0x4403830  ArcHistoryQueryOptions::setBeginTime:  (176 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcHistoryQueryOptions::setBeginTime_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  long lVar2;
  double extraout_d0;
  double dVar3;
  
  _objc_msgSend(param_3,"timeIntervalSince1970");
  lVar2 = 0x295e9648864000;
  if ((extraout_d0 != 0.0) && (lVar2 = 0, !NAN(extraout_d0))) {
    dVar3 = extraout_d0 * 1000000.0;
    lVar2 = 0;
    if (-9.223372036854776e+18 <= dVar3) {
      lVar2 = 0x7fffffffffffffff;
    }
    lVar1 = (long)dVar3;
    if (9.223372036854775e+18 < dVar3) {
      lVar1 = lVar2;
    }
    lVar2 = lVar1;
    if (1 < lVar1 + 0x8000000000000001U) {
      lVar2 = 0x7fffffffffffffff;
      if (!SCARRY8(lVar1,0x295e9648864000)) {
        lVar2 = lVar1 + 0x295e9648864000;
      }
    }
  }
  *(long *)(*(long *)(param_1 + 8) + 0x18) = lVar2;
  return;
}


