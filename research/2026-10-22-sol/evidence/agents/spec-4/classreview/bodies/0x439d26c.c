// ==== 0x439d26c  FUN_0439d26c  (84 bytes)

void FUN_0439d26c(undefined8 *param_1,long param_2)

{
  long lVar1;
  undefined8 uStack_30;
  undefined1 uStack_28;
  undefined7 local_27;
  undefined1 uStack_20;
  undefined8 uStack_1f;
  undefined4 uStack_17;
  undefined3 uStack_13;
  
  if ((*(long *)(param_2 + 0x130) != 0) &&
     (lVar1 = *(long *)(*(long *)(param_2 + 0x130) + 0x8d8), lVar1 != 0)) {
    uStack_13 = 0;
    uStack_17 = 0;
    uStack_30 = *(undefined8 *)(lVar1 + 0x88);
    uStack_1f = *(undefined8 *)(lVar1 + 0x99);
    uStack_20 = (undefined1)((ulong)*(undefined8 *)(lVar1 + 0x91) >> 0x38);
    uStack_28 = (undefined1)*(undefined8 *)(lVar1 + 0x90);
    local_27 = (undefined7)((ulong)*(undefined8 *)(lVar1 + 0x90) >> 8);
    FUN_0266f700(&uStack_30);
    return;
  }
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  return;
}


