// ==== 0x43386f8  FUN_043386f8  (336 bytes)

undefined8 * FUN_043386f8(undefined8 *param_1,long param_2)

{
  undefined8 *puVar1;
  code *pcVar2;
  int iVar3;
  undefined8 *puVar4;
  long *plVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  undefined8 *local_40;
  undefined8 uStack_38;
  
  *param_1 = &PTR_FUN_0da7eed0;
  local_40 = (undefined8 *)0x0;
  FUN_02ce0278(&local_40,*(undefined8 *)(param_2 + 8));
  FUN_007b7670(&uStack_38,&local_40);
  puVar4 = (undefined8 *)FUN_00008790(0x20);
  puVar1 = local_40;
  puVar4[1] = 0;
  puVar4[2] = 0;
  *puVar4 = &PTR_viewWillStartLiveResize_0da7ef60;
  if (puVar4 + 3 == (undefined8 *)0x0) {
LAB_043387d0:
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43387d4);
    (*pcVar2)();
  }
  puVar4[3] = uStack_38;
  param_1[1] = puVar4 + 3;
  param_1[2] = puVar4;
  if (local_40 != (undefined8 *)0x0) {
    LOAcquire();
    iVar3 = *(int *)(local_40 + 1);
    *(int *)(local_40 + 1) = iVar3 + -1;
    LORelease();
    if (iVar3 == 1) {
      *local_40 = &PTR_FUN_0dab38f0;
      plVar5 = (long *)local_40[5];
      local_40[5] = 0;
      if (plVar5 != (long *)0x0) {
        (**(code **)(*plVar5 + 8))();
      }
      lVar7 = puVar1[2];
      if (lVar7 != 0) {
        lVar6 = puVar1[3] * 0x208;
        lVar8 = lVar7;
        if (lVar6 != 0) {
          lVar6 = lVar6 + lVar7;
          lVar8 = puVar1[3] * -0x208;
          do {
            if (lVar6 == 0x208) goto LAB_043387d0;
            lVar6 = FUN_01c8dcf8();
            lVar8 = lVar8 + 0x208;
          } while (lVar8 != 0);
          lVar8 = puVar1[2];
        }
        puVar1[3] = (lVar7 - lVar8 >> 3) * 0xfc0fc0fc0fc0fc1;
        FUN_00005810(lVar8,puVar1[4] * 0x208);
      }
      FUN_00005810(puVar1,0x40);
    }
  }
  return param_1;
}


