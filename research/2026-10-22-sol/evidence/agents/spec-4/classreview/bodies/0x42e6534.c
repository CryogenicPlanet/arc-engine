// ==== 0x42e6534  FUN_042e6534  (192 bytes)

void FUN_042e6534(long param_1,undefined8 param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined4 *local_48;
  ulong local_40;
  long lStack_38;
  
  lVar4 = *(long *)(param_1 + 0x68);
  if (lVar4 != 0) {
    uVar2 = FUN_0238b08c();
    lVar4 = FUN_0009278c(uVar2,lVar4,1);
    if (lVar4 != 0) {
      local_48 = (undefined4 *)FUN_00008790(4);
      *local_48 = 1;
      lStack_38 = 1;
      local_40 = 1;
      FUN_06afd530(lVar4,param_2,&local_48);
      if (local_48 != (undefined4 *)0x0) {
        if ((local_40 & 0x3fffffffffffffff) != 0) {
          lVar4 = local_40 * -4;
          puVar3 = local_48 + local_40;
          do {
            puVar3 = puVar3 + -1;
            if (puVar3 == (undefined4 *)0x0) {
                    /* WARNING: Does not return */
              pcVar1 = (code *)SoftwareBreakpoint(1,0x42e65f4);
              (*pcVar1)();
            }
            lVar4 = lVar4 + 4;
          } while (lVar4 != 0);
        }
        local_40 = 0;
        FUN_00005810(local_48,lStack_38 << 2);
      }
    }
  }
  return;
}


