// ==== 0x4332374  FUN_04332374  (216 bytes)

void FUN_04332374(long *param_1)

{
  undefined8 *puVar1;
  long lVar2;
  code *pcVar3;
  undefined8 *puVar4;
  long lVar5;
  long local_48;
  long local_40;
  long lStack_38;
  
  local_48 = *param_1;
  lStack_38 = param_1[2];
  local_40 = param_1[1];
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  FUN_042bd840(&local_48);
  lVar2 = local_48;
  if (local_48 != 0) {
    if (local_40 * 0x18 != 0) {
      lVar5 = local_40 * -0x18;
      puVar1 = (undefined8 *)(local_40 * 0x18 + local_48);
      do {
        puVar4 = puVar1 + -3;
        if (puVar4 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
          pcVar3 = (code *)SoftwareBreakpoint(1,0x433244c);
          (*pcVar3)();
        }
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar4,puVar1[-1] & 0x7fffffffffffffff);
        }
        lVar5 = lVar5 + 0x18;
        puVar1 = puVar4;
      } while (lVar5 != 0);
    }
    local_40 = (lVar2 - local_48 >> 3) * -0x5555555555555555;
    FUN_00005810(local_48,lStack_38 * 0x18);
  }
  return;
}


