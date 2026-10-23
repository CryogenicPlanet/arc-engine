// ==== 0x433e540  FUN_0433e540  (232 bytes)

void FUN_0433e540(undefined8 param_1,long *param_2)

{
  int iVar1;
  code *pcVar2;
  int iVar3;
  undefined8 uVar4;
  long lVar5;
  int *local_58;
  int *local_50 [4];
  undefined1 local_30;
  long local_28;
  
  lVar5 = *param_2;
  local_28 = 0;
  local_50[0] = (int *)((ulong)local_50[0] & 0xffffffffffffff00);
  local_30 = 0xff;
  FUN_008b337c(local_50,lVar5 + 0x58);
  FUN_011bfbf8(*(undefined8 *)(lVar5 + 0x368));
  uVar4 = FUN_072b3e20();
  FUN_00d13f00(&local_28,local_50,uVar4);
  FUN_00d13d5c(local_50);
  FUN_007b7670(&local_58,&local_28);
  FUN_04338f40(param_1,&local_58);
  if (local_58 != (int *)0x0) {
    iVar3 = *local_58;
    iVar1 = iVar3 + -1;
    *local_58 = iVar1;
    if (iVar3 == 0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x433e628);
      (*pcVar2)();
    }
    if (iVar1 == 0) {
      local_50[0] = local_58 + 2;
      FUN_005b6294(local_50,*(undefined8 *)(local_58 + 4));
      FUN_00005810(local_58,0x20);
    }
  }
  if (local_28 != 0) {
    LOAcquire();
    iVar3 = *(int *)(local_28 + 8);
    *(int *)(local_28 + 8) = iVar3 + -1;
    LORelease();
    if (iVar3 == 1) {
      FUN_005dfee0();
    }
  }
  return;
}


