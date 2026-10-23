// ==== 0x43b6c18  FUN_043b6c18  (520 bytes)

/* WARNING: Removing unreachable block (ram,0x043b6db0) */

void FUN_043b6c18(long param_1,undefined8 *param_2,ulong param_3)

{
  size_t sVar1;
  size_t sVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  byte bVar5;
  byte bVar6;
  code *pcVar7;
  int iVar8;
  undefined8 uVar9;
  long lVar10;
  ulong uVar11;
  long lVar12;
  undefined8 *puVar13;
  undefined8 *puVar14;
  undefined8 *puVar15;
  long lVar16;
  undefined8 uVar17;
  
  lVar10 = *(long *)(param_1 + 0x68);
  if (lVar10 != 0) {
    uVar11 = *(ulong *)(lVar10 + 200);
    lVar12 = uVar11 * 0x18;
    if (uVar11 == 0) {
      lVar10 = 0;
      puVar15 = (undefined8 *)0x0;
      puVar14 = (undefined8 *)0x0;
    }
    else {
      if (0xaaaaaaaaaaaaaaa < uVar11) {
                    /* WARNING: Subroutine does not return */
        FUN_04047380();
      }
      lVar10 = *(long *)(lVar10 + 0xc0);
      puVar15 = (undefined8 *)FUN_00008790(lVar12);
      if (lVar12 == 0) {
        lVar10 = 0;
        puVar14 = puVar15;
      }
      else {
        lVar16 = 0;
        do {
          puVar14 = (undefined8 *)((long)puVar15 + lVar16);
          if (puVar14 == (undefined8 *)0x0) goto LAB_043b6e18;
          puVar13 = (undefined8 *)(lVar10 + lVar16);
          if (*(char *)((long)puVar13 + 0x17) < '\0') {
            puVar13 = (undefined8 *)(lVar10 + lVar16);
            FUN_00060a60(puVar14,*puVar13,puVar13[1]);
          }
          else {
            uVar17 = puVar13[1];
            uVar9 = *puVar13;
            puVar14[2] = puVar13[2];
            puVar14[1] = uVar17;
            *puVar14 = uVar9;
          }
          lVar16 = lVar16 + 0x18;
        } while (lVar12 - lVar16 != 0);
        puVar14 = (undefined8 *)((long)puVar15 + lVar16);
        if (lVar16 == 0) {
          lVar10 = 0;
        }
        else {
          lVar10 = (lVar16 >> 3) * -0x5555555555555555;
          bVar5 = *(byte *)((long)param_2 + 0x17);
          puVar13 = puVar15;
          sVar1 = param_2[1];
          if (-1 < (char)bVar5) {
            sVar1 = (ulong)bVar5;
          }
          do {
            bVar6 = *(byte *)((long)puVar13 + 0x17);
            sVar2 = puVar13[1];
            if (-1 < (char)bVar6) {
              sVar2 = (ulong)bVar6;
            }
            if (sVar2 == sVar1) {
              puVar3 = (void *)*puVar13;
              if (-1 < (char)bVar6) {
                puVar3 = puVar13;
              }
              puVar4 = (void *)*param_2;
              if (-1 < (char)bVar5) {
                puVar4 = param_2;
              }
              iVar8 = _memcmp(puVar3,puVar4,sVar1);
              if (iVar8 == 0) {
                if ((param_3 & 1) == 0) {
                  uVar9 = 0;
                  goto LAB_043b6c70;
                }
                if (puVar15 == (undefined8 *)0x0) {
                  return;
                }
                goto LAB_043b6dc8;
              }
            }
            lVar16 = lVar16 + -0x18;
            puVar13 = puVar13 + 3;
          } while (lVar16 != 0);
        }
      }
    }
    if ((param_3 & 1) != 0) {
      uVar9 = 1;
LAB_043b6c70:
      FUN_07cd94c0(*(undefined8 *)(param_1 + 0x68),param_2,uVar9);
    }
    if (puVar15 != (undefined8 *)0x0) {
      if (lVar10 != 0) {
LAB_043b6dc8:
        do {
          puVar14 = puVar14 + -3;
          if (puVar14 == (undefined8 *)0x0) {
LAB_043b6e18:
                    /* WARNING: Does not return */
            pcVar7 = (code *)SoftwareBreakpoint(1,0x43b6e1c);
            (*pcVar7)();
          }
        } while (puVar14 != puVar15);
      }
      FUN_00005810(puVar15,lVar12);
      return;
    }
  }
  return;
}


