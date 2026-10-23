// ==== 0x43d38cc  FUN_043d38cc  (764 bytes)

/* WARNING: Removing unreachable block (ram,0x043d3b1c) */

void FUN_043d38cc(long param_1,undefined8 *param_2,undefined8 *param_3)

{
  undefined8 *puVar1;
  size_t sVar2;
  byte bVar3;
  byte bVar4;
  size_t sVar5;
  code *pcVar6;
  int iVar7;
  undefined8 *puVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  undefined8 *puVar12;
  long lVar13;
  undefined8 local_c8 [2];
  undefined7 local_b8;
  char cStack_b1;
  byte local_b0;
  undefined1 auStack_a8 [24];
  undefined8 local_90 [2];
  undefined7 local_80;
  char cStack_79;
  byte local_78;
  undefined1 auStack_70 [31];
  undefined1 uStack_51;
  
  lVar10 = *(long *)(param_1 + 0x38) * 0x18;
  if (lVar10 != 0) {
    puVar12 = *(undefined8 **)(param_1 + 0x30);
    sVar5 = param_2[1];
    puVar8 = (void *)*param_2;
    if (-1 < (char)*(byte *)((long)param_2 + 0x17)) {
      sVar5 = (ulong)*(byte *)((long)param_2 + 0x17);
      puVar8 = param_2;
    }
    do {
      bVar3 = *(byte *)((long)puVar12 + 0x17);
      sVar2 = puVar12[1];
      if (-1 < (char)bVar3) {
        sVar2 = (ulong)bVar3;
      }
      if (sVar2 == sVar5) {
        puVar1 = (void *)*puVar12;
        if (-1 < (char)bVar3) {
          puVar1 = puVar12;
        }
        iVar7 = _memcmp(puVar1,puVar8,sVar5);
        if (iVar7 == 0) {
          FUN_0681222c(param_1);
          lVar10 = (long)*(char *)((long)param_2 + 0x17);
          puVar12 = param_2;
          if (lVar10 < 0) {
            lVar10 = param_2[1];
            puVar12 = (undefined8 *)*param_2;
          }
          FUN_00c87c40(local_90,puVar12,lVar10);
          FUN_021beb38(auStack_70,local_90);
          lVar10 = (long)*(char *)((long)param_3 + 0x17);
          puVar12 = param_3;
          if (lVar10 < 0) {
            puVar12 = (undefined8 *)*param_3;
            lVar10 = param_3[1];
          }
          FUN_00c87c40(local_c8,puVar12,lVar10);
          FUN_021beb38(auStack_a8,local_c8);
          FUN_06812404(param_1,auStack_70,auStack_a8);
          if ((local_b0 != 0xff) && ((local_b0 & 0xfd) != 0)) {
            if (local_b0 == 4) {
              if (cStack_b1 < '\0') {
                FUN_00005810(local_c8[0],CONCAT17(cStack_b1,local_b8) & 0x7fffffffffffffff);
              }
            }
            else {
              (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_b0])(&uStack_51,local_c8);
            }
          }
          if ((local_78 != 0xff) && ((local_78 & 0xfd) != 0)) {
            if (local_78 == 4) {
              if (cStack_79 < '\0') {
                FUN_00005810(local_90[0],CONCAT17(cStack_79,local_80) & 0x7fffffffffffffff);
              }
            }
            else {
              (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_78])(local_c8,local_90);
            }
          }
          lVar10 = *(long *)(param_1 + 0x38);
          lVar11 = lVar10 * 0x18;
          if (lVar11 == 0) goto LAB_043d3bc4;
          lVar13 = 0;
          lVar9 = *(long *)(param_1 + 0x30);
          bVar3 = *(byte *)((long)param_2 + 0x17);
          sVar5 = param_2[1];
          if (-1 < (char)bVar3) {
            sVar5 = (ulong)bVar3;
          }
          goto LAB_043d3a38;
        }
      }
      puVar12 = puVar12 + 3;
      lVar10 = lVar10 + -0x18;
    } while (lVar10 != 0);
  }
  return;
LAB_043d3a38:
  puVar12 = (undefined8 *)(lVar9 + lVar13);
  bVar4 = *(byte *)((long)puVar12 + 0x17);
  sVar2 = puVar12[1];
  if (-1 < (char)bVar4) {
    sVar2 = (ulong)bVar4;
  }
  if (sVar2 == sVar5) {
    puVar8 = (void *)*puVar12;
    if (-1 < (char)bVar4) {
      puVar8 = puVar12;
    }
    puVar12 = (undefined8 *)*param_2;
    if (-1 < (char)bVar3) {
      puVar12 = param_2;
    }
    iVar7 = _memcmp(puVar8,puVar12,sVar5);
    if (iVar7 == 0) {
      puVar12 = (undefined8 *)(lVar9 + lVar13);
      if (lVar13 + 0x18 != lVar11) {
        lVar10 = (lVar11 - lVar13) + -0x18;
        do {
          if (*(char *)((long)puVar12 + 0x17) < '\0') {
            FUN_00005810(*puVar12,puVar12[2] & 0x7fffffffffffffff);
          }
          puVar12[1] = puVar12[4];
          *puVar12 = puVar12[3];
          puVar12[2] = puVar12[5];
          *(undefined1 *)((long)puVar12 + 0x2f) = 0;
          puVar12 = puVar12 + 3;
          *(undefined1 *)puVar12 = 0;
          lVar10 = lVar10 + -0x18;
        } while (lVar10 != 0);
        lVar9 = *(long *)(param_1 + 0x30);
        lVar10 = *(long *)(param_1 + 0x38);
      }
      puVar8 = (undefined8 *)(lVar9 + lVar10 * 0x18);
      if (puVar12 != puVar8) {
        do {
          puVar8 = puVar8 + -3;
          if (puVar8 == (undefined8 *)0x0) {
LAB_043d3bc4:
                    /* WARNING: Does not return */
            pcVar6 = (code *)SoftwareBreakpoint(1,0x43d3bc8);
            (*pcVar6)();
          }
        } while (puVar8 != puVar12);
        lVar9 = *(long *)(param_1 + 0x30);
      }
      *(long *)(param_1 + 0x38) = ((long)puVar12 - lVar9 >> 3) * -0x5555555555555555;
      return;
    }
  }
  lVar13 = lVar13 + 0x18;
  if (lVar11 - lVar13 == 0) goto LAB_043d3bc4;
  goto LAB_043d3a38;
}


