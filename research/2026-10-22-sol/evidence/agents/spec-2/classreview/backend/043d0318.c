// ==== 0x43d0318  FUN_043d0318  (252 bytes)

undefined8 * FUN_043d0318(undefined8 *param_1)

{
  ulong uVar1;
  long *plVar2;
  byte bVar3;
  code *pcVar4;
  long lVar5;
  undefined8 uVar6;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  undefined8 local_38;
  undefined8 uStack_30;
  ulong local_28;
  
  *param_1 = &PTR_FUN_0da87230;
  param_1[2] = 0;
  param_1[1] = 0;
  param_1[4] = 0;
  param_1[3] = 0;
  param_1[6] = 0;
  param_1[5] = 0;
  local_38 = 0;
  uStack_30 = 0;
  local_28 = 0;
  local_50 = 0;
  uStack_48 = 0;
  local_40 = 0;
  lVar5 = FUN_00008790(0x68);
  FUN_043cd05c(lVar5,&local_38,&local_50);
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  if ((long)local_28 < 0) {
    FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
  }
  uVar6 = FUN_02967f00();
  bVar3 = *(byte *)(lVar5 + 0x1f);
  uVar1 = *(ulong *)(lVar5 + 0x10);
  if (-1 < (char)bVar3) {
    uVar1 = (ulong)bVar3;
  }
  if (uVar1 == 0x10) {
    plVar2 = (long *)*(long *)(lVar5 + 8);
    if (-1 < (char)bVar3) {
      plVar2 = (long *)(lVar5 + 8);
    }
    if (*plVar2 == 0x752d656d6f726863 && plVar2[1] == 0x646574737572746e) {
      FUN_01cae230(uVar6,lVar5);
      FUN_02968aa0(param_1);
      return param_1;
    }
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(0,0x43d0414);
  (*pcVar4)();
}


