// ==== 0x434a700  FUN_0434a700  (2420 bytes)

/* WARNING: Removing unreachable block (ram,0x0434aac0) */

void FUN_0434a700(undefined8 *param_1,undefined8 *param_2)

{
  long *plVar1;
  ulong uVar2;
  long lVar3;
  byte bVar4;
  int iVar5;
  pid_t pVar6;
  undefined4 *puVar7;
  long lVar8;
  long *plVar9;
  undefined8 uVar10;
  code *pcVar11;
  long *plVar12;
  undefined8 *puVar13;
  long lVar14;
  undefined8 uVar15;
  undefined8 local_3c0;
  undefined8 uStack_3b8;
  undefined8 local_3b0;
  undefined8 uStack_3a8;
  undefined8 uStack_3a0;
  undefined8 uStack_398;
  undefined8 local_390;
  undefined8 uStack_388;
  undefined8 uStack_380;
  undefined8 uStack_378;
  undefined8 local_370;
  undefined8 uStack_368;
  undefined8 uStack_360;
  undefined8 *local_358;
  undefined8 local_350;
  undefined8 uStack_348;
  undefined8 local_340;
  undefined8 local_338;
  undefined8 local_330;
  undefined8 uStack_328;
  undefined8 local_320;
  undefined8 uStack_318;
  undefined8 local_310;
  undefined8 uStack_308;
  undefined8 local_300;
  undefined8 local_2f8;
  undefined8 local_2f0;
  undefined8 uStack_2e8;
  undefined8 uStack_2e0;
  undefined8 uStack_2d8;
  undefined8 local_2d0;
  undefined8 uStack_2c8;
  undefined8 uStack_2c0;
  undefined8 local_2b8;
  undefined8 local_2b0;
  undefined8 uStack_2a8;
  undefined8 uStack_2a0;
  undefined8 uStack_298;
  undefined8 local_290;
  long local_288 [9];
  char *local_240;
  undefined8 uStack_238;
  code *local_230;
  undefined *local_228;
  undefined8 local_220;
  undefined8 uStack_218;
  undefined8 uStack_210;
  undefined8 uStack_208;
  undefined8 local_200;
  undefined8 uStack_1f8;
  undefined8 uStack_1f0;
  undefined8 uStack_1e8;
  undefined8 local_1e0;
  undefined8 uStack_1d8;
  undefined8 uStack_1d0;
  undefined8 uStack_1c8;
  undefined8 local_1c0;
  undefined8 uStack_1b8;
  undefined8 uStack_1b0;
  undefined8 uStack_1a8;
  undefined8 local_1a0;
  undefined8 uStack_198;
  undefined8 uStack_190;
  undefined8 uStack_188;
  undefined8 local_180;
  undefined8 uStack_178;
  undefined8 uStack_170;
  undefined8 uStack_168;
  undefined8 local_160;
  undefined8 uStack_158;
  undefined8 uStack_150;
  undefined8 uStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  undefined8 uStack_130;
  undefined8 uStack_128;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 uStack_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  long *local_78;
  undefined8 uStack_70;
  undefined1 *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  iVar5 = _pthread_mutex_trylock((pthread_mutex_t *)(param_1 + 0x18));
  if (iVar5 != 0) {
    FUN_002b39c8(param_1 + 0x18,&PTR_s_BaseLock_0df6cbd8);
  }
  lVar14 = param_1[0x17];
  _pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x18));
  if (lVar14 != 0) goto LAB_0434a750;
  iVar5 = _pthread_mutex_trylock((pthread_mutex_t *)(param_1 + 0x18));
  if (iVar5 != 0) {
    FUN_002b39c8(param_1 + 0x18,&PTR_s_BaseLock_0df6cbd8);
  }
  if (param_1 != param_2) {
    bVar4 = *(byte *)((long)param_2 + 0x17);
    if (*(char *)((long)param_1 + 0x17) < '\0') {
      uVar2 = param_2[1];
      puVar13 = (undefined8 *)*param_2;
      if (-1 < (char)bVar4) {
        uVar2 = (ulong)bVar4;
        puVar13 = param_2;
      }
      FUN_00359ad8(param_1,puVar13,uVar2);
    }
    else if ((char)bVar4 < '\0') {
      FUN_00359998(param_1,*param_2,param_2[1]);
    }
    else {
      uVar15 = param_2[1];
      uVar10 = *param_2;
      param_1[2] = param_2[2];
      param_1[1] = uVar15;
      *param_1 = uVar10;
    }
  }
  uVar10 = param_2[3];
  *(undefined1 *)(param_1 + 4) = *(undefined1 *)(param_2 + 4);
  param_1[3] = uVar10;
  uVar10 = FUN_03005724(PTR_PTR_0e2b8290,4,0);
  puVar13 = (undefined8 *)param_1[0x17];
  param_1[0x17] = uVar10;
  if (puVar13 != (undefined8 *)0x0) {
    *puVar13 = &PTR_LAB_0d845290;
    local_240 = (char *)puVar13[1];
    uStack_238 = puVar13[2];
    local_230 = FUN_040491a4;
    local_228 = &DAT_0c4b4f30;
    (**(code **)(**(long **)(local_240 + 0x18) + 0x10))(*(long **)(local_240 + 0x18),&local_240);
    if (*(code **)(local_228 + 8) != (code *)0x0) {
      (**(code **)(local_228 + 8))(local_240);
    }
    FUN_00005810(puVar13,0x20);
  }
  local_288[6] = 0;
  local_288[7] = 0;
  local_288[8] = 0;
  local_240 = (char *)CONCAT71(local_240._1_7_,1);
  local_228 = (undefined *)CONCAT71(local_228._1_7_,1);
  FUN_0053a240(local_288 + 6,"enable_systrace",0xf,&local_240);
  if (((ulong)local_228 & 0xff) != 0xff) {
    (*(code *)(&PTR__cxx_destruct_0df6d258)[(ulong)local_228 & 0xff])(&local_3c0,&local_240);
  }
  local_288[3] = 0;
  local_288[4] = 0;
  local_288[5] = 0;
  local_240 = "disabled-by-default-system_power";
  FUN_014bbb3c(local_288 + 3,&local_240);
  local_240 = "disabled-by-default-system_stats";
  FUN_014bbb3c(local_288 + 3,&local_240);
  FUN_00996568(local_288 + 6,"included_categories",0x13,local_288 + 3);
  local_288[0] = 0;
  local_288[1] = 0;
  local_288[2] = 0;
  local_240 = "Memory.Browser.PrivateMemoryFootprint";
  FUN_014bbb3c(local_288,&local_240);
  local_240 = "Memory.Gpu.PrivateMemoryFootprint";
  FUN_014bbb3c(local_288,&local_240);
  local_240 = "Memory.Renderer.PrivateMemoryFootprint";
  FUN_014bbb3c(local_288,&local_240);
  local_240 = "Memory.Total.PrivateMemoryFootprint";
  FUN_014bbb3c(local_288,&local_240);
  local_240 = "Memory.PressureLevel2";
  FUN_014bbb3c(local_288,&local_240);
  local_240 = "PerformanceMonitor.AverageCPU10.BrowserProcess";
  FUN_014bbb3c(local_288,&local_240);
  local_240 = "PerformanceMonitor.AverageCPU10.RendererProcess";
  FUN_014bbb3c(local_288,&local_240);
  local_240 = "PerformanceMonitor.AverageCPU10.Total";
  FUN_014bbb3c(local_288,&local_240);
  FUN_00996568(local_288 + 6,"histogram_names",0xf,local_288);
  local_358 = &local_350;
  uStack_2e0 = 0;
  uStack_348 = 0;
  local_350 = 0;
  uStack_368 = 0;
  local_370 = 0;
  uStack_360 = 0;
  uStack_388 = 0;
  local_390 = 0;
  uStack_378 = 0;
  uStack_380 = 0;
  uStack_3a8 = 0;
  local_3b0 = 0;
  uStack_398 = 0;
  uStack_3a0 = 0;
  uStack_3b8 = 0;
  local_3c0 = 0;
  local_338 = 0;
  local_330 = 0;
  local_340 = 0;
  uStack_328 = 0x400;
  uStack_318 = 0;
  local_320 = 0;
  uStack_308 = 0;
  local_310 = 0;
  local_300 = 0x3f800000;
  local_2f0 = 0;
  uStack_2e8 = 0;
  local_2f8 = 0;
  local_2d0 = 0;
  uStack_2d8 = 0;
  uStack_2c0 = 0;
  uStack_2c8 = 0;
  local_2b8 = 0x3f800000;
  uStack_2a8 = 0;
  local_2b0 = 0;
  uStack_298 = 0;
  uStack_2a0 = 0;
  local_290 = 0x3f800000;
  FUN_03419c3c(&local_3c0,local_288 + 6);
  local_80 = 0;
  uStack_98 = 0;
  local_a0 = 0;
  uStack_88 = 0;
  uStack_90 = 0;
  uStack_b8 = 0;
  local_c0 = 0;
  uStack_a8 = 0;
  uStack_b0 = 0;
  uStack_d8 = 0;
  local_e0 = 0;
  uStack_c8 = 0;
  uStack_d0 = 0;
  uStack_f8 = 0;
  local_100 = 0;
  uStack_e8 = 0;
  uStack_f0 = 0;
  uStack_118 = 0;
  local_120 = 0;
  uStack_108 = 0;
  uStack_110 = 0;
  uStack_138 = 0;
  local_140 = 0;
  uStack_128 = 0;
  uStack_130 = 0;
  uStack_158 = 0;
  local_160 = 0;
  uStack_148 = 0;
  uStack_150 = 0;
  uStack_178 = 0;
  local_180 = 0;
  uStack_168 = 0;
  uStack_170 = 0;
  uStack_198 = 0;
  local_1a0 = 0;
  uStack_188 = 0;
  uStack_190 = 0;
  uStack_1b8 = 0;
  local_1c0 = 0;
  uStack_1a8 = 0;
  uStack_1b0 = 0;
  uStack_1d8 = 0;
  local_1e0 = 0;
  uStack_1c8 = 0;
  uStack_1d0 = 0;
  uStack_1f8 = 0;
  local_200 = 0;
  uStack_1e8 = 0;
  uStack_1f0 = 0;
  uStack_218 = 0;
  local_220 = 0;
  uStack_208 = 0;
  uStack_210 = 0;
  uStack_238 = 0;
  local_240 = (char *)0x0;
  local_228 = (undefined *)0x0;
  local_230 = (code *)0x0;
  local_68 = (undefined1 *)((ulong)local_68 & 0xffffffffffffff);
  local_78 = (long *)((ulong)local_78 & 0xffffffffffffff00);
  FUN_02c979a0(&local_240,&local_3c0,*(undefined1 *)(param_2 + 4),0,&local_78);
  FUN_036c0f4c(param_1[0x17],&local_240,0xffffffff);
  lVar8 = *(long *)(param_1[0x17] + 8);
  lVar3 = *(long *)(param_1[0x17] + 0x10);
  plVar12 = *(long **)(lVar8 + 0x18);
  local_68 = &LAB_04049374;
  local_60 = &PTR_FUN_0ddd0640;
  local_78 = (long *)FUN_00008790(0x30);
  *local_78 = lVar8;
  local_78[1] = lVar3;
  local_78[2] = (long)param_1;
  local_78[4] = (long)&LAB_0434cf44;
  local_78[5] = (long)&DAT_0c9d70c0;
  (**(code **)(*plVar12 + 0x10))(plVar12,&local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78);
  }
  lVar8 = *(long *)(param_1[0x17] + 8);
  lVar3 = *(long *)(param_1[0x17] + 0x10);
  plVar12 = *(long **)(lVar8 + 0x18);
  local_68 = &LAB_04049574;
  local_60 = &PTR_FUN_0ddd0680;
  local_78 = (long *)FUN_00008790(0x30);
  *local_78 = lVar8;
  local_78[1] = lVar3;
  local_78[2] = (long)param_1;
  local_78[4] = (long)FUN_0434d078;
  local_78[5] = (long)&DAT_0c9d70e0;
  (**(code **)(*plVar12 + 0x10))(plVar12,&local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78);
  }
  local_78 = *(long **)(param_1[0x17] + 8);
  uStack_70 = *(undefined8 *)(param_1[0x17] + 0x10);
  local_68 = &LAB_03957a48;
  local_60 = (undefined **)&DAT_0c4b4f50;
  (**(code **)(*(long *)local_78[3] + 0x10))((long *)local_78[3],&local_78);
  if (*(code **)((long)local_60 + 8) != (code *)0x0) {
    (**(code **)((long)local_60 + 8))(local_78);
  }
  plVar12 = (long *)FUN_00008790(0x60);
  plVar12[2] = 0;
  plVar12[3] = 0;
  *(undefined4 *)(plVar12 + 4) = 0xffffffff;
  plVar12[6] = 0;
  plVar12[5] = 0;
  plVar12[8] = 0;
  plVar12[7] = 0;
  plVar12[10] = 0;
  plVar12[9] = 0;
  *plVar12 = (long)&PTR_FUN_0da8bc48;
  plVar12[1] = 0;
  plVar12[0xb] = 0;
  FUN_00029e40(&local_78,"StartCollectingMemoryPressureValue",
               "../../arc/browser/tracing_controller_impl.cc",0x5a);
  puVar7 = (undefined4 *)FUN_00008790(0x28);
  *puVar7 = 1;
  *(undefined1 **)(puVar7 + 2) = &LAB_0434be18;
  *(code **)(puVar7 + 4) = FUN_03954600;
  *(code **)(puVar7 + 6) = FUN_00b77080;
  FUN_0185fa3c(plVar12,&local_78,500000,puVar7);
  local_78 = plVar12;
  FUN_0434bd00(param_1 + 0x20,&local_78);
  if (local_78 != (long *)0x0) {
    FUN_023fbaac();
  }
  plVar12 = (long *)FUN_00008790(0x60);
  plVar12[2] = 0;
  plVar12[3] = 0;
  *(undefined4 *)(plVar12 + 4) = 0xffffffff;
  plVar12[6] = 0;
  plVar12[5] = 0;
  plVar12[8] = 0;
  plVar12[7] = 0;
  plVar12[10] = 0;
  plVar12[9] = 0;
  *plVar12 = (long)&PTR_FUN_0da8bc48;
  plVar12[1] = 0;
  plVar12[0xb] = 0;
  FUN_00029e40(&local_78,"StartCollectingSystemMetrics",
               "../../arc/browser/tracing_controller_impl.cc",0x68);
  puVar7 = (undefined4 *)FUN_00008790(0x28);
  *puVar7 = 1;
  *(undefined1 **)(puVar7 + 2) = &LAB_0434bfe4;
  *(code **)(puVar7 + 4) = FUN_03954600;
  *(code **)(puVar7 + 6) = FUN_00b77080;
  FUN_0185fa3c(plVar12,&local_78,1000000,puVar7);
  local_78 = plVar12;
  FUN_0434bd00(param_1 + 0x20,&local_78);
  if (local_78 != (long *)0x0) {
    FUN_023fbaac();
  }
  plVar12 = (long *)FUN_00008790(0x60);
  plVar12[2] = 0;
  plVar12[3] = 0;
  *(undefined4 *)(plVar12 + 4) = 0xffffffff;
  plVar12[6] = 0;
  plVar12[5] = 0;
  plVar12[8] = 0;
  plVar12[7] = 0;
  plVar12[10] = 0;
  plVar12[9] = 0;
  *plVar12 = (long)&PTR_FUN_0da8bc48;
  plVar12[1] = 0;
  plVar12[0xb] = 0;
  FUN_00029e40(&local_78,"StartCollectingSwapMetrics","../../arc/browser/tracing_controller_impl.cc"
               ,0x85);
  puVar7 = (undefined4 *)FUN_00008790(0x28);
  *puVar7 = 1;
  *(undefined1 **)(puVar7 + 2) = &LAB_0434c3b8;
  *(code **)(puVar7 + 4) = FUN_03954600;
  *(code **)(puVar7 + 6) = FUN_00b77080;
  FUN_0185fa3c(plVar12,&local_78,5000000,puVar7);
  local_78 = plVar12;
  FUN_0434bd00(param_1 + 0x20,&local_78);
  if (local_78 != (long *)0x0) {
    FUN_023fbaac();
  }
  plVar12 = (long *)FUN_00008790(0x60);
  plVar12[2] = 0;
  plVar12[3] = 0;
  *(undefined4 *)(plVar12 + 4) = 0xffffffff;
  plVar12[6] = 0;
  plVar12[5] = 0;
  plVar12[8] = 0;
  plVar12[7] = 0;
  plVar12[10] = 0;
  plVar12[9] = 0;
  *plVar12 = (long)&PTR_FUN_0da8bc48;
  plVar12[1] = 0;
  plVar12[0xb] = 0;
  pVar6 = _getpid();
  lVar8 = FUN_03b61c44(pVar6,0);
  if (lVar8 == 0) {
    plVar9 = (long *)0x0;
  }
  else {
    plVar9 = (long *)FUN_00008790(0x20);
    *plVar9 = (long)&PTR_viewWillStartLiveResize_0da7f948;
    plVar9[1] = 0;
    plVar9[2] = 0;
    plVar9[3] = lVar8;
  }
  FUN_0273c590(&local_78,lVar8);
  FUN_00029e40(&local_78,"StartCollectingBrowserCpuUsage",
               "../../arc/browser/tracing_controller_impl.cc",0xb5);
  puVar7 = (undefined4 *)FUN_00008790(0x38);
  *puVar7 = 1;
  *(undefined1 **)(puVar7 + 2) = &LAB_0434c588;
  *(code **)(puVar7 + 4) = FUN_0434ce74;
  *(code **)(puVar7 + 6) = FUN_00b77080;
  *(long *)(puVar7 + 10) = lVar8;
  *(long **)(puVar7 + 0xc) = plVar9;
  if (plVar9 == (long *)0x0) {
    FUN_0185fa3c(plVar12,&local_78,500000,puVar7);
  }
  else {
    plVar1 = plVar9 + 1;
    lVar8 = *plVar1;
    *plVar1 = lVar8 + 1;
    FUN_0185fa3c(lVar8,plVar12,&local_78,500000,puVar7);
    LOAcquire();
    lVar8 = *plVar1;
    *plVar1 = lVar8 + -1;
    LORelease();
    if (lVar8 == 0) {
      (**(code **)(*plVar9 + 0x10))(plVar9);
      if (plVar9[2] != 0) {
        LOAcquire();
        lVar8 = plVar9[2];
        plVar9[2] = lVar8 + -1;
        LORelease();
        if (lVar8 != 0) goto LAB_0434af44;
      }
      pcVar11 = *(code **)(*plVar9 + 0x20);
      if (((pcVar11 == FUN_00f01bc0) || (pcVar11 == FUN_00f01bc0)) || (pcVar11 == FUN_013efb40)) {
        FUN_00005810(plVar9,0x20);
      }
      else {
        (*pcVar11)(plVar9);
      }
    }
  }
LAB_0434af44:
  local_78 = plVar12;
  FUN_0434bd00(param_1 + 0x20,&local_78);
  if (local_78 != (long *)0x0) {
    FUN_023fbaac();
  }
  FUN_01dfc51c(&local_240);
  FUN_02b885f0(&local_3c0);
  if (local_288[0] != 0) {
    FUN_00996624(local_288);
    FUN_00005810(local_288[0],local_288[2] << 5);
  }
  if (local_288[3] != 0) {
    FUN_00996624(local_288 + 3);
    FUN_00005810(local_288[3],local_288[5] << 5);
  }
  FUN_005a58dc(local_288 + 6);
  _pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x18));
LAB_0434a750:
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(lVar14 == 0);
}


