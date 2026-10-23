// ==== 0x18f3c  FUN_00018f3c  (1084 bytes)

ulong FUN_00018f3c(ulong *param_1,ulong param_2)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  ulong *puVar4;
  ulong uVar5;
  ulong uVar6;
  ulong uVar7;
  long lVar8;
  ulong uVar9;
  ulong uVar10;
  long lVar11;
  ulong uVar12;
  long lVar13;
  ulong uVar14;
  long lVar15;
  
  if (param_2 < 0x21) {
    if (param_2 < 0x11) {
      if (8 < param_2) {
        uVar6 = *(ulong *)((long)param_1 + (param_2 - 8));
        uVar5 = uVar6 + param_2;
        uVar7 = uVar5 >> (param_2 & 0x3f) | uVar5 << 0x40 - (param_2 & 0x3f);
        uVar5 = (uVar7 ^ *param_1) * -0x622015f714c7d297;
        uVar5 = (uVar7 ^ uVar5 >> 0x2f ^ uVar5) * -0x622015f714c7d297;
        return (uVar5 ^ uVar5 >> 0x2f) * -0x622015f714c7d297 ^ uVar6;
      }
      if (param_2 < 4) {
        if (param_2 == 0) {
          return 0x9ae16a3b2f90404f;
        }
        uVar5 = (param_2 | (ulong)*(byte *)((long)param_1 + (param_2 - 1)) << 2) *
                -0x36b62838af619aa9 ^
                (ulong)CONCAT11(*(undefined1 *)((long)param_1 + (param_2 >> 1)),(char)*param_1) *
                -0x651e95c4d06fbfb1;
        return (uVar5 ^ uVar5 >> 0x2f) * -0x651e95c4d06fbfb1;
      }
      uVar6 = (ulong)*(uint *)((long)param_1 + (param_2 - 4));
      uVar5 = (param_2 + (ulong)(uint)*param_1 * 8 ^ uVar6) * -0x622015f714c7d297;
      uVar5 = uVar6 ^ uVar5 >> 0x2f ^ uVar5;
    }
    else {
      lVar13 = *(long *)((long)param_1 + (param_2 - 8));
      uVar7 = lVar13 * -0x651e95c4d06fbfb1;
      uVar12 = *param_1 * -0x4b6d499041670d8d - param_1[1];
      uVar5 = param_1[1] ^ 0xc949d7c7509e6557;
      uVar6 = *param_1 * -0x4b6d499041670d8d + param_2 + (uVar5 >> 0x14 | uVar5 << 0x2c) +
              lVar13 * 0x651e95c4d06fbfb1;
      uVar5 = ((uVar7 >> 0x1e | uVar7 << 0x22) + (uVar12 >> 0x2b | uVar12 * 0x200000) +
               *(long *)((long)param_1 + (param_2 - 0x10)) * -0x3c5a37a36834ced9 ^ uVar6) *
              -0x622015f714c7d297;
      uVar5 = uVar6 ^ uVar5 >> 0x2f ^ uVar5;
    }
    lVar8 = -0x622015f714c7d297;
    uVar5 = uVar5 * -0x622015f714c7d297;
  }
  else {
    if (0x40 < param_2) {
      lVar11 = *(long *)((long)param_1 + (param_2 - 0x38));
      lVar15 = *(long *)((long)param_1 + (param_2 - 0x10));
      lVar1 = *(long *)((long)param_1 + (param_2 - 8));
      uVar5 = lVar11 + lVar15;
      lVar8 = *(long *)((long)param_1 + (param_2 - 0x30));
      lVar2 = *(long *)((long)param_1 + (param_2 - 0x28));
      uVar3 = *(ulong *)((long)param_1 + (param_2 - 0x18));
      uVar6 = (lVar8 + param_2 ^ uVar3) * -0x622015f714c7d297;
      uVar6 = (uVar3 ^ uVar6 >> 0x2f ^ uVar6) * -0x622015f714c7d297;
      uVar10 = (uVar6 ^ uVar6 >> 0x2f) * -0x622015f714c7d297;
      lVar13 = *(long *)((long)param_1 + (param_2 - 0x40)) + param_2;
      uVar6 = lVar13 + lVar2 + uVar10;
      uVar7 = lVar8 + lVar11 + lVar13;
      uVar12 = uVar7 + lVar2;
      uVar6 = (uVar7 >> 0x2c | uVar7 * 0x100000) + lVar13 + (uVar6 >> 0x15 | uVar6 << 0x2b);
      lVar13 = uVar5 + *(long *)((long)param_1 + (param_2 - 0x20)) + -0x4b6d499041670d8d;
      uVar7 = lVar13 + lVar2 + lVar1;
      uVar3 = uVar3 + lVar15 + lVar13;
      uVar14 = uVar3 + lVar1;
      uVar7 = (uVar3 >> 0x2c | uVar3 * 0x100000) + lVar13 + (uVar7 >> 0x15 | uVar7 << 0x2b);
      puVar4 = param_1 + 4;
      lVar15 = *param_1 + lVar2 * -0x4b6d499041670d8d;
      lVar13 = -(param_2 - 1 & 0xffffffffffffffc0);
      do {
        uVar3 = uVar5 + lVar15 + uVar12 + puVar4[-3];
        uVar5 = uVar6 + uVar5 + puVar4[2];
        uVar9 = uVar14 + uVar10;
        uVar10 = (uVar3 >> 0x25 | uVar3 * 0x8000000) * -0x4b6d499041670d8d ^ uVar7;
        uVar5 = puVar4[1] + uVar12 + (uVar5 >> 0x2a | uVar5 * 0x400000) * -0x4b6d499041670d8d;
        lVar11 = puVar4[-4] + uVar6 * -0x4b6d499041670d8d;
        uVar6 = uVar10 + uVar14 + lVar11 + puVar4[-1];
        lVar15 = (uVar9 >> 0x21 | uVar9 * 0x80000000) * -0x4b6d499041670d8d;
        uVar3 = lVar11 + puVar4[-3] + puVar4[-2];
        lVar8 = lVar15 + uVar7 + *puVar4;
        uVar7 = uVar5 + puVar4[-2] + lVar8 + puVar4[3];
        uVar12 = uVar3 + puVar4[-1];
        uVar9 = puVar4[1] + puVar4[2] + lVar8;
        uVar6 = (uVar3 >> 0x2c | uVar3 * 0x100000) + lVar11 + (uVar6 >> 0x15 | uVar6 << 0x2b);
        uVar14 = uVar9 + puVar4[3];
        uVar7 = (uVar9 >> 0x2c | uVar9 * 0x100000) + lVar8 + (uVar7 >> 0x15 | uVar7 << 0x2b);
        puVar4 = puVar4 + 8;
        lVar13 = lVar13 + 0x40;
      } while (lVar13 != 0);
      uVar12 = (uVar14 ^ uVar12) * -0x622015f714c7d297;
      uVar12 = (uVar14 ^ uVar12 >> 0x2f ^ uVar12) * -0x622015f714c7d297;
      uVar6 = (uVar7 ^ uVar6) * -0x622015f714c7d297;
      uVar6 = (uVar7 ^ uVar6 >> 0x2f ^ uVar6) * -0x622015f714c7d297;
      uVar6 = lVar15 + (uVar6 ^ uVar6 >> 0x2f) * -0x622015f714c7d297;
      uVar5 = (uVar6 ^ uVar10 + (uVar5 ^ uVar5 >> 0x2f) * -0x4b6d499041670d8d +
                       (uVar12 ^ uVar12 >> 0x2f) * -0x622015f714c7d297) * -0x622015f714c7d297;
      uVar5 = (uVar6 ^ uVar5 >> 0x2f ^ uVar5) * -0x622015f714c7d297;
      return (uVar5 ^ uVar5 >> 0x2f) * -0x622015f714c7d297;
    }
    lVar15 = *(long *)((long)param_1 + (param_2 - 0x10));
    uVar9 = *param_1 + (lVar15 + param_2) * -0x3c5a37a36834ced9;
    uVar10 = param_1[3];
    uVar5 = uVar9 + param_1[1];
    uVar6 = uVar5 + param_1[2];
    uVar7 = *(long *)((long)param_1 + (param_2 - 0x20)) + param_1[2];
    uVar12 = *(long *)((long)param_1 + (param_2 - 0x18)) + uVar7;
    lVar13 = *(long *)((long)param_1 + (param_2 - 8)) + uVar10;
    uVar3 = lVar13 + uVar7;
    uVar14 = uVar12 + lVar15;
    lVar8 = -0x651e95c4d06fbfb1;
    lVar15 = (uVar5 >> 7 | uVar5 << 0x39) + (uVar9 >> 0x25 | uVar9 * 0x8000000) +
             (uVar9 + uVar10 >> 0x34 | (uVar9 + uVar10) * 0x1000) + (uVar6 >> 0x1f | uVar6 << 0x21);
    uVar5 = (uVar14 + lVar13 + lVar15) * -0x3c5a37a36834ced9 +
            (uVar6 + uVar10 + (uVar7 >> 0x25 | uVar7 * 0x8000000) + (uVar12 >> 7 | uVar12 << 0x39) +
                     (uVar3 >> 0x34 | uVar3 * 0x1000) + (uVar14 >> 0x1f | uVar14 << 0x21)) *
            -0x651e95c4d06fbfb1;
    uVar5 = lVar15 + (uVar5 ^ uVar5 >> 0x2f) * -0x3c5a37a36834ced9;
  }
  return (uVar5 ^ uVar5 >> 0x2f) * lVar8;
}


