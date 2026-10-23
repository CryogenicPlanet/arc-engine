// ==== 0x42e116c  FUN_042e116c  (524 bytes)

void FUN_042e116c(long param_1)

{
  long lVar1;
  code *pcVar2;
  long lVar3;
  long *plVar4;
  long lVar5;
  long lVar6;
  
  if (param_1 == 0) {
    return;
  }
  lVar5 = *(long *)(param_1 + 0x48);
  if (lVar5 != 0) {
    lVar3 = *(long *)(param_1 + 0x50) * 0x10;
    lVar6 = lVar5;
    if (lVar3 != 0) {
      lVar6 = *(long *)(param_1 + 0x50) * -0x10;
      lVar3 = lVar3 + lVar5;
      do {
        if (lVar3 + -0x10 == 0) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(1,0x42e1378);
          (*pcVar2)();
        }
        plVar4 = *(long **)(lVar3 + -8);
        if (plVar4 != (long *)0x0) {
          LOAcquire();
          lVar1 = plVar4[1];
          plVar4[1] = lVar1 + -1;
          LORelease();
          if (lVar1 == 0) {
            (**(code **)(*plVar4 + 0x10))(plVar4);
            if (plVar4[2] != 0) {
              LOAcquire();
              lVar1 = plVar4[2];
              plVar4[2] = lVar1 + -1;
              LORelease();
              if (lVar1 != 0) goto LAB_042e11f8;
            }
            pcVar2 = *(code **)(*plVar4 + 0x20);
            if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40))
            {
              FUN_00005810(plVar4,0x20);
            }
            else {
              (*pcVar2)(plVar4);
            }
          }
        }
LAB_042e11f8:
        lVar6 = lVar6 + 0x10;
        lVar3 = lVar3 + -0x10;
      } while (lVar6 != 0);
      lVar6 = *(long *)(param_1 + 0x48);
    }
    *(long *)(param_1 + 0x50) = lVar5 - lVar6 >> 4;
    FUN_00005810(lVar6,*(long *)(param_1 + 0x58) << 4);
  }
  plVar4 = *(long **)(param_1 + 0x40);
  if (plVar4 != (long *)0x0) {
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
        if (lVar5 != 0) goto LAB_042e12b0;
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
LAB_042e12b0:
  if ((*(ulong *)(param_1 + 0x30) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_000270f0();
  }
  FUN_00005810(param_1,0x60);
  return;
}


