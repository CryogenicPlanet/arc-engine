// ==== 0x43d06c0  FUN_043d06c0  (636 bytes)

void FUN_043d06c0(long param_1,undefined8 *param_2)

{
  byte bVar1;
  char cVar2;
  undefined8 *puVar3;
  code *pcVar4;
  long lVar5;
  ulong uVar6;
  long lVar7;
  ulong uVar8;
  long *plVar9;
  undefined8 *puVar10;
  long lVar11;
  undefined8 *puVar12;
  
  lVar11 = param_2[1] * 0x18;
  if (lVar11 != 0) {
    puVar12 = (undefined8 *)*param_2;
    do {
      bVar1 = *(byte *)((long)puVar12 + 0x17);
      uVar8 = (ulong)bVar1;
      if ((char)bVar1 < '\0') {
        if (puVar12[1] != 0) {
          uVar6 = puVar12[1];
          puVar10 = (undefined8 *)*puVar12;
          goto LAB_043d076c;
        }
      }
      else {
        uVar6 = uVar8;
        puVar10 = puVar12;
        if (bVar1 != 0) {
LAB_043d076c:
          if (*(char *)((long)puVar10 + (uVar6 - 1)) != '/') {
            if ((char)bVar1 < '\0') {
              uVar6 = puVar12[2] & 0x7fffffffffffffff;
              uVar8 = uVar6 - 1;
              if (puVar12[1] == uVar8) {
                if (0x7fffffffffffffee < uVar6) {
                    /* WARNING: Subroutine does not return */
                  FUN_04b089c0();
                }
                if (uVar6 <= uVar8 * 2) {
                  uVar6 = uVar8 * 2;
                }
                uVar6 = (uVar6 & 0xfffffffffffffff0) + 0x10;
                if (0x3fffffffffffffe7 < uVar8) {
                  uVar6 = 0x7ffffffffffffff0;
                }
LAB_043d08a8:
                puVar10 = (undefined8 *)FUN_00008790();
                cVar2 = *(char *)((long)puVar12 + 0x17);
                puVar3 = puVar12;
                if (cVar2 < '\0') {
                  puVar3 = (void *)*puVar12;
                }
                if (uVar8 != 0) {
                  if (((undefined8 *)((long)puVar10 + uVar8) < puVar10) ||
                     ((puVar10 <= puVar3 && (puVar3 < (undefined8 *)((long)puVar10 + uVar8)))))
                  goto LAB_043d0934;
                  _memmove(puVar10,puVar3,uVar8);
                }
                if (cVar2 < '\0') {
                  FUN_00005810(*puVar12,puVar12[2] & 0x7fffffffffffffff);
                }
                *puVar12 = puVar10;
                puVar12[2] = uVar6 | 0x8000000000000000;
              }
              else {
                puVar10 = (undefined8 *)*puVar12;
                uVar8 = puVar12[1];
              }
              puVar12[1] = uVar8 + 1;
            }
            else {
              if (bVar1 == 0x16) {
                uVar8 = 0x16;
                uVar6 = 0x30;
                goto LAB_043d08a8;
              }
              *(byte *)((long)puVar12 + 0x17) = bVar1 + 1 & 0x7f;
              puVar10 = puVar12;
            }
            *(undefined2 *)((long)puVar10 + uVar8) = 0x2f;
          }
        }
      }
      puVar12 = puVar12 + 3;
      lVar11 = lVar11 + -0x18;
    } while (lVar11 != 0);
  }
  plVar9 = (long *)(param_1 + 0x20);
  lVar11 = *plVar9;
  if (lVar11 != 0) {
    lVar7 = *(long *)(param_1 + 0x28) * 0x18;
    lVar5 = lVar11;
    if (lVar7 != 0) {
      lVar5 = *(long *)(param_1 + 0x28) * -0x18;
      puVar12 = (undefined8 *)(lVar7 + lVar11);
      do {
        puVar10 = puVar12 + -3;
        if (puVar10 == (undefined8 *)0x0) {
LAB_043d0934:
                    /* WARNING: Does not return */
          pcVar4 = (code *)SoftwareBreakpoint(1,0x43d0938);
          (*pcVar4)();
        }
        if (*(char *)((long)puVar12 + -1) < '\0') {
          FUN_00005810(*puVar10,puVar12[-1] & 0x7fffffffffffffff);
        }
        lVar5 = lVar5 + 0x18;
        puVar12 = puVar10;
      } while (lVar5 != 0);
      lVar5 = *plVar9;
    }
    *(long *)(param_1 + 0x28) = (lVar11 - lVar5 >> 3) * -0x5555555555555555;
    FUN_00005810(lVar5,*(long *)(param_1 + 0x30) * 0x18);
    *plVar9 = 0;
    *(undefined8 *)(param_1 + 0x28) = 0;
    *(undefined8 *)(param_1 + 0x30) = 0;
  }
  *(undefined8 *)(param_1 + 0x20) = *param_2;
  *(undefined8 *)(param_1 + 0x28) = param_2[1];
  *(undefined8 *)(param_1 + 0x30) = param_2[2];
  *param_2 = 0;
  param_2[1] = 0;
  param_2[2] = 0;
  return;
}


