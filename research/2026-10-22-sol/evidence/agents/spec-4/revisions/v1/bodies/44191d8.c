// ==== 0x44191d8  ArcWebContentsThumbnailsGenerator::.cxx_destruct  (468 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContentsThumbnailsGenerator::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  code *pcVar2;
  long lVar3;
  long *plVar4;
  long lVar5;
  long lVar6;
  
  lVar5 = *(long *)(param_1 + 0x28);
  if (lVar5 != 0) {
    lVar3 = *(long *)(param_1 + 0x30) * 0x10;
    lVar6 = lVar5;
    if (lVar3 != 0) {
      lVar6 = *(long *)(param_1 + 0x30) * -0x10;
      lVar3 = lVar3 + lVar5;
      do {
        if (lVar3 + -0x10 == 0) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(1,0x4419380);
          (*pcVar2)();
        }
        plVar4 = *(long **)(lVar3 + -8);
        if (plVar4 != (long *)0x0) {
          LOAcquire();
          lVar1 = plVar4[1];
          plVar4[1] = lVar1 + -1;
          LORelease();
          if (lVar1 == 0) {
            (**(code **)(*plVar4 + 0x10))(plVar4,param_2);
            if (plVar4[2] != 0) {
              LOAcquire();
              lVar1 = plVar4[2];
              plVar4[2] = lVar1 + -1;
              LORelease();
              param_2 = extraout_x1;
              if (lVar1 != 0) goto LAB_04419244;
            }
            pcVar2 = *(code **)(*plVar4 + 0x20);
            if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40))
            {
              FUN_00005810(plVar4,0x20);
              param_2 = extraout_x1_00;
            }
            else {
              (*pcVar2)(plVar4);
              param_2 = extraout_x1_01;
            }
          }
        }
LAB_04419244:
        lVar6 = lVar6 + 0x10;
        lVar3 = lVar3 + -0x10;
      } while (lVar6 != 0);
      lVar6 = *(long *)(param_1 + 0x28);
    }
    *(long *)(param_1 + 0x30) = lVar5 - lVar6 >> 4;
    FUN_00005810(lVar6,*(long *)(param_1 + 0x38) << 4);
  }
  if ((*(char *)(param_1 + 0x20) != '\0') &&
     (plVar4 = *(long **)(param_1 + 0x18), plVar4 != (long *)0x0)) {
    LOAcquire();
    lVar5 = plVar4[1];
    plVar4[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar5 = plVar4[2];
        plVar4[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_04419304;
      }
      pcVar2 = *(code **)(*plVar4 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar2)(plVar4);
      }
    }
  }
LAB_04419304:
  _objc_destroyWeak(param_1 + 8);
  return;
}


