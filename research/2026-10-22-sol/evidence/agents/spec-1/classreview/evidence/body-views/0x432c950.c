// ==== 0x432c950  FUN_0432c950  (868 bytes)

void FUN_0432c950(undefined8 *param_1,long *param_2,long *param_3,long *param_4)

{
  undefined8 ******ppppppuVar1;
  ulong uVar2;
  byte bVar3;
  long *plVar4;
  code *pcVar5;
  int iVar6;
  undefined8 uVar7;
  undefined4 *puVar8;
  char *pcVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  long lVar12;
  long lVar13;
  int *piVar14;
  long lVar15;
  double dVar16;
  long local_b0 [2];
  undefined7 local_a0;
  char cStack_99;
  undefined8 local_90 [2];
  undefined7 local_80;
  char cStack_79;
  undefined8 local_78 [2];
  undefined7 local_68;
  char cStack_61;
  undefined8 *****local_60;
  long lStack_58;
  undefined8 local_50;
  
  lVar12 = *(long *)(*param_4 + 0x88);
  if (lVar12 == 0) {
    pcVar9 = "Supplied a headless browser context";
    uVar7 = 0xd;
    uVar10 = 0x23;
    uVar11 = 0x31;
  }
  else {
    iVar6 = FUN_0432c834();
    if (iVar6 == 0) {
      if (param_2 != param_3) {
        bVar3 = *(byte *)((long)param_3 + 0x17);
        if (*(char *)((long)param_2 + 0x17) < '\0') {
          uVar2 = param_3[1];
          plVar4 = (long *)*param_3;
          if (-1 < (char)bVar3) {
            uVar2 = (ulong)bVar3;
            plVar4 = param_3;
          }
          FUN_00359ad8(param_2,plVar4,uVar2);
        }
        else if ((char)bVar3 < '\0') {
          FUN_00359998(param_2,*param_3,param_3[1]);
        }
        else {
          lVar15 = param_3[1];
          lVar13 = *param_3;
          param_2[2] = param_3[2];
          param_2[1] = lVar15;
          *param_2 = lVar13;
        }
      }
      param_2[3] = param_3[3];
      lVar13 = param_2[0x10];
      if (*(char *)((long)param_3 + 0x17) < '\0') {
        FUN_00060a60(&local_60,*param_3,param_3[1]);
      }
      else {
        lStack_58 = param_3[1];
        local_60 = (undefined8 *****)*param_3;
        local_50 = param_3[2];
      }
      ppppppuVar1 = (undefined8 ******)local_60;
      if (-1 < (long)local_50._7_1_) {
        ppppppuVar1 = &local_60;
      }
      lVar15 = lStack_58;
      if (-1 < (long)local_50) {
        lVar15 = (long)local_50._7_1_;
      }
      FUN_008a7fc4(local_b0,ppppppuVar1,lVar15);
      FUN_0317ff94(local_78,DAT_0e4a2fd8);
      FUN_029d22d4(local_90,1);
      FUN_0119426c(lVar12);
      uVar7 = FUN_020d48f8();
      FUN_0be709b4(lVar13,local_b0,1,0,0xffffffffffffffff,local_78,local_90,uVar7);
      if (cStack_79 < '\0') {
        FUN_00005810(local_90[0],CONCAT17(cStack_79,local_80) & 0x7fffffffffffffff);
      }
      if (cStack_61 < '\0') {
        FUN_00005810(local_78[0],CONCAT17(cStack_61,local_68) & 0x7fffffffffffffff);
      }
      if (cStack_99 < '\0') {
        FUN_00005810(local_b0[0],CONCAT17(cStack_99,local_a0) & 0x7fffffffffffffff);
      }
      if ((long)local_50 < 0) {
        FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
      }
      local_60 = (undefined8 *****)0x0;
      lStack_58 = 0;
      local_50 = 0;
      FUN_079407f8(local_b0,lVar12);
      FUN_005a5a40(&local_60,"prerenderInfo",0xd,local_b0);
      FUN_005a58dc(local_b0);
      FUN_07940864(local_b0,lVar12);
      FUN_00996568(&local_60,"extensionInfo",0xd,local_b0);
      if (local_b0[0] != 0) {
        FUN_00996624(local_b0);
        FUN_00005810(local_b0[0],CONCAT17(cStack_99,local_a0) << 5);
      }
      FUN_00029e40(local_b0,"StartLogging","../../arc/browser/network_activity_monitor_impl.cc",0x56
                  );
      dVar16 = (double)param_2[3] * 1000000.0;
      lVar12 = 0;
      if (-9.223372036854776e+18 <= dVar16) {
        lVar12 = 0x7fffffffffffffff;
      }
      lVar13 = (long)dVar16;
      if (9.223372036854775e+18 < dVar16) {
        lVar13 = lVar12;
      }
      piVar14 = (int *)param_2[0x11];
      if ((piVar14 != (int *)0x0) && (iVar6 = *piVar14, *piVar14 = iVar6 + 1, iVar6 == 0x7fffffff))
      {
                    /* WARNING: Does not return */
        pcVar5 = (code *)SoftwareBreakpoint(0,0x432ccb4);
        (*pcVar5)();
      }
      lVar15 = param_2[0x12];
      puVar8 = (undefined4 *)FUN_00008790(0x58);
      uVar7 = local_50;
      lVar12 = lStack_58;
      *puVar8 = 1;
      *(undefined1 **)(puVar8 + 2) = &LAB_0432cd00;
      *(code **)(puVar8 + 4) = FUN_041be2d0;
      *(undefined1 **)(puVar8 + 6) = &LAB_040c0f80;
      *(code **)(puVar8 + 8) = FUN_0432ccbc;
      *(undefined8 *)(puVar8 + 10) = 0;
      *(int **)(puVar8 + 0xc) = piVar14;
      *(long *)(puVar8 + 0xe) = lVar15;
      *(undefined8 ******)(puVar8 + 0x10) = local_60;
      local_60 = (undefined8 *****)0x0;
      lStack_58 = 0;
      local_50 = 0;
      *(undefined8 *)(puVar8 + 0x14) = uVar7;
      *(long *)(puVar8 + 0x12) = lVar12;
      FUN_003b866c(param_2 + 4,local_b0,lVar13,puVar8);
      *param_1 = 1;
      FUN_005a58dc(&local_60);
      return;
    }
    pcVar9 = "The network logging is already in progress.";
    uVar7 = 0x39;
    uVar10 = 0x2b;
    uVar11 = 0x39;
  }
  uVar7 = FUN_04b37104(uVar7,pcVar9,uVar10,uVar11,
                       "../../arc/browser/network_activity_monitor_impl.cc");
  *param_1 = uVar7;
  return;
}


