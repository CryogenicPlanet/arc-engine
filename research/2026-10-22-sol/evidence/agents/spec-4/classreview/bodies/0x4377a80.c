// ==== 0x4377a80  FUN_04377a80  (736 bytes)

/* WARNING: Removing unreachable block (ram,0x04377cf8) */
/* WARNING: Removing unreachable block (ram,0x04377c44) */
/* WARNING: Removing unreachable block (ram,0x04377ce4) */
/* WARNING: Removing unreachable block (ram,0x04377d0c) */

void FUN_04377a80(undefined8 *param_1,long param_2)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  undefined4 *puVar4;
  long lVar5;
  int *piVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  int *piVar9;
  undefined1 local_280 [8];
  undefined8 local_278;
  undefined7 local_268;
  char cStack_261;
  undefined8 local_260;
  undefined7 local_250;
  char cStack_249;
  undefined8 local_228;
  undefined7 local_218;
  char cStack_211;
  undefined8 local_210;
  undefined7 local_200;
  char cStack_1f9;
  undefined8 local_1d8;
  undefined7 local_1c8;
  char cStack_1c1;
  undefined8 local_1c0;
  undefined7 local_1b0;
  char cStack_1a9;
  char local_170;
  undefined1 local_168 [8];
  undefined8 local_160;
  undefined7 local_150;
  char cStack_149;
  undefined8 local_148;
  undefined7 local_138;
  char cStack_131;
  char local_f8;
  undefined8 *local_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined1 auStack_d8 [80];
  undefined1 local_88 [56];
  undefined1 local_50;
  undefined8 **local_48;
  
  lVar3 = *(long *)(param_2 + 0x130);
  if (lVar3 == 0) {
    uVar7 = FUN_04b37104(0x25,"managed webcontents is null",0x1b,0x73b,
                         "../../arc/browser/web_contents/web_contents_impl.cc");
  }
  else {
    lVar5 = *(long *)(lVar3 + 0x270);
    piVar6 = *(int **)(lVar5 + 0x20);
    if (piVar6 != (int *)0x0) {
      iVar2 = *piVar6;
      *piVar6 = iVar2 + 1;
      if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x4377cd0);
        (*pcVar1)();
      }
      lVar3 = *(long *)(param_2 + 0x130);
    }
    uVar7 = *(undefined8 *)(lVar5 + 0x28);
    local_88[0] = 0;
    local_50 = 0;
    FUN_010071c8(lVar3);
    FUN_004b7f9c(auStack_d8);
    local_f0 = &local_e8;
    local_e8 = 0;
    uStack_e0 = 0;
    local_168[0] = 0;
    local_f8 = '\0';
    local_280[0] = 0;
    local_170 = '\0';
    piVar9 = *(int **)(param_2 + 0x610);
    if ((piVar9 != (int *)0x0) && (iVar2 = *piVar9, *piVar9 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x4377cdc);
      (*pcVar1)();
    }
    uVar8 = *(undefined8 *)(param_2 + 0x618);
    puVar4 = (undefined4 *)FUN_00008790(0x40);
    *puVar4 = 1;
    *(undefined1 **)(puVar4 + 2) = &LAB_040c5ad0;
    *(code **)(puVar4 + 4) = FUN_040a6300;
    *(undefined1 **)(puVar4 + 6) = &LAB_04169bc0;
    *(undefined1 **)(puVar4 + 8) = &LAB_04374f68;
    *(undefined8 *)(puVar4 + 10) = 0;
    *(int **)(puVar4 + 0xc) = piVar9;
    *(undefined8 *)(puVar4 + 0xe) = uVar8;
    FUN_06397b40(piVar6,uVar7,local_88,auStack_d8,0x3f,&local_f0,0,local_168,local_280,0,puVar4);
    if (local_170 != '\0') {
      if (cStack_1a9 < '\0') {
        FUN_00005810(local_1c0,CONCAT17(cStack_1a9,local_1b0) & 0x7fffffffffffffff);
      }
      if (cStack_1c1 < '\0') {
        FUN_00005810(local_1d8,CONCAT17(cStack_1c1,local_1c8) & 0x7fffffffffffffff);
      }
      if (cStack_1f9 < '\0') {
        FUN_00005810(local_210,CONCAT17(cStack_1f9,local_200) & 0x7fffffffffffffff);
      }
      if (cStack_211 < '\0') {
        FUN_00005810(local_228,CONCAT17(cStack_211,local_218) & 0x7fffffffffffffff);
      }
      if (cStack_249 < '\0') {
        FUN_00005810(local_260,CONCAT17(cStack_249,local_250) & 0x7fffffffffffffff);
      }
      if (cStack_261 < '\0') {
        FUN_00005810(local_278,CONCAT17(cStack_261,local_268) & 0x7fffffffffffffff);
      }
    }
    if (local_f8 != '\0') {
      if (cStack_131 < '\0') {
        FUN_00005810(local_148,CONCAT17(cStack_131,local_138) & 0x7fffffffffffffff);
      }
      if (cStack_149 < '\0') {
        FUN_00005810(local_160,CONCAT17(cStack_149,local_150) & 0x7fffffffffffffff);
      }
    }
    local_48 = &local_f0;
    FUN_0063242c(&local_48,local_e8);
    uVar7 = 1;
  }
  *param_1 = uVar7;
  return;
}


