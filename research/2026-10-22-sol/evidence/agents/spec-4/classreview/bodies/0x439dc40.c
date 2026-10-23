// ==== 0x439dc40  FUN_0439dc40  (552 bytes)

undefined8 FUN_0439dc40(long param_1,int *param_2,undefined8 param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  byte bVar4;
  code *pcVar5;
  undefined4 uVar6;
  int *piVar7;
  long lVar8;
  undefined8 uVar9;
  long *plVar10;
  long lVar11;
  undefined8 uVar12;
  undefined **local_110;
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
  char cStack_78;
  undefined7 uStack_77;
  char cStack_70;
  undefined8 uStack_6f;
  undefined8 local_60;
  char cStack_58;
  undefined7 uStack_57;
  char cStack_50;
  undefined7 uStack_4f;
  byte local_48;
  undefined7 uStack_47;
  undefined1 auStack_38 [8];
  
  if (*(long *)(param_1 + 0x130) == 0) {
    return 0;
  }
  cStack_58 = '\0';
  uStack_57 = 0;
  local_60 = 0;
  local_48 = 0;
  uStack_47 = 0;
  cStack_50 = '\0';
  uStack_4f = 0;
  FUN_09273058(&local_60,param_3);
  piVar7 = (int *)FUN_00d7e658();
  if ((int)local_60 == *piVar7) {
    bVar4 = *(byte *)(piVar7 + 6);
    if ((bVar4 & local_48) == 0) {
      if (local_48 == bVar4) {
        return 0;
      }
    }
    else {
      if ((bVar4 & 1) == 0) {
                    /* WARNING: Does not return */
        pcVar5 = (code *)SoftwareBreakpoint(1,0x439de68);
        (*pcVar5)();
      }
      uVar12 = *(undefined8 *)(piVar7 + 4);
      uVar9 = *(undefined8 *)(piVar7 + 2);
      if (CONCAT17(~-((char)((ulong)uVar9 >> 0x38) == (char)((uint7)uStack_57 >> 0x30)),
                   CONCAT16(~-((char)((ulong)uVar9 >> 0x30) == (char)((uint7)uStack_57 >> 0x28)),
                            CONCAT15(~-((char)((ulong)uVar9 >> 0x28) ==
                                       (char)((uint7)uStack_57 >> 0x20)),
                                     CONCAT14(~-((char)((ulong)uVar9 >> 0x20) ==
                                                (char)((uint7)uStack_57 >> 0x18)),
                                              CONCAT13(~-((char)((ulong)uVar9 >> 0x18) ==
                                                         (char)((uint7)uStack_57 >> 0x10)),
                                                       CONCAT12(~-((char)((ulong)uVar9 >> 0x10) ==
                                                                  (char)((uint7)uStack_57 >> 8)),
                                                                CONCAT11(~-((char)((ulong)uVar9 >> 8
                                                                                  ) ==
                                                                           (char)uStack_57),
                                                                         ~-((char)uVar9 == cStack_58
                                                                           )))))))) +
          CONCAT17(~-((char)((ulong)uVar12 >> 0x38) == (char)((uint7)uStack_4f >> 0x30)),
                   CONCAT16(~-((char)((ulong)uVar12 >> 0x30) == (char)((uint7)uStack_4f >> 0x28)),
                            CONCAT15(~-((char)((ulong)uVar12 >> 0x28) ==
                                       (char)((uint7)uStack_4f >> 0x20)),
                                     CONCAT14(~-((char)((ulong)uVar12 >> 0x20) ==
                                                (char)((uint7)uStack_4f >> 0x18)),
                                              CONCAT13(~-((char)((ulong)uVar12 >> 0x18) ==
                                                         (char)((uint7)uStack_4f >> 0x10)),
                                                       CONCAT12(~-((char)((ulong)uVar12 >> 0x10) ==
                                                                  (char)((uint7)uStack_4f >> 8)),
                                                                CONCAT11(~-((char)((ulong)uVar12 >>
                                                                                  8) ==
                                                                           (char)uStack_4f),
                                                                         ~-((char)uVar12 ==
                                                                           cStack_50)))))))) == 0) {
        return 0;
      }
    }
  }
  cStack_78 = cStack_58;
  local_80 = local_60;
  uStack_6f = CONCAT17(local_48,uStack_4f);
  uStack_77 = uStack_57;
  cStack_70 = cStack_50;
  lVar8 = FUN_06675f30(&local_80);
  if (lVar8 == 0) {
    return 0;
  }
  do {
    if (*(long *)(lVar8 + 0x178) == 0) {
      lVar11 = *(long *)(*(long *)(lVar8 + 0x110) + 0x38);
      if (lVar11 == 0) {
        FUN_0837a430(&local_110,"../../content/browser/renderer_host/render_frame_host_impl.cc",
                     "rsions/A/CoreMedia");
        FUN_08376274(auStack_38,&local_110);
        FUN_083762f0(auStack_38);
        return 0;
      }
      break;
    }
    lVar11 = *(long *)(lVar8 + 0x750);
    lVar8 = *(long *)(lVar8 + 0x178);
  } while (lVar11 == 0);
  iVar3 = *param_2;
  if (iVar3 == 4) {
    return 0;
  }
  uVar6 = 10;
  if (iVar3 != 3) {
    uVar6 = 0;
  }
  uVar2 = 9;
  if (iVar3 != 2) {
    uVar2 = uVar6;
  }
  uVar6 = 8;
  if (iVar3 != 1) {
    uVar6 = 0;
  }
  uVar1 = 7;
  if (iVar3 != 0) {
    uVar1 = uVar6;
  }
  if (iVar3 < 2) {
    uVar2 = uVar1;
  }
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
  local_110 = (undefined **)0x0;
  uStack_f8 = 0;
  uStack_100 = 0;
  uVar9 = (*(code *)PTR_FUN_0e2f1ea8)();
  FUN_095ceeb0(&local_110,uVar2,0,uVar9);
  plVar10 = (long *)(param_2 + 8);
  if (*(char *)((long)param_2 + 0x37) < '\0') {
    if (*(long *)(param_2 + 10) != 0) {
      plVar10 = (long *)*plVar10;
LAB_0439ddf8:
      uVar6 = FUN_08b7cbd8(plVar10);
      local_b0 = CONCAT44(local_b0._4_4_,uVar6);
    }
  }
  else if (*(char *)((long)param_2 + 0x37) != '\0') goto LAB_0439ddf8;
  plVar10 = (long *)(param_2 + 2);
  if (*(char *)((long)param_2 + 0x1f) < '\0') {
    if (*(long *)(param_2 + 4) == 0) goto LAB_0439de20;
    plVar10 = (long *)*plVar10;
  }
  else if (*(char *)((long)param_2 + 0x1f) == '\0') goto LAB_0439de20;
  uVar6 = FUN_08b7c698(plVar10);
  uStack_b8 = CONCAT44(uVar6,(undefined4)uStack_b8);
LAB_0439de20:
  if (param_2[0xe] != 0) {
    uStack_c0 = CONCAT44(param_2[0xe],(undefined4)uStack_c0);
  }
  local_90 = CONCAT71(local_90._1_7_,1);
  FUN_066a5b90(lVar11);
  FUN_066a61a0(lVar11,&local_110);
  local_110 = &PTR_FUN_0daf5640;
  FUN_03b5f36c(&uStack_98);
  return 1;
}


