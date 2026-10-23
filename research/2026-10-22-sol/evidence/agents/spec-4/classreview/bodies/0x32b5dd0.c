// ==== 0x32b5dd0  FUN_032b5dd0  (196 bytes)

double FUN_032b5dd0(double *param_1)

{
  ulong uVar1;
  long lVar2;
  double dVar3;
  ulong uVar4;
  long lVar5;
  double dVar6;
  
  lVar5 = (*(code *)PTR_FUN_0e2f1ea8)();
  uVar4 = lVar5 - (long)param_1[3];
  uVar1 = (long)uVar4 >> 0x3f ^ 0x8000000000000000;
  if (!SBORROW8(lVar5,(long)param_1[3])) {
    uVar1 = uVar4;
  }
  dVar6 = *param_1 * (double)(uVar1 & ((long)uVar1 >> 0x3f ^ 0xffffffffffffffffU));
  lVar5 = 0;
  if (-9.223372036854776e+18 <= dVar6) {
    lVar5 = 0x7fffffffffffffff;
  }
  lVar2 = (long)dVar6;
  if (9.223372036854775e+18 < dVar6) {
    lVar2 = lVar5;
  }
  if (lVar2 != -0x8000000000000000) {
    if (lVar2 == 0x7fffffffffffffff) {
      return param_1[1];
    }
    dVar6 = (double)((long)param_1[2] + lVar2);
    dVar3 = -(double)((long)dVar6 >> 0x3f);
    if (!SCARRY8((long)param_1[2],lVar2)) {
      dVar3 = dVar6;
    }
    if (0 < (long)dVar3) {
      if ((long)dVar3 < (long)param_1[1]) {
        return dVar3;
      }
      return param_1[1];
    }
  }
  return 0.0;
}


