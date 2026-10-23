// ==== 0x7c7dca0  FUN_07c7dca0  (464 bytes)

void FUN_07c7dca0(long *param_1,undefined8 param_2)

{
  uint uVar1;
  code *pcVar2;
  long lVar3;
  undefined4 uVar4;
  int iVar5;
  long lVar6;
  undefined8 uVar7;
  long local_88 [12];
  long local_28;
  
  FUN_07c5a310();
  if ((*(byte *)((long)param_1 + 0x93) & 1) != 0) {
    return;
  }
  if ((*(byte *)((long)param_1 + 0x29) & 1) == 0) {
    FUN_07c7dad4(param_1,param_2);
  }
  uVar1 = *(uint *)((long)param_1 + 0x6c);
  if (uVar1 == 5) {
    *(undefined4 *)((long)param_1 + 0x6c) = 7;
    if ((char)param_1[0x7f] != '\0') {
      thunk_FUN_09e1eac0(param_1 + 0x71);
      *(undefined1 *)(param_1 + 0x7f) = 0;
    }
    (**(code **)(*param_1 + 0x80))(param_1);
    if ((char)param_1[0x11] != '\0') {
      local_88[1] = 1;
      lVar6 = param_1[0x10];
      param_1[0x10] = 0;
      lVar3 = FUN_00d5ff9c();
      *(undefined1 *)(lVar3 + 8) = 0;
      if (lVar6 == 0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x7c7de70);
        (*pcVar2)();
      }
      local_88[0] = 0;
      local_28 = lVar6;
      (**(code **)(lVar6 + 8))(lVar6);
      FUN_00d5ff9c(&local_28);
      FUN_00d5ff9c(local_88);
    }
    FUN_009de658(param_1 + 0x80);
    return;
  }
  iVar5 = (int)param_2;
  if (iVar5 == 3) {
    uVar7 = 2;
LAB_07c7ddc4:
    if (uVar1 - 2 < 2) {
      FUN_02abb830(local_88);
      local_28 = param_1[0xe];
      if (local_28 == 0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x7c7de88);
        (*pcVar2)();
      }
      param_1[0xe] = 0;
      (**(code **)(local_28 + 8))(local_28,uVar7,local_88);
      FUN_00d5ff9c(&local_28);
      thunk_FUN_0093fa9c(local_88);
    }
    else if (uVar1 < 2) {
      local_88[0] = param_1[0xf];
      if (local_88[0] == 0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x7c7de7c);
        (*pcVar2)();
      }
      param_1[0xf] = 0;
      (**(code **)(local_88[0] + 8))(local_88[0],uVar7);
      FUN_00d5ff9c(local_88);
    }
LAB_07c7ddfc:
    uVar4 = 7;
  }
  else {
    if (iVar5 == 2) {
      uVar7 = 1;
      goto LAB_07c7ddc4;
    }
    if ((iVar5 != 1) || (7 < uVar1)) goto LAB_07c7de04;
    if ((0x4cU >> (ulong)(uVar1 & 0x1f) & 1) != 0) goto LAB_07c7ddfc;
    if ((3U >> (ulong)(uVar1 & 0x1f) & 1) == 0) {
      if ((0x90U >> (ulong)(uVar1 & 0x1f) & 1) != 0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x7c7de64);
        (*pcVar2)();
      }
      goto LAB_07c7de04;
    }
    uVar4 = 6;
  }
  *(undefined4 *)((long)param_1 + 0x6c) = uVar4;
LAB_07c7de04:
  (**(code **)(*param_1 + 0x80))(param_1);
  return;
}


