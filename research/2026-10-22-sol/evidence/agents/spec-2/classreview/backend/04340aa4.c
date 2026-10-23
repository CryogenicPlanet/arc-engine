// ==== 0x4340aa4  FUN_04340aa4  (364 bytes)

void FUN_04340aa4(long *param_1,long *param_2)

{
  long *plVar1;
  long lVar2;
  undefined8 *puVar3;
  code *pcVar4;
  long lVar5;
  long lVar6;
  long *plVar7;
  undefined8 local_70;
  long *local_68;
  
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  lVar6 = *(long *)(*param_2 + 0xb0) * 0x298;
  if (lVar6 != 0) {
    lVar5 = *(long *)(*param_2 + 0xa8);
    do {
      FUN_043404b4(&local_70,lVar5);
      if (param_1[1] == param_1[2]) {
        puVar3 = (undefined8 *)FUN_041e7c00(param_1,&local_70);
      }
      else {
        puVar3 = (undefined8 *)(*param_1 + param_1[1] * 0x10);
        if (puVar3 == (undefined8 *)0x0) goto LAB_04340bfc;
        *puVar3 = local_70;
        puVar3[1] = local_68;
        local_70 = 0;
        local_68 = (long *)0x0;
        puVar3 = puVar3 + 2;
      }
      plVar1 = local_68;
      param_1[1] = (long)puVar3 - *param_1 >> 4;
      if ((long)puVar3 - *param_1 == 0) {
LAB_04340bfc:
                    /* WARNING: Does not return */
        pcVar4 = (code *)SoftwareBreakpoint(1,0x4340c00);
        (*pcVar4)();
      }
      if (local_68 != (long *)0x0) {
        LOAcquire();
        lVar2 = local_68[1];
        local_68[1] = lVar2 + -1;
        LORelease();
        if (lVar2 == 0) {
          plVar7 = local_68 + 2;
          (**(code **)(*local_68 + 0x10))(local_68);
          if (*plVar7 != 0) {
            LOAcquire();
            lVar2 = plVar1[2];
            plVar1[2] = lVar2 + -1;
            LORelease();
            if (lVar2 != 0) goto LAB_04340b34;
          }
          pcVar4 = *(code **)(*plVar1 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(plVar1,0x20);
          }
          else {
            (*pcVar4)(plVar1);
          }
        }
      }
LAB_04340b34:
      lVar5 = lVar5 + 0x298;
      lVar6 = lVar6 + -0x298;
    } while (lVar6 != 0);
  }
  return;
}


