// ==== 0x42f2ca4  FUN_042f2ca4  (896 bytes)

void FUN_042f2ca4(undefined8 *param_1,long param_2,long *param_3,undefined8 param_4)

{
  ulong uVar1;
  byte bVar2;
  uint uVar3;
  code *pcVar4;
  bool bVar5;
  undefined8 uVar6;
  long lVar7;
  long lVar8;
  short ****ppppsVar9;
  short *psVar10;
  ulong uVar11;
  ulong *puVar12;
  ulong uVar13;
  ulong *puVar14;
  short ***local_b0;
  ulong uStack_a8;
  ulong local_a0;
  short ***local_90;
  ulong uStack_88;
  undefined8 local_80;
  ulong *local_78;
  ulong local_70;
  long local_68;
  
  if (*(long *)(param_2 + 0x70) == 0) {
    uVar6 = FUN_04b37104(0x25,"managed profile is null",0x17,0xa7,
                         "../../arc/browser/browser_context/preferences_impl.cc");
    *param_1 = uVar6;
  }
  else {
    uVar6 = FUN_01a9dba4();
    local_78 = (ulong *)0x0;
    local_70 = 0;
    local_68 = 0;
    FUN_0262c5e0(&local_78);
    if ((local_70 & 0x1fffffffffffffff) != 0) {
      puVar12 = local_78 + local_70;
      puVar14 = local_78;
      do {
        uVar13 = *puVar14;
        if ((uVar13 & 0xfffffffc00000000) == DAT_0e2f2208) {
          FUN_0000ffa8(uVar13);
        }
        lVar7 = FUN_014eb710(uVar13);
        lVar8 = *param_3;
        if (*(char *)(lVar8 + 0x3cf) < '\0') {
          FUN_00054420(&local_90,*(undefined8 *)(lVar8 + 0x3b8),*(undefined8 *)(lVar8 + 0x3c0));
        }
        else {
          local_80 = *(long *)(lVar8 + 0x3c8);
          uStack_88 = *(ulong *)(lVar8 + 0x3c0);
          local_90 = (short ***)*(short *****)(lVar8 + 0x3b8);
        }
        bVar2 = *(byte *)(lVar7 + 0x3cf);
        uVar11 = *(ulong *)(lVar7 + 0x3c0);
        if (-1 < (char)bVar2) {
          uVar11 = (ulong)bVar2;
        }
        uVar3 = (uint)(char)local_80._7_1_;
        uVar1 = uStack_88;
        if (-1 < (int)uVar3) {
          uVar1 = (ulong)local_80._7_1_;
        }
        if (uVar11 == uVar1) {
          if (uVar11 != 0) {
            ppppsVar9 = (short ****)local_90;
            if (-1 < (int)uVar3) {
              ppppsVar9 = &local_90;
            }
            psVar10 = *(short **)(lVar7 + 0x3b8);
            if (-1 < (char)bVar2) {
              psVar10 = (short *)(lVar7 + 0x3b8);
            }
            do {
              if (*(short *)ppppsVar9 != *psVar10) goto LAB_042f2d5c;
              uVar11 = uVar11 - 1;
              ppppsVar9 = (short ****)((long)ppppsVar9 + 2);
              psVar10 = psVar10 + 1;
            } while (uVar11 != 0);
          }
          lVar7 = FUN_014eb710(uVar13);
          lVar8 = *param_3;
          if (*(char *)(lVar8 + 999) < '\0') {
            FUN_00054420(&local_b0,*(undefined8 *)(lVar8 + 0x3d0),*(undefined8 *)(lVar8 + 0x3d8));
          }
          else {
            local_a0 = *(ulong *)(lVar8 + 0x3e0);
            uStack_a8 = *(ulong *)(lVar8 + 0x3d8);
            local_b0 = (short ***)*(short *****)(lVar8 + 0x3d0);
          }
          bVar2 = *(byte *)(lVar7 + 999);
          uVar11 = *(ulong *)(lVar7 + 0x3d8);
          if (-1 < (char)bVar2) {
            uVar11 = (ulong)bVar2;
          }
          uVar1 = uStack_a8;
          if (-1 < (long)local_a0) {
            uVar1 = local_a0 >> 0x38;
          }
          if (uVar11 == uVar1) {
            if (uVar11 == 0) {
              bVar5 = true;
            }
            else {
              ppppsVar9 = (short ****)local_b0;
              if (-1 < (long)local_a0) {
                ppppsVar9 = &local_b0;
              }
              psVar10 = *(short **)(lVar7 + 0x3d0);
              if (-1 < (char)bVar2) {
                psVar10 = (short *)(lVar7 + 0x3d0);
              }
              do {
                uVar11 = uVar11 - 1;
                bVar5 = *(short *)ppppsVar9 == *psVar10;
                if (!bVar5) break;
                ppppsVar9 = (short ****)((long)ppppsVar9 + 2);
                psVar10 = psVar10 + 1;
              } while (uVar11 != 0);
            }
          }
          else {
            bVar5 = false;
          }
          if ((long)local_a0 < 0) {
            FUN_00005810(local_b0,local_a0 << 1);
          }
          if (local_80 < 0) goto LAB_042f2d64;
        }
        else {
LAB_042f2d5c:
          bVar5 = false;
          if ((uVar3 >> 7 & 1) != 0) {
LAB_042f2d64:
            FUN_00005810(local_90,local_80 << 1);
          }
        }
        if (bVar5) {
          FUN_09ecacd0(uVar6,uVar13,param_4);
          *param_1 = 1;
          if ((uVar13 & 0xfffffffc00000000) == DAT_0e2f2208) {
            FUN_000270f0(uVar13);
          }
          goto LAB_042f2f38;
        }
        if ((uVar13 & 0xfffffffc00000000) == DAT_0e2f2208) {
          FUN_000270f0(uVar13);
        }
        puVar14 = puVar14 + 1;
      } while (puVar14 != puVar12);
    }
    uVar6 = FUN_04b37104(0xd,"Search engine cannot be set as default - not found",0x32,0xb7,
                         "../../arc/browser/browser_context/preferences_impl.cc");
    *param_1 = uVar6;
LAB_042f2f38:
    puVar14 = local_78;
    if (local_78 != (ulong *)0x0) {
      if ((local_70 & 0x1fffffffffffffff) != 0) {
        puVar12 = local_78 + local_70;
        lVar7 = local_70 * -8;
        do {
          puVar12 = puVar12 + -1;
          if (puVar12 == (ulong *)0x0) {
                    /* WARNING: Does not return */
            pcVar4 = (code *)SoftwareBreakpoint(1,0x42f3024);
            (*pcVar4)();
          }
          if ((*puVar12 & 0xfffffffc00000000) == DAT_0e2f2208) {
            FUN_000270f0();
          }
          *puVar12 = 0;
          lVar7 = lVar7 + 8;
        } while (lVar7 != 0);
      }
      local_70 = (long)puVar14 - (long)local_78 >> 3;
      FUN_00005810(local_78,local_68 << 3);
    }
  }
  return;
}


