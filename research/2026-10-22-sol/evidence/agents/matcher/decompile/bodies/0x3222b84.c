// ==== 0x3222b84  FUN_03222b84  (536 bytes)

void FUN_03222b84(ulong param_1,long param_2)

{
  code *pcVar1;
  int iVar2;
  undefined8 *puVar3;
  int *piVar4;
  int *piVar5;
  undefined8 uVar6;
  long lVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined1 auStack_60 [32];
  
  if (param_2 == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x3222cf4);
    (*pcVar1)();
  }
  if (*(char *)(param_1 + 0x21) != '\0') {
    puVar3 = (undefined8 *)FUN_02ed7f64();
    uVar8 = *puVar3;
    FUN_00029e40(auStack_60,"ScheduleCallbackOnBookmarkModelLoad",
                 "../../components/bookmarks/browser/bookmark_model_load_waiter.cc",0x53);
    piVar4 = *(int **)(param_1 + 0x170);
    if ((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x3222db4);
      (*pcVar1)();
    }
    uVar9 = *(undefined8 *)(param_1 + 0x178);
    uVar6 = FUN_00008790(0x40);
    lVar7 = FUN_01064994(uVar6,&LAB_011d1330,FUN_0650bb00);
    *(undefined1 **)(lVar7 + 0x20) = &LAB_09f54180;
    *(int **)(lVar7 + 0x28) = piVar4;
    *(undefined8 *)(lVar7 + 0x30) = uVar9;
    *(long *)(lVar7 + 0x38) = param_2;
    FUN_00a1c3cc(uVar8,auStack_60,uVar6);
    return;
  }
  puVar3 = (undefined8 *)FUN_00008790(0x30);
  *puVar3 = &PTR_FUN_0da8a590;
  piVar4 = (int *)FUN_00008790(8);
  *piVar4 = 0;
  *(undefined1 *)(piVar4 + 1) = 0;
  if ((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x3222d0c);
    (*pcVar1)();
  }
  *puVar3 = &PTR_FUN_0db35ce0;
  puVar3[1] = piVar4;
  puVar3[2] = puVar3;
  puVar3[3] = 0;
  if (((ulong)puVar3 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(puVar3);
  }
  puVar3[4] = puVar3;
  puVar3[5] = 0;
  if (*(char *)(param_1 + 0x21) == '\0') {
    piVar4 = (int *)puVar3[3];
    puVar3[3] = param_2;
    if (piVar4 != (int *)0x0) {
      LOAcquire();
      iVar2 = *piVar4;
      *piVar4 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        if (*(code **)(piVar4 + 4) == FUN_039a66c0) {
          piVar5 = *(int **)(piVar4 + 0xc);
          if (piVar5 != (int *)0x0) {
            LOAcquire();
            iVar2 = *piVar5;
            *piVar5 = iVar2 + -1;
            LORelease();
            if (iVar2 == 1) {
              FUN_00005810(piVar5,8);
            }
          }
          FUN_00005810(piVar4,0x48);
        }
        else {
          (**(code **)(piVar4 + 4))(piVar4);
        }
      }
    }
    if ((param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(param_1);
    }
    if ((puVar3[5] & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    puVar3[5] = param_1;
    FUN_00575e00(param_1 + 0x70,puVar3[4]);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0,0x3222d00);
  (*pcVar1)();
}


