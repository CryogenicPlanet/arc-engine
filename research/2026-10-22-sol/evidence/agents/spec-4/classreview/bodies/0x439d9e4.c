// ==== 0x439d9e4  FUN_0439d9e4  (512 bytes)

/* WARNING: Removing unreachable block (ram,0x0439db20) */

undefined8 FUN_0439d9e4(long param_1,undefined8 *param_2,undefined8 param_3)

{
  byte bVar1;
  code *pcVar2;
  undefined8 uVar3;
  int *piVar4;
  long lVar5;
  undefined8 *puVar6;
  long lVar7;
  long lVar8;
  undefined8 uVar9;
  long local_b8;
  long local_b0;
  long local_a8;
  undefined8 local_a0;
  char cStack_98;
  undefined7 uStack_97;
  char cStack_90;
  undefined8 uStack_8f;
  undefined8 local_80;
  char cStack_78;
  undefined7 uStack_77;
  char cStack_70;
  undefined7 uStack_6f;
  byte local_68;
  undefined7 uStack_67;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  long local_38;
  
  if (*(long *)(param_1 + 0x130) == 0) goto LAB_0439da00;
  cStack_78 = '\0';
  uStack_77 = 0;
  local_80 = 0;
  local_68 = 0;
  uStack_67 = 0;
  cStack_70 = '\0';
  uStack_6f = 0;
  FUN_09273058(&local_80,param_3);
  piVar4 = (int *)FUN_00d7e658();
  if ((int)local_80 == *piVar4) {
    bVar1 = *(byte *)(piVar4 + 6);
    if ((bVar1 & local_68) == 0) {
      if (local_68 != bVar1) goto LAB_0439da80;
    }
    else {
      if ((bVar1 & 1) == 0) {
LAB_0439db60:
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x439db64);
        (*pcVar2)();
      }
      uVar9 = *(undefined8 *)(piVar4 + 4);
      uVar3 = *(undefined8 *)(piVar4 + 2);
      if (CONCAT17(~-((char)((ulong)uVar3 >> 0x38) == (char)((uint7)uStack_77 >> 0x30)),
                   CONCAT16(~-((char)((ulong)uVar3 >> 0x30) == (char)((uint7)uStack_77 >> 0x28)),
                            CONCAT15(~-((char)((ulong)uVar3 >> 0x28) ==
                                       (char)((uint7)uStack_77 >> 0x20)),
                                     CONCAT14(~-((char)((ulong)uVar3 >> 0x20) ==
                                                (char)((uint7)uStack_77 >> 0x18)),
                                              CONCAT13(~-((char)((ulong)uVar3 >> 0x18) ==
                                                         (char)((uint7)uStack_77 >> 0x10)),
                                                       CONCAT12(~-((char)((ulong)uVar3 >> 0x10) ==
                                                                  (char)((uint7)uStack_77 >> 8)),
                                                                CONCAT11(~-((char)((ulong)uVar3 >> 8
                                                                                  ) ==
                                                                           (char)uStack_77),
                                                                         ~-((char)uVar3 == cStack_78
                                                                           )))))))) +
          CONCAT17(~-((char)((ulong)uVar9 >> 0x38) == (char)((uint7)uStack_6f >> 0x30)),
                   CONCAT16(~-((char)((ulong)uVar9 >> 0x30) == (char)((uint7)uStack_6f >> 0x28)),
                            CONCAT15(~-((char)((ulong)uVar9 >> 0x28) ==
                                       (char)((uint7)uStack_6f >> 0x20)),
                                     CONCAT14(~-((char)((ulong)uVar9 >> 0x20) ==
                                                (char)((uint7)uStack_6f >> 0x18)),
                                              CONCAT13(~-((char)((ulong)uVar9 >> 0x18) ==
                                                         (char)((uint7)uStack_6f >> 0x10)),
                                                       CONCAT12(~-((char)((ulong)uVar9 >> 0x10) ==
                                                                  (char)((uint7)uStack_6f >> 8)),
                                                                CONCAT11(~-((char)((ulong)uVar9 >> 8
                                                                                  ) ==
                                                                           (char)uStack_6f),
                                                                         ~-((char)uVar9 == cStack_70
                                                                           )))))))) != 0)
      goto LAB_0439da80;
    }
LAB_0439da00:
    uVar3 = 0;
  }
  else {
LAB_0439da80:
    cStack_98 = cStack_78;
    local_a0 = local_80;
    uStack_8f = CONCAT17(local_68,uStack_6f);
    uStack_97 = uStack_77;
    cStack_90 = cStack_70;
    lVar5 = FUN_06675f30(&local_a0);
    uVar3 = 0;
    if (lVar5 != 0) {
      do {
        if (*(long *)(lVar5 + 0x178) == 0) {
          lVar7 = *(long *)(*(long *)(lVar5 + 0x110) + 0x38);
          if (lVar7 == 0) {
            FUN_0837a430(&local_58,"../../content/browser/renderer_host/render_frame_host_impl.cc",
                         "rsions/A/CoreMedia");
            FUN_08376274(&local_b8,&local_58);
            FUN_083762f0(&local_b8);
            goto LAB_0439da00;
          }
          break;
        }
        lVar7 = *(long *)(lVar5 + 0x750);
        lVar5 = *(long *)(lVar5 + 0x178);
      } while (lVar7 == 0);
      local_58 = 0;
      uStack_50 = 0;
      local_48 = 0;
      lVar5 = (long)*(char *)((long)param_2 + 0x17);
      puVar6 = param_2;
      if (lVar5 < 0) {
        puVar6 = (undefined8 *)*param_2;
        lVar5 = param_2[1];
      }
      FUN_007cf8e0(&local_58,puVar6,lVar5);
      local_b8 = 0;
      local_b0 = 0;
      local_a8 = 0;
      local_38 = -1;
      FUN_066a8034(lVar7,&local_58,&local_b8,&local_38,0);
      lVar5 = local_b8;
      if (local_b8 != 0) {
        if (local_b0 * 0x58 != 0) {
          lVar8 = local_b0 * 0x58 + local_b8 + -0x20;
          lVar7 = local_b0 * -0x58;
          do {
            if (lVar8 == 0x38) goto LAB_0439db60;
            local_38 = lVar8;
            FUN_01a80e40(&local_38);
            lVar8 = lVar8 + -0x58;
            lVar7 = lVar7 + 0x58;
          } while (lVar7 != 0);
        }
        local_b0 = (lVar5 - local_b8 >> 3) * 0x2e8ba2e8ba2e8ba3;
        FUN_00005810(local_b8,local_a8 * 0x58);
      }
      uVar3 = 1;
    }
  }
  return uVar3;
}


