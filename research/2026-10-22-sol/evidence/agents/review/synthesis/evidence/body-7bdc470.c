// ==== 0x7bdc470  FUN_07bdc470  (652 bytes)

/* WARNING: Removing unreachable block (ram,0x07bdc6a4) */
/* WARNING: Removing unreachable block (ram,0x07bdc6e4) */
/* WARNING: Removing unreachable block (ram,0x07bdc6c8) */
/* WARNING: Removing unreachable block (ram,0x07bdc6b4) */

void FUN_07bdc470(undefined8 param_1,undefined8 param_2,undefined1 param_3,undefined8 param_4,
                 undefined8 param_5)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  int *piVar8;
  undefined1 auStack_238 [32];
  undefined1 auStack_218 [200];
  undefined8 local_150;
  undefined8 uStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  
  lVar3 = FUN_01a29860(DAT_0e481618);
  uVar4 = FUN_01a298f8();
  local_80 = 0;
  uStack_78 = 0;
  local_70 = 0;
  do {
    FUN_07bdc704(&local_150,lVar3);
    uStack_78 = uStack_148;
    local_80 = local_150;
    local_70 = local_140;
    lVar5 = FUN_01a299e0(uVar4,&local_80);
  } while (lVar5 != 0);
  local_90 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  uStack_98 = 0;
  uStack_a0 = 0;
  uStack_c8 = 0;
  local_d0 = 0;
  uStack_b8 = 0;
  uStack_c0 = 0;
  uStack_e8 = 0;
  local_f0 = 0;
  uStack_d8 = 0;
  uStack_e0 = 0;
  uStack_108 = 0;
  local_110 = 0;
  uStack_f8 = 0;
  uStack_100 = 0;
  uStack_128 = 0;
  local_130 = 0;
  uStack_118 = 0;
  uStack_120 = 0;
  uStack_148 = 0;
  local_150 = 0;
  uStack_138 = 0;
  local_140 = 0;
  FUN_03b1676c(&local_150);
  FUN_01fef980(&local_150,&local_80);
  FUN_007ab5d4(&uStack_138,param_1);
  local_90._0_2_ = CONCAT11(param_3,param_3);
  uStack_e8 = param_2;
  FUN_04210bc0(auStack_218,&local_150);
  FUN_0321fb6c(uVar4,auStack_218);
  FUN_0321e91c(auStack_218);
  FUN_00029e40(auStack_238,"CreateMultiProfileAsync",
               "../../chrome/browser/profiles/profile_manager.cc",0x465);
  piVar8 = *(int **)(lVar3 + 0x1a8);
  if ((piVar8 != (int *)0x0) && (iVar2 = *piVar8, *piVar8 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x7bdc6fc);
    (*pcVar1)();
  }
  uVar4 = *(undefined8 *)(lVar3 + 0x1b0);
  puVar6 = (undefined4 *)FUN_00008790(0x68);
  *puVar6 = 1;
  *(undefined1 **)(puVar6 + 2) = &LAB_07bddab8;
  *(code **)(puVar6 + 4) = FUN_07bddaf0;
  *(undefined1 **)(puVar6 + 6) = &LAB_04d21600;
  *(code **)(puVar6 + 8) = FUN_07bdba94;
  *(undefined8 *)(puVar6 + 10) = 0;
  *(int **)(puVar6 + 0xc) = piVar8;
  *(undefined8 *)(puVar6 + 0xe) = uVar4;
  *(undefined8 *)(puVar6 + 0x12) = uStack_78;
  *(undefined8 *)(puVar6 + 0x10) = local_80;
  *(undefined8 *)(puVar6 + 0x14) = local_70;
  *(undefined8 *)(puVar6 + 0x16) = param_4;
  *(undefined8 *)(puVar6 + 0x18) = param_5;
  puVar7 = (undefined4 *)FUN_00008790(0x48);
  *puVar7 = 1;
  *(undefined1 **)(puVar7 + 2) = &LAB_07bddca0;
  *(code **)(puVar7 + 4) = FUN_07bddcb4;
  *(code **)(puVar7 + 6) = FUN_00b77080;
  *(undefined1 **)(puVar7 + 8) = &LAB_07be064c;
  *(undefined8 *)(puVar7 + 0xc) = uStack_78;
  *(undefined8 *)(puVar7 + 10) = local_80;
  *(undefined8 *)(puVar7 + 0xe) = local_70;
  *(undefined4 **)(puVar7 + 0x10) = puVar6;
  FUN_01c50294(auStack_238,&DAT_0c9bdd38,puVar7);
  FUN_0321e91c(&local_150);
  return;
}


