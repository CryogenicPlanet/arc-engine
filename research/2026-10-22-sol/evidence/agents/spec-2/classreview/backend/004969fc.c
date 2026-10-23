// ==== 0x4969fc  FUN_004969fc  (144 bytes)

ulong FUN_004969fc(long param_1,ulong param_2)

{
  long *plVar1;
  ulong uVar2;
  
  for (plVar1 = *(long **)(param_1 + 8); plVar1 != (long *)0x0; plVar1 = (long *)*plVar1) {
    while ((ulong)plVar1[4] <= param_2) {
      if (param_2 == plVar1[4]) {
        if (plVar1 == (long *)(param_1 + 8)) goto LAB_00496a38;
        uVar2 = plVar1[5];
        if ((uVar2 & 0xfffffffc00000000) == DAT_0e2f2208) {
          FUN_0000ffa8(uVar2);
        }
        goto LAB_00496a64;
      }
      plVar1 = (long *)plVar1[1];
      if (plVar1 == (long *)0x0) goto LAB_00496a38;
    }
  }
LAB_00496a38:
  uVar2 = 0;
LAB_00496a64:
  if ((uVar2 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4(uVar2);
  }
  return uVar2;
}


