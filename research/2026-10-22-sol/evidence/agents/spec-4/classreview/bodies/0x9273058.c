// ==== 0x9273058  FUN_09273058  (184 bytes)

undefined4 * FUN_09273058(undefined4 *param_1,undefined8 *param_2)

{
  ulong uVar1;
  undefined8 *puVar2;
  long lVar3;
  char cVar4;
  code *pcVar5;
  undefined1 auStack_58 [32];
  undefined8 local_38;
  undefined8 uStack_30;
  ulong local_28;
  
  *(undefined1 *)(param_1 + 2) = 0;
  *(undefined1 *)(param_1 + 6) = 0;
  uVar1 = param_2[1];
  if ((*(byte *)((long)param_2 + 0x17) & 0x80) == 0) {
    uVar1 = (ulong)*(byte *)((long)param_2 + 0x17);
  }
  if (uVar1 == 0) {
    *param_1 = 0;
  }
  else {
    *param_1 = 1;
    local_38 = 0;
    uStack_30 = 0;
    local_28 = 0;
    cVar4 = *(char *)((long)param_2 + 0x17);
    puVar2 = (undefined8 *)*param_2;
    if (-1 < (long)cVar4) {
      puVar2 = param_2;
    }
    lVar3 = param_2[1];
    if (-1 < cVar4) {
      lVar3 = (long)cVar4;
    }
    thunk_FUN_00c87c40(auStack_58,puVar2,lVar3);
    FUN_0837a2dc(&local_38,auStack_58);
    FUN_02721bb0(auStack_58);
    if ((local_28 & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar5 = (code *)SoftwareBreakpoint(0,0x9273110);
      (*pcVar5)();
    }
    *(undefined8 *)(param_1 + 4) = uStack_30;
    *(undefined8 *)(param_1 + 2) = local_38;
    if ((*(byte *)(param_1 + 6) & 1) == 0) {
      *(undefined1 *)(param_1 + 6) = 1;
    }
  }
  return param_1;
}


