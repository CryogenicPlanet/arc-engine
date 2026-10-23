// ==== 0x784a050  FUN_0784a050  (404 bytes)

/* WARNING: Removing unreachable block (ram,0x0784a15c) */

void FUN_0784a050(long param_1)

{
  uint uVar1;
  code *pcVar2;
  int iVar3;
  ulong uVar4;
  undefined8 uVar5;
  long lVar6;
  long lVar7;
  long *plVar8;
  undefined8 local_b8 [2];
  undefined7 local_a8;
  char cStack_a1;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined8 local_30;
  undefined4 local_24;
  
  iVar3 = *(int *)(param_1 + 8);
  *(int *)(param_1 + 8) = iVar3 + 1;
  if (iVar3 == 0x7fffffff) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(0,0x784a1cc);
    (*pcVar2)();
  }
  lVar6 = param_1 + 0x28;
  lVar7 = (long)*(char *)(param_1 + 0x3f);
  if (lVar7 < 0) {
    lVar6 = *(long *)(param_1 + 0x28);
    lVar7 = *(long *)(param_1 + 0x30);
  }
  uVar4 = FUN_03367708(lVar6,lVar7);
  if ((uVar4 & 1) == 0) {
    FUN_00b89b80(local_b8,"Invalid Chrome Web Store item ID",&local_40);
    FUN_0784a544(param_1,5,local_b8);
    if (cStack_a1 < '\0') {
      FUN_00005810(local_b8[0],CONCAT17(cStack_a1,local_a8) & 0x7fffffffffffffff);
    }
  }
  else {
    local_24 = 1;
    local_40 = 0;
    uStack_38 = 0;
    local_30 = 0;
    uVar4 = FUN_0784a1fc(param_1,&local_24,&local_40);
    if ((uVar4 & 1) == 0) {
      FUN_0784a544(param_1,local_24,&local_40);
    }
    else {
      FUN_0269eb80(local_b8);
      uVar5 = FUN_00008790(0xd0);
      thunk_FUN_06f2f4b8(uVar5,param_1,local_b8,param_1 + 0x28);
      lVar6 = *(long *)(param_1 + 0x78);
      *(undefined8 *)(param_1 + 0x78) = uVar5;
      if (lVar6 != 0) {
        uVar5 = thunk_FUN_06f2f56c();
        FUN_00005810(uVar5,0xd0);
      }
      FUN_0002822c(local_b8);
      uVar5 = *(undefined8 *)(param_1 + 0x78);
      lVar6 = FUN_0119426c(*(undefined8 *)(param_1 + 0x48));
      if (*(undefined8 **)(lVar6 + 0x278) == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x784a1d8);
        (*pcVar2)();
      }
      plVar8 = (long *)**(undefined8 **)(lVar6 + 0x278);
      if (plVar8 == (long *)0x0) {
        FUN_06f2f608(uVar5,0);
      }
      else {
        uVar1 = *(uint *)(plVar8 + 1);
        *(uint *)(plVar8 + 1) = uVar1 + 1;
        if (0xfffffffe < uVar1) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x784a1e4);
          (*pcVar2)();
        }
        FUN_06f2f608(uVar5,plVar8);
        FUN_003d2710(plVar8 + 1);
        if ((int)plVar8[1] == 0) {
          (**(code **)(*plVar8 + 8))(plVar8);
        }
      }
    }
  }
  return;
}


