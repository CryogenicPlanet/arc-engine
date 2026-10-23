// ==== 0x42efb64  FUN_042efb64  (680 bytes)

/* WARNING: Removing unreachable block (ram,0x042efde4) */

void FUN_042efb64(long *param_1,long param_2)

{
  long lVar1;
  code *pcVar2;
  int iVar3;
  undefined8 *puVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  undefined8 local_120;
  undefined8 uStack_118;
  ulong local_110;
  undefined8 local_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  ulong local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 local_78;
  long local_68;
  long local_60;
  long local_58;
  
  lVar5 = *(long *)(param_2 + 0x28);
  if (lVar5 != 0) {
    if (((DAT_0e49a148 & 1) == 0) && (iVar3 = thunk_FUN_0c44b610(&DAT_0e49a148), iVar3 != 0)) {
      FUN_02683f70(&DAT_0e49a0f0);
      thunk_FUN_0c44b760(&DAT_0e49a148);
    }
    lVar5 = FUN_0009278c(&DAT_0e49a0f0,lVar5,1);
    if (lVar5 != 0) {
      local_68 = 0;
      local_60 = 0;
      local_58 = 0;
      FUN_06f26c80(&local_68,lVar5,1);
      *param_1 = 0;
      param_1[1] = 0;
      param_1[2] = 0;
      if ((local_60 * 9 & 0xfffffffffffffffU) != 0) {
        lVar6 = local_68 + local_60 * 0x90;
        lVar5 = local_68;
        do {
          uStack_88 = 0;
          local_90 = 0;
          local_78 = 0;
          uStack_80 = 0;
          uStack_a8 = 0;
          local_b0 = 0;
          uStack_98 = 0;
          uStack_a0 = 0;
          uStack_c8 = 0;
          local_d0 = 0;
          uStack_b8 = 0;
          uStack_c0 = 0;
          uStack_e8 = 0;
          local_f0 = 0;
          uStack_d8 = 0;
          uStack_e0 = 0;
          uStack_f8 = 0;
          local_100 = 0;
          FUN_000609a0(&local_100,lVar5);
          uStack_80 = *(undefined8 *)(lVar5 + 0x80);
          uStack_88 = *(undefined8 *)(lVar5 + 0x78);
          local_78 = *(undefined8 *)(lVar5 + 0x88);
          puVar4 = (undefined8 *)FUN_007cf864(&local_100);
          if (*(char *)((long)puVar4 + 0x17) < '\0') {
            FUN_00060a60(&local_120,*puVar4,puVar4[1]);
          }
          else {
            uStack_118 = puVar4[1];
            local_120 = *puVar4;
            local_110 = puVar4[2];
          }
          local_108 = uStack_88;
          if (param_1[1] == param_1[2]) {
            puVar4 = (undefined8 *)FUN_042efed8(param_1,&local_120);
          }
          else {
            puVar4 = (undefined8 *)(*param_1 + param_1[1] * 0x20);
            if (puVar4 == (undefined8 *)0x0) goto LAB_042efde0;
            puVar4[2] = local_110;
            puVar4[1] = uStack_118;
            *puVar4 = local_120;
            uStack_118 = 0;
            local_110 = 0;
            local_120 = 0;
            puVar4[3] = uStack_88;
            puVar4 = puVar4 + 4;
          }
          param_1[1] = (long)puVar4 - *param_1 >> 5;
          if ((long)puVar4 - *param_1 == 0) goto LAB_042efde0;
          if ((long)local_110 < 0) {
            FUN_00005810(local_120,local_110 & 0x7fffffffffffffff);
          }
          FUN_000282bc(&local_90,0);
          FUN_0002827c(&uStack_e0);
          if ((long)local_f0 < 0) {
            FUN_00005810(local_100,local_f0 & 0x7fffffffffffffff);
          }
          lVar5 = lVar5 + 0x90;
        } while (lVar5 != lVar6);
      }
      lVar5 = local_68;
      if (local_68 == 0) {
        return;
      }
      lVar6 = local_60 * 0x90;
      if (lVar6 != 0) {
        lVar7 = 0;
        lVar8 = local_68;
        do {
          lVar1 = lVar8 + lVar6;
          if (lVar1 == 0x90) {
LAB_042efde0:
                    /* WARNING: Does not return */
            pcVar2 = (code *)SoftwareBreakpoint(1,0x42efde4);
            (*pcVar2)();
          }
          FUN_000282bc(lVar1 + -0x20,0);
          FUN_0002827c(lVar1 + -0x70);
          lVar8 = lVar8 + -0x90;
          lVar7 = lVar7 + 0x90;
        } while (lVar6 - lVar7 != 0);
      }
      local_60 = (lVar5 - local_68 >> 4) * -0x71c71c71c71c71c7;
      FUN_00005810(local_68,local_58 * 0x90);
      return;
    }
  }
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  return;
}


