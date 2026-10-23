// ==== 0x42b785c  FUN_042b785c  (280 bytes)

void FUN_042b785c(undefined8 *param_1,long param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  long lVar3;
  code *pcVar4;
  long *plVar5;
  
  if (*(long *)(param_2 + 0xb8) == 0) {
    puVar1 = (undefined8 *)FUN_00008790(0xb0);
    puVar1[1] = 0;
    puVar1[2] = 0;
    *puVar1 = &PTR_viewWillStartLiveResize_0da7bdf8;
    if (puVar1 + 3 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(1,0x42b7974);
      (*pcVar4)();
    }
    uVar2 = FUN_0432c68c(puVar1 + 3);
    plVar5 = *(long **)(param_2 + 0xc0);
    *(undefined8 *)(param_2 + 0xb8) = uVar2;
    *(undefined8 **)(param_2 + 0xc0) = puVar1;
    if (plVar5 != (long *)0x0) {
      LOAcquire();
      lVar3 = plVar5[1];
      plVar5[1] = lVar3 + -1;
      LORelease();
      if (lVar3 == 0) {
        (**(code **)(*plVar5 + 0x10))(plVar5);
        if (plVar5[2] != 0) {
          LOAcquire();
          lVar3 = plVar5[2];
          plVar5[2] = lVar3 + -1;
          LORelease();
          if (lVar3 != 0) goto LAB_042b787c;
        }
        pcVar4 = *(code **)(*plVar5 + 0x20);
        if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
          FUN_00005810(plVar5,0x20);
        }
        else {
          (*pcVar4)(plVar5);
        }
      }
    }
  }
LAB_042b787c:
  lVar3 = *(long *)(param_2 + 0xc0);
  uVar2 = *(undefined8 *)(param_2 + 0xb8);
  param_1[1] = *(undefined8 *)(param_2 + 0xc0);
  *param_1 = uVar2;
  if (lVar3 != 0) {
    plVar5 = (long *)(lVar3 + 8);
    *plVar5 = *plVar5 + 1;
  }
  return;
}


