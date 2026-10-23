// ==== 0x836a724  FUN_0836a724  (520 bytes)

void FUN_0836a724(long *param_1)

{
  code *pcVar1;
  ulong uVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  ulong uVar5;
  long lVar6;
  undefined4 *puVar7;
  undefined4 uVar8;
  long lVar9;
  long lVar10;
  undefined4 local_bc;
  ulong local_b8;
  long lStack_b0;
  long local_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  local_b8 = 0;
  lStack_b0 = 0;
  local_a8 = 0;
  FUN_0242e56c();
  FUN_064eff98(&local_b8);
  if (local_b8 + lStack_b0 * 0x20 < local_b8) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x836a92c);
    (*pcVar1)();
  }
  lVar9 = lStack_b0 << 5;
  if (lVar9 != 0) {
    lVar6 = 0;
    lVar10 = 0;
    uVar5 = local_b8;
    do {
      if ((uVar5 != 0) && (*(char *)(uVar5 + 0x18) == '\x06')) {
        uVar2 = FUN_01443580(uVar5,"level",5);
        puVar3 = (undefined8 *)FUN_02ae8880(uVar5,"header",6);
        puVar4 = (undefined8 *)FUN_02ae8880(uVar5,"message",7);
        if ((puVar4 != (undefined8 *)0x0 && puVar3 != (undefined8 *)0x0) &&
            (uVar2 & 0x100000000) != 0) {
          uVar8 = (undefined4)uVar2;
          local_bc = uVar8;
          if (lVar6 == param_1[2]) {
            puVar7 = (undefined4 *)FUN_042c014c(param_1,&local_bc,puVar3,puVar4);
          }
          else {
            puVar7 = (undefined4 *)(lVar10 + lVar6 * 0x38);
            if (puVar7 == (undefined4 *)0x0) goto LAB_0836a90c;
            if (*(char *)((long)puVar3 + 0x17) < '\0') {
              thunk_FUN_00060a60(&local_80,*puVar3,puVar3[1]);
            }
            else {
              uStack_78 = puVar3[1];
              local_80 = *puVar3;
              local_70 = puVar3[2];
            }
            if (*(char *)((long)puVar4 + 0x17) < '\0') {
              thunk_FUN_00060a60(&local_a0,*puVar4,puVar4[1]);
            }
            else {
              uStack_98 = puVar4[1];
              local_a0 = *puVar4;
              local_90 = puVar4[2];
            }
            *puVar7 = uVar8;
            *(undefined8 *)(puVar7 + 6) = local_70;
            *(undefined8 *)(puVar7 + 4) = uStack_78;
            *(undefined8 *)(puVar7 + 2) = local_80;
            *(undefined8 *)(puVar7 + 0xc) = local_90;
            *(undefined8 *)(puVar7 + 10) = uStack_98;
            *(undefined8 *)(puVar7 + 8) = local_a0;
            puVar7 = puVar7 + 0xe;
          }
          lVar10 = *param_1;
          lVar6 = ((long)puVar7 - lVar10 >> 3) * 0x6db6db6db6db6db7;
          param_1[1] = lVar6;
          if ((long)puVar7 - lVar10 == 0) {
LAB_0836a90c:
                    /* WARNING: Does not return */
            pcVar1 = (code *)SoftwareBreakpoint(1,0x836a910);
            (*pcVar1)();
          }
        }
      }
      uVar5 = uVar5 + 0x20;
      lVar9 = lVar9 + -0x20;
    } while (lVar9 != 0);
  }
  if (local_b8 != 0) {
    FUN_00996624(&local_b8,local_b8);
    thunk_FUN_00005810(local_b8,local_a8 << 5);
  }
  return;
}


