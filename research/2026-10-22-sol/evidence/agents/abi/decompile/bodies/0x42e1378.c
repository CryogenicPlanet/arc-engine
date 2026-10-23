// ==== 0x42e1378  FUN_042e1378  (764 bytes)

void FUN_042e1378(undefined8 *param_1,long *param_2,undefined8 param_3)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  code *pcVar4;
  long *plVar5;
  long lVar6;
  long local_88;
  long local_80;
  long local_78;
  undefined1 auStack_70 [8];
  long *local_68;
  
  param_2 = (long *)*param_2;
  lVar6 = *param_2;
  plVar5 = (long *)param_2[2];
  lVar2 = param_2[2];
  lVar1 = param_2[1];
  if (plVar5 != (long *)0x0) {
    plVar3 = plVar5 + 2;
    *plVar3 = *plVar3 + 1;
    *plVar3 = *plVar3 + 1;
  }
  plVar3 = *(long **)(lVar6 + 0x80);
  *(long *)(lVar6 + 0x80) = lVar2;
  *(long *)(lVar6 + 0x78) = lVar1;
  if (plVar3 != (long *)0x0) {
    if (plVar3[2] != 0) {
      LOAcquire();
      lVar1 = plVar3[2];
      plVar3[2] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) goto LAB_042e13f8;
    }
    pcVar4 = *(code **)(*plVar3 + 0x20);
    if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
      FUN_00005810(plVar3,0x20);
    }
    else {
      (*pcVar4)();
    }
  }
LAB_042e13f8:
  if (plVar5 != (long *)0x0) {
    if (plVar5[2] != 0) {
      LOAcquire();
      lVar1 = plVar5[2];
      plVar5[2] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) goto LAB_042e1428;
    }
    pcVar4 = *(code **)(*plVar5 + 0x20);
    if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
      FUN_00005810(plVar5,0x20);
    }
    else {
      (*pcVar4)(plVar5);
    }
  }
LAB_042e1428:
  if (*(char *)(lVar6 + 0x28) != '\0') {
    FUN_042c84f8(auStack_70,param_3);
    FUN_042dfe4c(&local_88,lVar6 + 0x38);
    FUN_042df994(lVar6 + 0x78,auStack_70,&local_88);
    if (local_88 != 0) {
      if (local_80 * 0x10 != 0) {
        lVar6 = local_80 * -0x10;
        lVar1 = local_80 * 0x10 + local_88;
        do {
          if (lVar1 + -0x10 == 0) {
                    /* WARNING: Does not return */
            pcVar4 = (code *)SoftwareBreakpoint(1,0x42e1674);
            (*pcVar4)();
          }
          plVar5 = *(long **)(lVar1 + -8);
          if (plVar5 != (long *)0x0) {
            LOAcquire();
            lVar2 = plVar5[1];
            plVar5[1] = lVar2 + -1;
            LORelease();
            if (lVar2 == 0) {
              (**(code **)(*plVar5 + 0x10))(plVar5);
              if (plVar5[2] != 0) {
                LOAcquire();
                lVar2 = plVar5[2];
                plVar5[2] = lVar2 + -1;
                LORelease();
                if (lVar2 != 0) goto LAB_042e1570;
              }
              pcVar4 = *(code **)(*plVar5 + 0x20);
              if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)
                 ) {
                FUN_00005810(plVar5,0x20);
              }
              else {
                (*pcVar4)(plVar5);
              }
            }
          }
LAB_042e1570:
          lVar6 = lVar6 + 0x10;
          lVar1 = lVar1 + -0x10;
        } while (lVar6 != 0);
      }
      FUN_00005810(local_88,local_78 << 4);
    }
    if (local_68 != (long *)0x0) {
      LOAcquire();
      lVar6 = local_68[1];
      local_68[1] = lVar6 + -1;
      LORelease();
      if (lVar6 == 0) {
        (**(code **)(*local_68 + 0x10))(local_68);
        if (local_68[2] != 0) {
          LOAcquire();
          lVar6 = local_68[2];
          local_68[2] = lVar6 + -1;
          LORelease();
          if (lVar6 != 0) goto LAB_042e153c;
        }
        pcVar4 = *(code **)(*local_68 + 0x20);
        if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
          FUN_00005810(local_68,0x20);
        }
        else {
          (*pcVar4)(local_68);
        }
      }
    }
  }
LAB_042e153c:
  *param_1 = 1;
  return;
}


