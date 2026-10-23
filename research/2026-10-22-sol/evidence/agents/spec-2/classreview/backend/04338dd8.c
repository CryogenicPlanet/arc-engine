// ==== 0x4338dd8  FUN_04338dd8  (344 bytes)

void FUN_04338dd8(undefined8 *param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  uint uVar2;
  long lVar3;
  long *plVar4;
  undefined8 *puVar5;
  code *pcVar6;
  uint *puVar7;
  long *plVar8;
  
  *param_1 = &PTR_FUN_0da7eed0;
  plVar4 = (long *)FUN_00008790(0x20);
  plVar8 = plVar4 + 1;
  *plVar8 = 0;
  plVar4[2] = 0;
  *plVar4 = (long)&PTR_viewWillStartLiveResize_0da7ef60;
  if (plVar4 + 3 != (long *)0x0) {
    plVar4[3] = 0;
    puVar5 = (undefined8 *)FUN_00008790(0x20);
    puVar5[1] = 0;
    puVar5[2] = 0;
    *puVar5 = &PTR_viewWillStartLiveResize_0da7ef60;
    puVar1 = puVar5 + 3;
    if (puVar1 != (undefined8 *)0x0) {
      puVar7 = (uint *)*param_2;
      *puVar1 = puVar7;
      if ((puVar7 != (uint *)0x0) && (uVar2 = *puVar7, *puVar7 = uVar2 + 1, 0xfffffffe < uVar2)) {
                    /* WARNING: Does not return */
        pcVar6 = (code *)SoftwareBreakpoint(0,0x4338f30);
        (*pcVar6)();
      }
      param_1[1] = puVar1;
      param_1[2] = puVar5;
      if (plVar4 != (long *)0x0) {
        LOAcquire();
        lVar3 = *plVar8;
        *plVar8 = lVar3 + -1;
        LORelease();
        if (lVar3 == 0) {
          (**(code **)(*plVar4 + 0x10))(plVar4);
          if (plVar4[2] != 0) {
            LOAcquire();
            lVar3 = plVar4[2];
            plVar4[2] = lVar3 + -1;
            LORelease();
            if (lVar3 != 0) {
              return;
            }
          }
          pcVar6 = *(code **)(*plVar4 + 0x20);
          if (((pcVar6 != FUN_00f01bc0) && (pcVar6 != FUN_00f01bc0)) && (pcVar6 != FUN_013efb40)) {
            (*pcVar6)(plVar4);
            return;
          }
          FUN_00005810(plVar4,0x20);
          return;
        }
      }
      return;
    }
  }
                    /* WARNING: Does not return */
  pcVar6 = (code *)SoftwareBreakpoint(1,0x4338f2c);
  (*pcVar6)();
}


