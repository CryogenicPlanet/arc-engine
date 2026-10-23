// ==== 0x42f3334  FUN_042f3334  (460 bytes)

void FUN_042f3334(undefined8 *param_1,ulong param_2,undefined8 *param_3)

{
  long *plVar1;
  int iVar2;
  ulong uVar3;
  int *piVar4;
  undefined8 uVar5;
  long lVar6;
  code *pcVar7;
  int *local_50;
  undefined8 *local_48;
  
  if (*(long *)(param_2 + 0x70) == 0) {
    uVar5 = FUN_04b37104(0x25,"managed profile is null",0x17,0xf4,
                         "../../arc/browser/browser_context/preferences_impl.cc");
    goto LAB_042f3468;
  }
  uVar3 = FUN_01a9dba4();
  if ((param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_2);
  }
  piVar4 = (int *)FUN_00008790(0x48);
  *piVar4 = 1;
  *(code **)(piVar4 + 2) = FUN_042f60d4;
  *(code **)(piVar4 + 4) = FUN_042f6694;
  *(code **)(piVar4 + 6) = FUN_00b77080;
  if ((uVar3 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(uVar3);
  }
  *(ulong *)(piVar4 + 10) = uVar3;
  lVar6 = param_3[1];
  uVar5 = *param_3;
  *(undefined8 *)(piVar4 + 0xe) = param_3[1];
  *(undefined8 *)(piVar4 + 0xc) = uVar5;
  if (lVar6 != 0) {
    plVar1 = (long *)(lVar6 + 8);
    *plVar1 = *plVar1 + 1;
  }
  *(ulong *)(piVar4 + 0x10) = param_2;
  if (*(char *)(uVar3 + 0x170) == '\0') {
    if (piVar4 == (int *)0x0) {
                    /* WARNING: Does not return */
      pcVar7 = (code *)SoftwareBreakpoint(0,0x42f34f8);
      (*pcVar7)();
    }
    FUN_01469bb4(&local_50,uVar3 + 0x1e8,piVar4);
    local_48 = param_3;
    lVar6 = FUN_042f6c18(param_2 + 0x130,&local_48);
    if (lVar6 == 0) {
                    /* WARNING: Does not return */
      pcVar7 = (code *)SoftwareBreakpoint(1,0x42f3500);
      (*pcVar7)();
    }
    FUN_01469964(lVar6 + 0x20,&local_50);
    piVar4 = local_50;
    if (local_50 != (int *)0x0) {
      local_50 = (int *)0x0;
      (**(code **)(piVar4 + 2))(piVar4);
      LOAcquire();
      iVar2 = *piVar4;
      *piVar4 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        (**(code **)(piVar4 + 4))(piVar4);
      }
      if (local_50 != (int *)0x0) {
        LOAcquire();
        iVar2 = *local_50;
        *local_50 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          pcVar7 = *(code **)(local_50 + 4);
          piVar4 = local_50;
          goto LAB_042f3420;
        }
      }
    }
  }
  else {
    if (piVar4 == (int *)0x0) {
                    /* WARNING: Does not return */
      pcVar7 = (code *)SoftwareBreakpoint(0,0x42f3504);
      (*pcVar7)();
    }
    FUN_042f60d4(piVar4);
    LOAcquire();
    iVar2 = *piVar4;
    *piVar4 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      pcVar7 = *(code **)(piVar4 + 4);
LAB_042f3420:
      (*pcVar7)(piVar4);
    }
  }
  uVar5 = 1;
LAB_042f3468:
  *param_1 = uVar5;
  return;
}


