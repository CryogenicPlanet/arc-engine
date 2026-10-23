// ==== 0x443808c  ArcBrowserContext::setBookmarkObservers:  (1268 bytes)

/* Function Stack Size: 0x28 bytes */

void ArcBrowserContext::setBookmarkObservers_
               (ID param_1,SEL param_2,
               vector<std::shared_ptr<ArcBookmarkModelObserverBridge>,_std::allocator<std::shared_ptr<ArcBookmarkModelObserverBridge>>>
               *param_3)

{
  vector<std::shared_ptr<ArcBookmarkModelObserverBridge>,_std::allocator<std::shared_ptr<ArcBookmarkModelObserverBridge>>>
  *pvVar1;
  undefined8 *puVar2;
  unsigned_long_long uVar3;
  ulong uVar4;
  void *pvVar5;
  undefined8 *puVar6;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  ulong uVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  code *pcVar10;
  ulong uVar11;
  long lVar12;
  void *pvVar13;
  long *plVar14;
  long *plVar15;
  long *plVar16;
  long lVar17;
  long lVar18;
  
  pvVar1 = (vector<std::shared_ptr<ArcBookmarkModelObserverBridge>,_std::allocator<std::shared_ptr<ArcBookmarkModelObserverBridge>>>
            *)(param_1 + 0x48);
  if (pvVar1 != param_3) {
    puVar8 = (param_3->field0_0x0).field0_0x0;
    uVar3 = (param_3->field0_0x0).field1_0x8;
    lVar17 = uVar3 << 4;
    puVar2 = puVar8 + uVar3 * 2;
    uVar4 = (long)(uVar3 << 4) >> 4;
    uVar7 = *(ulong *)(param_1 + 0x58);
    if (uVar7 < uVar4) {
      pvVar13 = (pvVar1->field0_0x0).field0_0x0;
      if (pvVar13 != (void *)0x0) {
        lVar12 = *(long *)(param_1 + 0x50) * 0x10;
        pvVar5 = pvVar13;
        if (lVar12 != 0) {
          lVar18 = *(long *)(param_1 + 0x50) * -0x10;
          pvVar5 = (void *)(lVar12 + (long)pvVar13);
          do {
            if ((void *)((long)pvVar5 + -0x10) == (void *)0x0) goto LAB_04438368;
            plVar14 = *(long **)((long)pvVar5 + -8);
            if (plVar14 != (long *)0x0) {
              LOAcquire();
              lVar12 = plVar14[1];
              plVar14[1] = lVar12 + -1;
              LORelease();
              if (lVar12 == 0) {
                (**(code **)(*plVar14 + 0x10))(plVar14);
                if (plVar14[2] != 0) {
                  LOAcquire();
                  lVar12 = plVar14[2];
                  plVar14[2] = lVar12 + -1;
                  LORelease();
                  if (lVar12 != 0) goto LAB_044382d4;
                }
                pcVar10 = *(code **)(*plVar14 + 0x20);
                if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) ||
                   (pcVar10 == FUN_013efb40)) {
                  FUN_00005810(plVar14,0x20);
                }
                else {
                  (*pcVar10)(plVar14);
                }
              }
            }
LAB_044382d4:
            lVar18 = lVar18 + 0x10;
            pvVar5 = (void *)((long)pvVar5 + -0x10);
          } while (lVar18 != 0);
          uVar7 = *(ulong *)(param_1 + 0x58);
          pvVar5 = *(void **)(param_1 + 0x48);
        }
        *(long *)(param_1 + 0x50) = (long)pvVar13 - (long)pvVar5 >> 4;
        FUN_00005810(pvVar5,uVar7 << 4);
        uVar7 = 0;
        *(undefined8 *)(param_1 + 0x48) = 0;
        *(undefined8 *)(param_1 + 0x50) = 0;
        *(undefined8 *)(param_1 + 0x58) = 0;
      }
      if (uVar4 >> 0x3c != 0) {
LAB_0443857c:
                    /* WARNING: Subroutine does not return */
        FUN_040416c0();
      }
      uVar11 = uVar7 * 2;
      if (uVar11 < uVar4 || uVar11 - uVar4 == 0) {
        uVar11 = uVar4;
      }
      if (0x7fffffffffffffe < uVar7) {
        uVar11 = 0xfffffffffffffff;
      }
      if (uVar11 >> 0x3c != 0) goto LAB_0443857c;
      puVar6 = (undefined8 *)FUN_00008790(uVar11 << 4);
      *(undefined8 **)(param_1 + 0x48) = puVar6;
      *(undefined8 *)(param_1 + 0x50) = 0;
      *(ulong *)(param_1 + 0x58) = uVar11;
      puVar9 = puVar6;
      if (lVar17 != 0) {
        do {
          if (puVar6 == (undefined8 *)0x0) {
LAB_04438368:
                    /* WARNING: Does not return */
            pcVar10 = (code *)SoftwareBreakpoint(1,0x443836c);
            (*pcVar10)();
          }
          *puVar6 = *puVar8;
          lVar17 = puVar8[1];
          puVar6[1] = lVar17;
          if (lVar17 != 0) {
            *(long *)(lVar17 + 8) = *(long *)(lVar17 + 8) + 1;
          }
          puVar8 = puVar8 + 2;
          puVar6 = puVar6 + 2;
        } while (puVar8 != puVar2);
        puVar9 = *(undefined8 **)(param_1 + 0x48);
      }
      lVar17 = (long)puVar6 - (long)puVar9;
    }
    else {
      uVar7 = *(ulong *)(param_1 + 0x50);
      if (uVar7 < uVar4) {
        pvVar13 = (pvVar1->field0_0x0).field0_0x0;
        puVar6 = puVar8;
        if ((0 < (long)uVar7) && (lVar17 = uVar7 << 4, lVar17 != 0)) {
          puVar6 = puVar8 + uVar7 * 2;
          plVar14 = (long *)((long)pvVar13 + 8);
          plVar16 = puVar8 + 1;
          do {
            lVar18 = *plVar16;
            lVar12 = plVar16[-1];
            if (*plVar16 != 0) {
              plVar15 = (long *)(*plVar16 + 8);
              *plVar15 = *plVar15 + 1;
            }
            plVar15 = (long *)*plVar14;
            *plVar14 = lVar18;
            plVar14[-1] = lVar12;
            if (plVar15 != (long *)0x0) {
              LOAcquire();
              lVar12 = plVar15[1];
              plVar15[1] = lVar12 + -1;
              LORelease();
              if (lVar12 == 0) {
                (**(code **)(*plVar15 + 0x10))(plVar15,param_2);
                if (plVar15[2] != 0) {
                  LOAcquire();
                  lVar12 = plVar15[2];
                  plVar15[2] = lVar12 + -1;
                  LORelease();
                  param_2 = extraout_x1;
                  if (lVar12 != 0) goto LAB_04438378;
                }
                pcVar10 = *(code **)(*plVar15 + 0x20);
                if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) ||
                   (pcVar10 == FUN_013efb40)) {
                  FUN_00005810(plVar15,0x20);
                  param_2 = extraout_x1_00;
                }
                else {
                  (*pcVar10)(plVar15);
                  param_2 = extraout_x1_01;
                }
              }
            }
LAB_04438378:
            plVar16 = plVar16 + 2;
            plVar14 = plVar14 + 2;
            lVar17 = lVar17 + -0x10;
          } while (lVar17 != 0);
          pvVar13 = *(void **)(param_1 + 0x48);
          uVar7 = *(ulong *)(param_1 + 0x50);
        }
        puVar8 = (undefined8 *)((long)pvVar13 + uVar7 * 0x10);
        if (puVar6 != puVar2) {
          do {
            if (puVar8 == (undefined8 *)0x0) goto LAB_04438368;
            *puVar8 = *puVar6;
            lVar17 = puVar6[1];
            puVar8[1] = lVar17;
            if (lVar17 != 0) {
              *(long *)(lVar17 + 8) = *(long *)(lVar17 + 8) + 1;
            }
            puVar6 = puVar6 + 2;
            puVar8 = puVar8 + 2;
          } while (puVar6 != puVar2);
          pvVar13 = *(void **)(param_1 + 0x48);
        }
        lVar17 = (long)puVar8 - (long)pvVar13;
      }
      else {
        plVar16 = (pvVar1->field0_0x0).field0_0x0;
        plVar14 = plVar16;
        if (lVar17 != 0) {
          plVar16 = puVar8 + 1;
          do {
            lVar18 = *plVar16;
            lVar12 = plVar16[-1];
            if (*plVar16 != 0) {
              plVar15 = (long *)(*plVar16 + 8);
              *plVar15 = *plVar15 + 1;
            }
            plVar15 = (long *)plVar14[1];
            plVar14[1] = lVar18;
            *plVar14 = lVar12;
            if (plVar15 != (long *)0x0) {
              LOAcquire();
              lVar12 = plVar15[1];
              plVar15[1] = lVar12 + -1;
              LORelease();
              if (lVar12 == 0) {
                (**(code **)(*plVar15 + 0x10))(plVar15);
                if (plVar15[2] != 0) {
                  LOAcquire();
                  lVar12 = plVar15[2];
                  plVar15[2] = lVar12 + -1;
                  LORelease();
                  if (lVar12 != 0) goto LAB_04438428;
                }
                pcVar10 = *(code **)(*plVar15 + 0x20);
                if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) ||
                   (pcVar10 == FUN_013efb40)) {
                  FUN_00005810(plVar15,0x20);
                }
                else {
                  (*pcVar10)(plVar15);
                }
              }
            }
LAB_04438428:
            plVar16 = plVar16 + 2;
            plVar14 = plVar14 + 2;
            lVar17 = lVar17 + -0x10;
          } while (lVar17 != 0);
          plVar16 = *(long **)(param_1 + 0x48);
          uVar7 = *(ulong *)(param_1 + 0x50);
        }
        plVar15 = plVar16 + uVar7 * 2;
        if (plVar14 != plVar15) {
          do {
            plVar16 = plVar15 + -2;
            if (plVar16 == (long *)0x0) goto LAB_04438368;
            plVar15 = (long *)plVar15[-1];
            if (plVar15 != (long *)0x0) {
              LOAcquire();
              lVar17 = plVar15[1];
              plVar15[1] = lVar17 + -1;
              LORelease();
              if (lVar17 == 0) {
                (**(code **)(*plVar15 + 0x10))(plVar15);
                if (plVar15[2] != 0) {
                  LOAcquire();
                  lVar17 = plVar15[2];
                  plVar15[2] = lVar17 + -1;
                  LORelease();
                  if (lVar17 != 0) goto LAB_044384f4;
                }
                pcVar10 = *(code **)(*plVar15 + 0x20);
                if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) ||
                   (pcVar10 == FUN_013efb40)) {
                  FUN_00005810(plVar15,0x20);
                }
                else {
                  (*pcVar10)(plVar15);
                }
              }
            }
LAB_044384f4:
            plVar15 = plVar16;
          } while (plVar16 != plVar14);
          plVar16 = *(long **)(param_1 + 0x48);
        }
        lVar17 = (long)plVar14 - (long)plVar16;
      }
    }
    *(long *)(param_1 + 0x50) = lVar17 >> 4;
  }
  return;
}


