// ==== 0x4377260  FUN_04377260  (656 bytes)

void FUN_04377260(long param_1,undefined8 *param_2,undefined8 *param_3)

{
  code *pcVar1;
  long lVar2;
  undefined4 *puVar3;
  undefined8 *puVar4;
  long *plVar5;
  long lVar6;
  long *plVar7;
  undefined8 *puVar8;
  long lVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  long local_b0;
  long local_a8;
  long local_a0;
  long local_98;
  long local_90;
  long local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  undefined1 local_68;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  
  if (*(long *)(param_1 + 0x130) != 0) {
    plVar5 = *(long **)(*(long *)(param_1 + 0x130) + 0x8d8);
    if (*(undefined1 **)(*plVar5 + 0xd0) == &LAB_04152228) {
      do {
        plVar7 = plVar5;
        plVar5 = (long *)plVar7[0x2f];
      } while ((long *)plVar7[0x2f] != (long *)0x0);
    }
    else {
      do {
        plVar7 = plVar5;
        plVar5 = (long *)plVar7[0x2f];
      } while ((long *)plVar7[0x2f] != (long *)0x0);
    }
    if ((*(byte *)(plVar7 + 0x219) & 1) == 0) {
LAB_043774cc:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x43774d0);
      (*pcVar1)();
    }
    lVar2 = FUN_07e9ab3c(plVar7[0x1de]);
    if (lVar2 != 0) {
      puVar3 = (undefined4 *)FUN_00008790(0x48);
      uVar10 = *param_3;
      uVar12 = param_3[3];
      uVar11 = param_3[2];
      *(undefined8 *)(puVar3 + 0xc) = param_3[1];
      *(undefined8 *)(puVar3 + 10) = uVar10;
      *puVar3 = 1;
      *(undefined1 **)(puVar3 + 2) = &LAB_0439e36c;
      *(code **)(puVar3 + 4) = FUN_042a34c0;
      *(code **)(puVar3 + 6) = FUN_00b77080;
      lVar6 = param_3[3];
      *(undefined8 *)(puVar3 + 0x10) = uVar12;
      *(undefined8 *)(puVar3 + 0xe) = uVar11;
      if (*(long *)(lVar6 + 8) != 0) {
        param_3[2] = 0;
        param_3[3] = &DAT_0c4b3500;
      }
      FUN_07892e44(lVar2,param_2,puVar3);
      return;
    }
    local_98 = 0;
    local_90 = 0;
    local_88 = 0;
    FUN_04377500(&local_98,param_2[1]);
    lVar2 = param_2[1] * 0x18;
    lVar6 = local_98;
    lVar9 = local_90;
    if (lVar2 != 0) {
      param_2 = (undefined8 *)*param_2;
      do {
        if (*(char *)((long)param_2 + 0x17) < '\0') {
          FUN_00060a60(&local_60,*param_2,param_2[1]);
        }
        else {
          uStack_58 = param_2[1];
          local_60 = *param_2;
          local_50 = param_2[2];
        }
        uStack_78 = uStack_58;
        local_80 = local_60;
        local_70 = local_50;
        local_68 = 0;
        if (local_90 == local_88) {
          puVar4 = (undefined8 *)FUN_0439e200(&local_98,&local_80);
        }
        else {
          puVar4 = (undefined8 *)(local_98 + local_90 * 0x20);
          if (puVar4 == (undefined8 *)0x0) goto LAB_043774cc;
          puVar4[1] = uStack_58;
          *puVar4 = local_60;
          puVar4[2] = local_50;
          uStack_78 = 0;
          local_70 = 0;
          local_80 = 0;
          *(undefined1 *)(puVar4 + 3) = 0;
          puVar4 = puVar4 + 4;
        }
        lVar6 = local_98;
        lVar9 = (long)puVar4 - local_98 >> 5;
        local_90 = lVar9;
        if ((long)puVar4 - local_98 == 0) goto LAB_043774cc;
        if ((long)local_70 < 0) {
          FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
        }
        param_2 = param_2 + 3;
        lVar2 = lVar2 + -0x18;
      } while (lVar2 != 0);
    }
    local_a0 = local_88;
    local_b0 = lVar6;
    local_a8 = lVar9;
    (*(code *)param_3[2])(param_3,&local_b0);
    lVar2 = local_b0;
    if (local_b0 != 0) {
      if (local_a8 * 0x20 != 0) {
        lVar6 = local_a8 * -0x20;
        puVar4 = (undefined8 *)(local_a8 * 0x20 + local_b0);
        do {
          puVar8 = puVar4 + -4;
          if (puVar8 == (undefined8 *)0x0) goto LAB_043774cc;
          if (*(char *)((long)puVar4 + -9) < '\0') {
            FUN_00005810(*puVar8,puVar4[-2] & 0x7fffffffffffffff);
          }
          lVar6 = lVar6 + 0x20;
          puVar4 = puVar8;
        } while (lVar6 != 0);
      }
      local_a8 = lVar2 - local_b0 >> 5;
      FUN_00005810(local_b0,local_a0 << 5);
    }
  }
  return;
}


