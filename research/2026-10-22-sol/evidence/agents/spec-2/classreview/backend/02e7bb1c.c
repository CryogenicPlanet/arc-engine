// ==== 0x2e7bb1c  FUN_02e7bb1c  (192 bytes)

void FUN_02e7bb1c(undefined8 param_1,int *param_2)

{
  undefined8 *puVar1;
  code *pcVar2;
  int iVar3;
  ulong uVar4;
  undefined8 *local_28;
  
  uVar4 = *(ulong *)(DAT_0e481618 + 0x38);
  local_28 = (undefined8 *)FUN_00008790(0x10);
  *local_28 = &PTR_LAB_0db2ea90;
  if ((uVar4 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(uVar4);
  }
  local_28[1] = uVar4;
  if (param_2 != (int *)0x0) {
    (**(code **)(param_2 + 2))(param_2,&local_28,1);
    LOAcquire();
    iVar3 = *param_2;
    *param_2 = iVar3 + -1;
    LORelease();
    if (iVar3 == 1) {
      (**(code **)(param_2 + 4))(param_2);
    }
    puVar1 = local_28;
    local_28 = (undefined8 *)0x0;
    if (puVar1 != (undefined8 *)0x0) {
      FUN_03c2114c();
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(0,0x2e7bbc8);
  (*pcVar2)();
}


