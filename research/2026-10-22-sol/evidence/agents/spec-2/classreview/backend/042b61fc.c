// ==== 0x42b61fc  FUN_042b61fc  (408 bytes)

void FUN_042b61fc(long *param_1)

{
  code *pcVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  undefined8 *local_a0;
  ulong local_98;
  long local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  ulong local_78;
  undefined8 local_70;
  undefined8 uStack_68;
  ulong local_60;
  undefined4 local_58;
  
  FUN_01a29860(DAT_0e481618);
  param_1[1] = 0;
  param_1[2] = 0;
  *param_1 = 0;
  FUN_01a298f8();
  FUN_01a2af78(&local_a0);
  lVar4 = local_98 << 3;
  if (lVar4 != 0) {
    lVar6 = 0;
    lVar5 = 0;
    puVar3 = local_a0;
    do {
      FUN_042bc344(&local_88,*puVar3);
      if (lVar6 == param_1[2]) {
        puVar2 = (undefined8 *)FUN_042bf6a8(param_1,&local_88);
      }
      else {
        puVar2 = (undefined8 *)(lVar5 + lVar6 * 0x38);
        if (puVar2 == (undefined8 *)0x0) goto LAB_042b6380;
        puVar2[2] = local_78;
        puVar2[1] = uStack_80;
        *puVar2 = local_88;
        uStack_80 = 0;
        local_78 = 0;
        local_88 = 0;
        puVar2[5] = local_60;
        puVar2[4] = uStack_68;
        puVar2[3] = local_70;
        uStack_68 = 0;
        local_60 = 0;
        local_70 = 0;
        *(undefined4 *)(puVar2 + 6) = local_58;
        puVar2 = puVar2 + 7;
      }
      lVar5 = *param_1;
      lVar6 = ((long)puVar2 - lVar5 >> 3) * 0x6db6db6db6db6db7;
      param_1[1] = lVar6;
      if ((long)puVar2 - lVar5 == 0) goto LAB_042b6380;
      if ((long)local_60 < 0) {
        FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
      }
      if ((long)local_78 < 0) {
        FUN_00005810(local_88,local_78 & 0x7fffffffffffffff);
      }
      puVar3 = puVar3 + 1;
      lVar4 = lVar4 + -8;
    } while (lVar4 != 0);
  }
  if (local_a0 != (undefined8 *)0x0) {
    if ((local_98 & 0x1fffffffffffffff) != 0) {
      lVar4 = local_98 * -8;
      puVar3 = local_a0 + local_98;
      do {
        puVar3 = puVar3 + -1;
        if (puVar3 == (undefined8 *)0x0) {
LAB_042b6380:
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x42b6384);
          (*pcVar1)();
        }
        lVar4 = lVar4 + 8;
      } while (lVar4 != 0);
    }
    local_98 = 0;
    FUN_00005810(local_a0,local_90 << 3);
  }
  return;
}


