// ==== 0x42eaf60  FUN_042eaf60  (1012 bytes)

void FUN_042eaf60(long *param_1,long param_2)

{
  ulong uVar1;
  byte bVar2;
  long *plVar3;
  code *pcVar4;
  bool bVar5;
  int iVar6;
  undefined8 uVar7;
  long lVar8;
  undefined8 *puVar9;
  long lVar10;
  undefined8 ***pppuVar11;
  long lVar12;
  long *plVar13;
  long *plVar14;
  undefined8 *puVar15;
  undefined8 *puVar16;
  ulong local_130;
  ulong uStack_128;
  ulong local_120;
  char local_118;
  ulong local_108;
  ulong uStack_100;
  ulong local_f8;
  undefined8 **local_f0;
  undefined8 **ppuStack_e8;
  undefined8 **local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  long local_c8;
  ulong local_c0;
  ulong uStack_b8;
  ulong local_b0;
  ulong local_a8;
  ulong uStack_a0;
  ulong uStack_98;
  ulong local_90;
  undefined8 local_88;
  undefined8 *local_80;
  undefined8 local_78;
  undefined8 local_70;
  
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  lVar12 = *(long *)(param_2 + 0x68);
  if (lVar12 != 0) {
    uVar7 = FUN_027132a8(&DAT_0e49a548);
    lVar12 = FUN_0009278c(uVar7,lVar12,1);
    lVar8 = FUN_014baf1c(*(undefined8 *)(param_2 + 0x68));
    if (lVar12 != 0 && lVar8 != 0) {
      plVar13 = *(long **)(lVar8 + 8);
      if (plVar13 != (long *)(lVar8 + 0x10)) {
        do {
          iVar6 = FUN_06b38690(plVar13[7]);
          if (iVar6 != 0) {
            local_78 = 0;
            local_70 = 0;
            local_80 = &local_78;
            FUN_02d60f40(lVar12,*(undefined8 *)(plVar13[7] + 0x228),2,2,&local_80);
            puVar16 = local_80;
            while (puVar16 != &local_78) {
              uStack_98 = 0;
              uStack_a0 = 0;
              local_88 = 0;
              local_90 = 0;
              uStack_b8 = 0;
              local_c0 = 0;
              local_a8 = 0;
              local_b0 = 0;
              uStack_d8 = 0;
              local_e0 = (undefined8 **)0x0;
              local_c8 = 0;
              uStack_d0 = 0;
              ppuStack_e8 = (undefined8 **)0x0;
              local_f0 = (undefined8 **)0x0;
              pppuVar11 = *(undefined8 ****)(plVar13[7] + 0x228);
              if (&local_f0 != pppuVar11) {
                if (*(char *)((long)pppuVar11 + 0x17) < '\0') {
                  FUN_00359998(&local_f0,*pppuVar11,pppuVar11[1]);
                }
                else {
                  ppuStack_e8 = pppuVar11[1];
                  local_f0 = *pppuVar11;
                  local_e0 = pppuVar11[2];
                }
              }
              puVar9 = puVar16 + 4;
              if (&uStack_d8 != puVar9) {
                bVar2 = *(byte *)((long)puVar16 + 0x37);
                if (local_c8 < 0) {
                  uVar1 = puVar16[5];
                  puVar15 = (undefined8 *)puVar16[4];
                  if (-1 < (char)bVar2) {
                    uVar1 = (ulong)bVar2;
                    puVar15 = puVar9;
                  }
                  FUN_00359ad8(&uStack_d8,puVar15,uVar1);
                }
                else if ((char)bVar2 < '\0') {
                  FUN_00359998(&uStack_d8,puVar16[4],puVar16[5]);
                }
                else {
                  uStack_d0 = puVar16[5];
                  uStack_d8 = *puVar9;
                  local_c8 = puVar16[6];
                }
              }
              lVar10 = (long)*(char *)((long)puVar16 + 0x8f);
              if (lVar10 < 0) {
                puVar9 = (undefined8 *)puVar16[0xf];
                lVar10 = puVar16[0x10];
              }
              else {
                puVar9 = puVar16 + 0xf;
              }
              FUN_00af22f8(&local_130,puVar9,lVar10);
              if ((long)local_b0 < 0) {
                FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
              }
              uStack_b8 = uStack_128;
              local_c0 = local_130;
              local_b0 = local_120;
              local_108 = 0;
              uStack_100 = 0;
              local_f8 = 0;
              FUN_0329a3e0(&local_108,puVar16 + 0xb);
              uVar1 = uStack_100;
              if (-1 < (long)local_f8) {
                uVar1 = local_f8 >> 0x38;
              }
              if (uVar1 == 0) {
                local_118 = '\0';
                local_130 = local_130 & 0xffffffffffffff00;
              }
              else {
                if ((long)local_f8 < 0) {
                  FUN_00060a60(&local_130,local_108);
                }
                else {
                  uStack_128 = uStack_100;
                  local_130 = local_108;
                  local_120 = local_f8;
                }
                local_118 = '\x01';
              }
              if ((char)local_90 == local_118) {
                if ((char)local_90 == '\x01') {
                  if ((long)uStack_98 < 0) {
                    FUN_00005810(local_a8,uStack_98 & 0x7fffffffffffffff);
                  }
                  uStack_a0 = uStack_128;
                  local_a8 = local_130;
                  uStack_98 = local_120;
                  local_120 = local_120 & 0xffffffffffffff;
                  local_130 = local_130 & 0xffffffffffffff00;
                }
              }
              else if ((char)local_90 == '\x01') {
                if ((long)uStack_98 < 0) {
                  FUN_00005810(local_a8,uStack_98 & 0x7fffffffffffffff);
                }
                local_90 = local_90 & 0xffffffffffffff00;
              }
              else {
                uStack_a0 = uStack_128;
                local_a8 = local_130;
                uStack_98 = local_120;
                uStack_128 = 0;
                local_120 = 0;
                local_130 = 0;
                local_90 = CONCAT71(local_90._1_7_,1);
              }
              if ((local_118 != '\0') && ((long)local_120 < 0)) {
                FUN_00005810(local_130,local_120 & 0x7fffffffffffffff);
              }
              local_88 = CONCAT71(local_88._1_7_,*(undefined1 *)(puVar16 + 0x12));
              if (param_1[1] == param_1[2]) {
                lVar10 = FUN_042ec784(param_1,&local_f0);
              }
              else {
                lVar10 = *param_1 + param_1[1] * 0x70;
                if (lVar10 == 0) goto LAB_042eb2e8;
                lVar10 = FUN_0441e200(lVar10,&local_f0);
                lVar10 = lVar10 + 0x70;
              }
              param_1[1] = (lVar10 - *param_1 >> 4) * 0x6db6db6db6db6db7;
              if (lVar10 - *param_1 == 0) {
LAB_042eb2e8:
                    /* WARNING: Does not return */
                pcVar4 = (code *)SoftwareBreakpoint(1,0x42eb2ec);
                (*pcVar4)();
              }
              if ((long)local_f8 < 0) {
                FUN_00005810(local_108,local_f8 & 0x7fffffffffffffff);
              }
              FUN_0441e178(&local_f0);
              puVar9 = (undefined8 *)puVar16[1];
              puVar15 = puVar16;
              if ((undefined8 *)puVar16[1] == (undefined8 *)0x0) {
                do {
                  puVar16 = (undefined8 *)puVar15[2];
                  bVar5 = puVar15 != (undefined8 *)*puVar16;
                  puVar15 = puVar16;
                } while (bVar5);
              }
              else {
                do {
                  puVar16 = puVar9;
                  puVar9 = (undefined8 *)*puVar16;
                } while ((undefined8 *)*puVar16 != (undefined8 *)0x0);
              }
            }
            local_f0 = &local_80;
            FUN_02d61364(&local_f0,local_78);
          }
          plVar3 = (long *)plVar13[1];
          plVar14 = plVar13;
          if ((long *)plVar13[1] == (long *)0x0) {
            do {
              plVar13 = (long *)plVar14[2];
              bVar5 = plVar14 != (long *)*plVar13;
              plVar14 = plVar13;
            } while (bVar5);
          }
          else {
            do {
              plVar13 = plVar3;
              plVar3 = (long *)*plVar13;
            } while ((long *)*plVar13 != (long *)0x0);
          }
        } while (plVar13 != (long *)(lVar8 + 0x10));
      }
    }
  }
  return;
}


