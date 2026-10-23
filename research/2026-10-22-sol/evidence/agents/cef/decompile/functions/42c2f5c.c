// ==== 0x42c2f5c  FUN_042c2f5c  (600 bytes)

undefined8 * FUN_042c2f5c(undefined8 *param_1,ulong param_2,uint param_3)

{
  uint uVar1;
  code *pcVar2;
  int iVar3;
  int *piVar4;
  undefined8 uVar5;
  ulong uVar6;
  long lVar7;
  undefined8 *puVar8;
  long lVar9;
  uint local_58 [2];
  undefined1 *local_50;
  undefined2 local_48;
  undefined1 local_46;
  
  *param_1 = &PTR_FUN_0da8a590;
  piVar4 = (int *)FUN_00008790(8);
  *piVar4 = 0;
  *(undefined1 *)(piVar4 + 1) = 0;
  if ((piVar4 != (int *)0x0) && (iVar3 = *piVar4, *piVar4 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(0,0x42c3190);
    (*pcVar2)();
  }
  param_1[1] = piVar4;
  param_1[2] = param_1;
  *param_1 = &PTR_LAB_0da7d958;
  puVar8 = param_1 + 3;
  *puVar8 = &PTR_FUN_0da8a590;
  piVar4 = (int *)FUN_00008790(8);
  *piVar4 = 0;
  *(undefined1 *)(piVar4 + 1) = 0;
  if ((piVar4 != (int *)0x0) && (iVar3 = *piVar4, *piVar4 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(0,0x42c319c);
    (*pcVar2)();
  }
  *param_1 = &PTR_FUN_0da7be30;
  param_1[3] = &PTR_FUN_0da7be80;
  param_1[4] = piVar4;
  param_1[5] = puVar8;
  param_1[6] = 0;
  param_1[9] = FUN_040490c0;
  param_1[10] = &DAT_0c4b3500;
  if (((ulong)param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  uVar6 = DAT_0e2f2208;
  param_1[0xb] = param_1;
  param_1[0xc] = 0;
  if (((ulong)puVar8 & 0xfffffffc00000000) == uVar6) {
    FUN_0000ffa8(puVar8);
  }
  param_1[0xd] = puVar8;
  param_1[0xe] = 0;
  if (param_2 != 0) {
    uVar1 = -(param_3 & 1) & 0x1000c | param_3 >> 1 & 1;
    uVar5 = FUN_00008790(0x78);
    FUN_012c5b90(uVar5,param_2,0);
    local_50 = &LAB_0016e360;
    local_48 = 0;
    local_46 = 1;
    lVar9 = DAT_0e4a5380;
    local_58[0] = uVar1;
    if (DAT_0e4a5380 == 0) {
      lVar9 = FUN_0445ac8c();
    }
    uVar6 = FUN_0005cdd0(lVar9,0x928,0);
    uVar5 = FUN_0287b240(uVar6,uVar5,local_58);
    lVar9 = param_1[6];
    param_1[6] = uVar5;
    if (lVar9 != 0) {
      FUN_02f80cb0(lVar9);
      lVar7 = DAT_0e4a5380;
      if (DAT_0e4a5380 == 0) {
        lVar7 = FUN_0445ac8c();
      }
      FUN_014f2ca0(lVar7,lVar9);
      uVar6 = param_1[6];
    }
    if ((uVar6 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(uVar6);
    }
    if ((param_1[0xc] & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    param_1[0xc] = uVar6;
    FUN_01d2cd80(uVar6 + 0x10,param_1[0xb]);
    if ((param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(param_2);
    }
    if ((param_1[0xe] & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    param_1[0xe] = param_2;
    FUN_00575e00(param_2 + 0x88,param_1[0xd]);
  }
  return param_1;
}


