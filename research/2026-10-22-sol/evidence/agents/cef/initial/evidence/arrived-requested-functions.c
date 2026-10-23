/* SOURCE /home/exedev/work/inputs/decompile/0x40.c:79668 sha256 74748e88c7fdca8d2b1b6344f97e3e28e17602d93b27306f9f6ebf1d2b9ef893 */
// ==== 0x40422b0  objc::Categories::ArcBrowserContextDownloads_Extensions_::startDownload:error:  (276 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContextDownloads_Extensions_::startDownload_error_
               (ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  code *pcVar1;
  uint uVar2;
  ID IVar3;
  uint *puVar4;
  uint *local_70;
  undefined8 local_68 [2];
  undefined7 local_58;
  char cStack_51;
  undefined1 auStack_50 [16];
  byte local_40;
  uint *local_38;
  
  local_38 = (uint *)0x0;
  _objc_msgSend(param_1,"browser_context_downloads",auStack_50);
  if ((local_40 & 1) != 0) {
    IVar3 = _objc_msgSend(param_3,"absoluteString");
    FUN_03b60ee8(local_68,IVar3);
    FUN_043328cc(&local_38,auStack_50,local_68);
    if (cStack_51 < '\0') {
      FUN_00005810(local_68[0],CONCAT17(cStack_51,local_58) & 0x7fffffffffffffff);
    }
    if (local_40 != 0) {
      FUN_04332750(auStack_50);
    }
    if (local_38 != (uint *)((long)&MACH_HEADER.magic + 1) && param_4 != (ID *)0x0) {
      local_70 = local_38;
      puVar4 = local_38;
      if (((ulong)local_38 & 1) == 0) {
        uVar2 = *local_38;
        *local_38 = uVar2 + 1;
        puVar4 = (uint *)(ulong)uVar2;
      }
      IVar3 = FUN_044025f4(puVar4,&local_70);
      *param_4 = IVar3;
      if (((ulong)local_70 & 1) == 0) {
        FUN_04b35898();
      }
    }
    if (((ulong)local_38 & 1) == 0) {
      FUN_04b35898();
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x40423c4);
  (*pcVar1)();
}



/* SOURCE /home/exedev/work/inputs/decompile/0x40.c:80076 sha256 74748e88c7fdca8d2b1b6344f97e3e28e17602d93b27306f9f6ebf1d2b9ef893 */
// ==== 0x4042ed8  objc::Categories::ArcBrowserContextHistory_Extensions_::searchHistory:completion:  (300 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::searchHistory_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  code *pcVar1;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined1 auStack_70 [16];
  byte local_60;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context_history",auStack_70,param_3,param_4,(ulong)param_5);
  if ((local_60 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4043000);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_80 = 0;
    uStack_78 = 0;
  }
  else {
    _objc_msgSend(param_3,"options",&local_80);
  }
  local_48 = FUN_040454a0;
  local_40 = &PTR_FUN_0da87b68;
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_4;
  FUN_04332ba4(auStack_70,&local_80,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  FUN_04332750(&local_80);
  if (local_60 != 0) {
    FUN_04332750(auStack_70);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x40.c:81071 sha256 74748e88c7fdca8d2b1b6344f97e3e28e17602d93b27306f9f6ebf1d2b9ef893 */
// ==== 0x4044a1c  objc::Categories::ArcBrowserContextHistory_Extensions_::deleteByOriginatorVisitIDs:originatorCacheGuid:startTime:endTime:completion:  (856 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Type propagation algorithm not settling */
/* Function Stack Size: 0x38 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::
     deleteByOriginatorVisitIDs_originatorCacheGuid_startTime_endTime_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,ID param_7,
               undefined4 param_8)

{
  code *pcVar1;
  ID IVar2;
  ID IVar3;
  long lVar4;
  undefined8 extraout_d0;
  undefined8 extraout_d0_00;
  ID local_1b8 [3];
  long *local_1a0;
  undefined8 uStack_198;
  undefined8 local_190;
  undefined8 uStack_188;
  undefined8 uStack_180;
  undefined8 uStack_178;
  long local_170;
  ID local_168;
  ID local_160;
  long *local_158;
  ID *pIStack_150;
  long *local_148;
  ID *local_140;
  long *plStack_138;
  long *local_130;
  long local_128;
  ID *local_120 [2];
  undefined1 *local_110;
  undefined **local_108;
  undefined1 auStack_100 [128];
  long local_80;
  
  local_80 = *(long *)PTR____stack_chk_guard_0d824020;
  local_170 = 0;
  local_168 = 0;
  local_160 = 0;
  IVar2 = _objc_msgSend(param_3,"count");
  if (IVar2 != 0) {
    if (IVar2 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    local_170 = FUN_00008790(IVar2 << 3);
    local_168 = 0;
    local_160 = IVar2;
  }
  uStack_188 = 0;
  local_190 = 0;
  uStack_178 = 0;
  uStack_180 = 0;
  local_1b8[2] = 0;
  local_1b8[1] = 0;
  uStack_198 = 0;
  local_1a0 = (long *)0x0;
  IVar2 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",local_1b8 + 1,
                        auStack_100,0x10);
  if (IVar2 != 0) {
    lVar4 = *local_1a0;
    do {
      IVar3 = 0;
      do {
        if (*local_1a0 != lVar4) {
          _objc_enumerationMutation(param_3);
        }
        local_1b8[0] = _objc_msgSend(*(ID *)(local_1b8[2] + IVar3 * 8),"longLongValue");
        local_128 = local_170 + local_168 * 8;
        local_158 = &local_128;
        pIStack_150 = local_1b8;
        local_148 = &local_170;
        local_140 = local_1b8;
        plStack_138 = &local_170;
        local_130 = &local_128;
        FUN_01ab9b20(local_168 != local_160,&local_140,&local_158);
        local_168 = local_128 - local_170 >> 3;
        if (local_128 - local_170 == 0) goto LAB_04044d68;
        IVar3 = IVar3 + 1;
      } while (IVar2 != IVar3);
      IVar2 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",local_1b8 + 1,
                            auStack_100,0x10);
    } while (IVar2 != 0);
  }
  local_140 = (ID *)0x0;
  plStack_138 = (long *)0x0;
  local_130 = (long *)0x0;
  FUN_03b60ee8(&local_140,param_4);
  _objc_msgSend(param_1,"browser_context_history",&local_158);
  if (((ulong)local_148 & 1) != 0) {
    _objc_msgSend(param_5,"timeIntervalSince1970");
    _objc_msgSend(param_6,"timeIntervalSince1970");
    local_110 = &LAB_04044e30;
    local_108 = &PTR_FUN_0da87ce8;
    local_120[0] = (ID *)FUN_00008790(8);
    *local_120[0] = param_7;
    FUN_04333408(extraout_d0,extraout_d0_00,&local_158,&local_140,&local_170,local_120);
    if ((code *)local_108[1] != (code *)0x0) {
      (*(code *)local_108[1])(local_120[0]);
    }
    if ((char)local_148 != '\0') {
      FUN_04332750(&local_158);
    }
    if ((long)local_130 < 0) {
      FUN_00005810(local_140,(ulong)local_130 & 0x7fffffffffffffff);
    }
    if (local_170 == 0) {
LAB_04044c24:
      if (*(long *)PTR____stack_chk_guard_0d824020 != local_80) {
                    /* WARNING: Subroutine does not return */
        ___stack_chk_fail(0);
      }
      return;
    }
    lVar4 = local_168 << 3;
    do {
      if (lVar4 == 0) {
        local_168 = 0;
        FUN_00005810(local_170,local_160 << 3);
        goto LAB_04044c24;
      }
      lVar4 = lVar4 + -8;
    } while (local_170 + lVar4 != 0);
  }
LAB_04044d68:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4044d6c);
  (*pcVar1)();
}



/* SOURCE /home/exedev/work/inputs/decompile/0x40.c:135360 sha256 74748e88c7fdca8d2b1b6344f97e3e28e17602d93b27306f9f6ebf1d2b9ef893 */
// ==== 0x4084dc0  AXAnnouncementSpec::window  (8 bytes)

/* Function Stack Size: 0x10 bytes */

ID AXAnnouncementSpec::window(ID param_1,SEL param_2)

{
  return *(ID *)(param_1 + 0x18);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x42.c:129695 sha256 fb2d264d078694cfe2d5c7faf47662b118dc5279d4f6f0cf3788ead1cec1de87 */
// ==== 0x42b32d0  FUN_042b32d0  (768 bytes)

void FUN_042b32d0(undefined8 param_1,long param_2,undefined8 param_3,undefined8 *param_4)

{
  undefined1 *puVar1;
  long *plVar2;
  undefined1 uVar3;
  long lVar4;
  long *plVar5;
  long *plVar6;
  code *pcVar7;
  long *plVar8;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  
  puVar1 = (undefined1 *)(param_2 + 0x148);
  if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_2 + 0x148);
  }
  uVar3 = *puVar1;
  *puVar1 = 1;
  plVar5 = (long *)FUN_00008790(0x2c0);
  plVar8 = plVar5 + 1;
  *plVar8 = 0;
  plVar5[2] = 0;
  *plVar5 = (long)&PTR_viewWillStartLiveResize_0da7db20;
  plVar2 = plVar5 + 3;
  if (plVar2 == (long *)0x0) {
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(1,0x42b35d0);
    (*pcVar7)();
  }
  if (*(char *)((long)param_4 + 0x17) < '\0') {
    FUN_00060a60(&local_80,*param_4,param_4[1]);
  }
  else {
    uStack_78 = param_4[1];
    local_80 = *param_4;
    local_70 = param_4[2];
  }
  FUN_042f8300(plVar2,param_3,0,&local_80);
  if ((long)local_70 < 0) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if ((plVar5 != (long *)0xffffffffffffffe0) &&
     ((plVar6 = (long *)plVar5[5], plVar6 == (long *)0x0 || (plVar6[1] == -1)))) {
    if (plVar5 != (long *)0x0) {
      plVar5[1] = plVar5[1] + 1;
      plVar5[2] = plVar5[2] + 1;
    }
    plVar5[4] = (long)plVar2;
    plVar5[5] = (long)plVar5;
    if (plVar6 != (long *)0x0) {
      if (plVar6[2] != 0) {
        LOAcquire();
        lVar4 = plVar6[2];
        plVar6[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) goto LAB_042b3400;
      }
      pcVar7 = *(code **)(*plVar6 + 0x20);
      if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
        FUN_00005810(plVar6,0x20);
      }
      else {
        (*pcVar7)();
      }
    }
LAB_042b3400:
    if (plVar5 != (long *)0x0) {
      LOAcquire();
      lVar4 = *plVar8;
      *plVar8 = lVar4 + -1;
      LORelease();
      if (lVar4 == 0) {
        (**(code **)(*plVar5 + 0x10))(plVar5);
        if (plVar5[2] != 0) {
          LOAcquire();
          lVar4 = plVar5[2];
          plVar5[2] = lVar4 + -1;
          LORelease();
          if (lVar4 != 0) goto LAB_042b3410;
        }
        pcVar7 = *(code **)(*plVar5 + 0x20);
        if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
          FUN_00005810(plVar5,0x20);
        }
        else {
          (*pcVar7)(plVar5);
        }
      }
    }
  }
LAB_042b3410:
  FUN_042b35d0(param_2 + 0x408,plVar2);
  FUN_042c8400(param_1,plVar2);
  if (plVar5 != (long *)0x0) {
    LOAcquire();
    lVar4 = *plVar8;
    *plVar8 = lVar4 + -1;
    LORelease();
    if (lVar4 == 0) {
      (**(code **)(*plVar5 + 0x10))(plVar5);
      if (plVar5[2] != 0) {
        LOAcquire();
        lVar4 = plVar5[2];
        plVar5[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) goto LAB_042b3438;
      }
      pcVar7 = *(code **)(*plVar5 + 0x20);
      if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
        FUN_00005810(plVar5,0x20);
      }
      else {
        (*pcVar7)(plVar5);
      }
    }
  }
LAB_042b3438:
  if (puVar1 != (undefined1 *)0x0) {
    *puVar1 = uVar3;
  }
  if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4(puVar1);
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x42.c:130247 sha256 fb2d264d078694cfe2d5c7faf47662b118dc5279d4f6f0cf3788ead1cec1de87 */
// ==== 0x42b3d74  FUN_042b3d74  (440 bytes)

void FUN_042b3d74(undefined8 *param_1,long param_2,undefined4 param_3,undefined8 param_4,
                 undefined8 *param_5,undefined8 *param_6)

{
  ulong uVar1;
  byte bVar2;
  code *pcVar3;
  int iVar4;
  undefined4 *puVar5;
  char *pcVar6;
  undefined8 uVar7;
  undefined8 *puVar8;
  int *piVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 local_68 [2];
  undefined7 local_58;
  char cStack_51;
  
  bVar2 = *(byte *)((long)param_5 + 0x17);
  uVar1 = param_5[1];
  if (-1 < (char)bVar2) {
    uVar1 = (ulong)bVar2;
  }
  if (uVar1 == 0) {
    pcVar6 = "profile name cannot be empty";
    uVar10 = 0x1c;
    uVar7 = 0x213;
  }
  else {
    if (*(char *)(param_6[3] + 0x10) == '\0') {
      puVar8 = (undefined8 *)*param_5;
      if (-1 < (char)bVar2) {
        puVar8 = param_5;
      }
      FUN_007cf8e0(local_68,puVar8);
      piVar9 = *(int **)(param_2 + 0x468);
      if ((piVar9 != (int *)0x0) && (iVar4 = *piVar9, *piVar9 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
        pcVar3 = (code *)SoftwareBreakpoint(0,0x42b3f2c);
        (*pcVar3)();
      }
      uVar10 = *(undefined8 *)(param_2 + 0x470);
      puVar5 = (undefined4 *)FUN_00008790(0x80);
      *puVar5 = 1;
      *(undefined1 **)(puVar5 + 2) = &LAB_042bf4cc;
      *(code **)(puVar5 + 4) = FUN_042bf5ac;
      *(undefined1 **)(puVar5 + 6) = &LAB_04169bc0;
      *(code **)(puVar5 + 8) = FUN_042b3f34;
      *(undefined8 *)(puVar5 + 10) = 0;
      *(int **)(puVar5 + 0xc) = piVar9;
      *(undefined8 *)(puVar5 + 0xe) = uVar10;
      puVar5[0x10] = param_3;
      if (*(char *)((long)param_5 + 0x17) < '\0') {
        FUN_00060a60(puVar5 + 0x12,*param_5,param_5[1]);
      }
      else {
        uVar10 = *param_5;
        *(undefined8 *)(puVar5 + 0x14) = param_5[1];
        *(undefined8 *)(puVar5 + 0x12) = uVar10;
        *(undefined8 *)(puVar5 + 0x16) = param_5[2];
      }
      puVar8 = (undefined8 *)param_6[3];
      uVar10 = *param_6;
      uVar11 = param_6[3];
      uVar7 = param_6[2];
      *(undefined8 *)(puVar5 + 0x1a) = param_6[1];
      *(undefined8 *)(puVar5 + 0x18) = uVar10;
      *(undefined8 *)(puVar5 + 0x1e) = uVar11;
      *(undefined8 *)(puVar5 + 0x1c) = uVar7;
      if ((code *)*puVar8 != (code *)0x0) {
        uVar10 = (*(code *)*puVar8)(*param_6);
        *(undefined8 *)(puVar5 + 0x18) = uVar10;
      }
      FUN_07bdc470(local_68,0,param_4,puVar5,0);
      if (cStack_51 < '\0') {
        FUN_00005810(local_68[0],CONCAT17(cStack_51,local_58) << 1);
      }
      uVar10 = 1;
      goto LAB_042b3eb4;
    }
    pcVar6 = "callback is null";
    uVar10 = 0x10;
    uVar7 = 0x218;
  }
  uVar10 = FUN_04b37104(0xd,pcVar6,uVar10,uVar7,"../../arc/browser/browser_application_impl.cc");
LAB_042b3eb4:
  *param_1 = uVar10;
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x42.c:137955 sha256 fb2d264d078694cfe2d5c7faf47662b118dc5279d4f6f0cf3788ead1cec1de87 */
// ==== 0x42bdee4  FUN_042bdee4  (164 bytes)

void FUN_042bdee4(long param_1,undefined8 param_2,undefined8 param_3,undefined8 *param_4)

{
  undefined8 uVar1;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  long local_28;
  long local_18;
  
  local_18 = *(long *)PTR____stack_chk_guard_0d824020;
  uStack_38 = param_4[1];
  local_40 = *param_4;
  local_28 = param_4[3];
  uStack_30 = param_4[2];
  if (*(long *)(param_4[3] + 8) != 0) {
    param_4[2] = 0;
    param_4[3] = &DAT_0c4b3500;
  }
  uVar1 = FUN_042bdf88(param_1 + 0x348,param_2,param_3,&local_40);
  if (*(code **)(local_28 + 8) == (code *)0x0) {
    if (*(long *)PTR____stack_chk_guard_0d824020 == local_18) {
      return;
    }
  }
  else {
    uVar1 = local_40;
    if (*(long *)PTR____stack_chk_guard_0d824020 == local_18) {
                    /* WARNING: Could not recover jumptable at 0x042bdf5c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(local_28 + 8))();
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(uVar1);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x42.c:141660 sha256 fb2d264d078694cfe2d5c7faf47662b118dc5279d4f6f0cf3788ead1cec1de87 */
// ==== 0x42c35e4  FUN_042c35e4  (348 bytes)

void FUN_042c35e4(long param_1,undefined8 *param_2,int param_3,undefined1 param_4,
                 undefined8 *param_5)

{
  long lVar1;
  code *pcVar2;
  undefined8 *puVar3;
  undefined8 uVar4;
  undefined **local_4e8;
  ulong local_4e0;
  undefined8 local_4d8 [2];
  undefined7 local_4c8;
  char cStack_4c1;
  undefined1 auStack_4c0 [432];
  undefined1 local_310;
  undefined4 local_30c;
  
  if (*(long *)(param_1 + 0x70) != 0) {
    lVar1 = *(long *)(param_1 + 0x50);
    *(undefined8 *)(param_1 + 0x48) = 0;
    *(undefined8 **)(param_1 + 0x50) = &DAT_0c4b3500;
    pcVar2 = *(code **)(lVar1 + 8);
    if (pcVar2 != (code *)0x0) {
      (*pcVar2)(*(undefined8 *)(param_1 + 0x38));
    }
    uVar4 = *param_5;
    *(undefined8 *)(param_1 + 0x40) = param_5[1];
    *(undefined8 *)(param_1 + 0x38) = uVar4;
    *(undefined8 *)(param_1 + 0x48) = param_5[2];
    *(undefined8 *)(param_1 + 0x50) = param_5[3];
    param_5[2] = 0;
    param_5[3] = &DAT_0c4b3500;
    _bzero(auStack_4c0,0x470);
    lVar1 = (long)*(char *)((long)param_2 + 0x17);
    puVar3 = param_2;
    if (lVar1 < 0) {
      puVar3 = (undefined8 *)*param_2;
      lVar1 = param_2[1];
    }
    FUN_007cf8e0(local_4d8,puVar3,lVar1);
    local_4e0 = *(ulong *)(param_1 + 0x70);
    local_4e8 = &PTR_FUN_0da51808;
    if ((local_4e0 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(local_4e0);
    }
    FUN_01c8f2bc(auStack_4c0,local_4d8,(long)param_3,4,&local_4e8,0,0,0);
    local_4e8 = &PTR_FUN_0da51808;
    if ((local_4e0 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    if (cStack_4c1 < '\0') {
      FUN_00005810(local_4d8[0],CONCAT17(cStack_4c1,local_4c8) << 1);
    }
    local_30c = 0;
    local_310 = param_4;
    FUN_020cd850(*(undefined8 *)(param_1 + 0x30),auStack_4c0);
    FUN_01cd230c(auStack_4c0);
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x42.c:153646 sha256 fb2d264d078694cfe2d5c7faf47662b118dc5279d4f6f0cf3788ead1cec1de87 */
// ==== 0x42d5d2c  FUN_042d5d2c  (460 bytes)

void FUN_042d5d2c(long param_1,undefined8 *param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  int iVar3;
  undefined4 *puVar4;
  long lVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  ulong uVar8;
  long *plVar9;
  undefined8 *puVar10;
  int *piVar11;
  undefined1 *local_60;
  long lStack_58;
  undefined8 local_50;
  undefined8 local_48;
  undefined8 uStack_40;
  ulong local_38;
  
  local_48 = 0;
  uStack_40 = 0;
  local_38 = 0;
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(&local_60,*param_2,param_2[1]);
  }
  else {
    lStack_58 = param_2[1];
    local_60 = (undefined1 *)*param_2;
    local_50 = param_2[2];
  }
  ppuVar1 = (undefined1 **)local_60;
  if (-1 < (long)local_50._7_1_) {
    ppuVar1 = &local_60;
  }
  lVar5 = lStack_58;
  if (-1 < (long)local_50) {
    lVar5 = (long)local_50._7_1_;
  }
  FUN_008a7fc4(&local_48,ppuVar1,lVar5);
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  puVar10 = *(undefined8 **)(param_1 + 0x250);
  if (puVar10 == (undefined8 *)0x0) {
    puVar10 = (undefined8 *)FUN_00008790(0x10);
    uVar8 = *(ulong *)(param_1 + 0x88);
    if ((uVar8 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(uVar8);
    }
    FUN_0836a940(puVar10,uVar8);
    plVar9 = *(long **)(param_1 + 0x250);
    *(undefined8 **)(param_1 + 0x250) = puVar10;
    if (plVar9 != (long *)0x0) {
      lVar5 = plVar9[1];
      plVar9[1] = 0;
      if (lVar5 != 0) {
        uVar7 = FUN_06c06af8();
        FUN_00005810(uVar7,0x178);
      }
      lVar5 = *plVar9;
      *plVar9 = 0;
      if (lVar5 != 0) {
        uVar7 = FUN_078ca584();
        FUN_00005810(uVar7,0x80);
      }
      FUN_00005810(plVar9,0x10);
      puVar10 = *(undefined8 **)(param_1 + 0x250);
    }
  }
  uVar7 = *puVar10;
  piVar11 = *(int **)(param_1 + 0x310);
  if ((piVar11 != (int *)0x0) && (iVar3 = *piVar11, *piVar11 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(0,0x42d5ef8);
    (*pcVar2)();
  }
  uVar6 = *(undefined8 *)(param_1 + 0x318);
  puVar4 = (undefined4 *)FUN_00008790(0x40);
  *puVar4 = 1;
  *(undefined1 **)(puVar4 + 2) = &LAB_04076980;
  *(code **)(puVar4 + 4) = FUN_040a6300;
  *(undefined1 **)(puVar4 + 6) = &LAB_04169bc0;
  *(code **)(puVar4 + 8) = PiPNSView::viewWillStartLiveResize;
  *(undefined8 *)(puVar4 + 10) = 0;
  *(int **)(puVar4 + 0xc) = piVar11;
  *(undefined8 *)(puVar4 + 0xe) = uVar6;
  FUN_078cab40(uVar7,&local_48,1,puVar4);
  if ((long)local_38 < 0) {
    FUN_00005810(local_48,local_38 & 0x7fffffffffffffff);
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x42.c:166351 sha256 fb2d264d078694cfe2d5c7faf47662b118dc5279d4f6f0cf3788ead1cec1de87 */
// ==== 0x42e6790  FUN_042e6790  (2420 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_042e6790(long param_1,undefined8 *param_2,int param_3,undefined8 *param_4)

{
  undefined8 *******pppppppuVar1;
  size_t sVar2;
  size_t sVar3;
  byte bVar4;
  long lVar5;
  ulong uVar6;
  undefined8 ******ppppppuVar7;
  int iVar8;
  long lVar9;
  undefined8 uVar10;
  undefined4 *puVar11;
  long *plVar12;
  undefined8 *puVar13;
  ulong uVar14;
  undefined8 *puVar15;
  undefined8 *puVar16;
  code *pcVar17;
  undefined8 *puVar18;
  ulong uVar19;
  long *plVar20;
  long *plVar21;
  long *plVar22;
  long *plVar23;
  bool bVar24;
  int *piVar25;
  ulong uVar26;
  ulong uVar27;
  undefined8 *puVar28;
  ulong uVar29;
  ulong uVar30;
  ulong uVar31;
  float fVar32;
  undefined8 ******local_b0;
  size_t sStack_a8;
  undefined8 local_a0;
  long *local_98;
  undefined8 *local_90;
  ulong local_88;
  ulong local_80;
  undefined8 *local_78;
  ulong local_70;
  long local_68;
  
  if (*(long *)(param_1 + 0x68) == 0) {
    return;
  }
  lVar9 = FUN_042dda40(param_1 + 0x70);
  if (lVar9 != 0) {
    if (*(long *)(lVar9 + 0x38) == *(long *)(lVar9 + 0x40)) {
      puVar15 = (undefined8 *)FUN_042bfa68((long *)(lVar9 + 0x30),param_4);
    }
    else {
      puVar15 = (undefined8 *)(*(long *)(lVar9 + 0x30) + *(long *)(lVar9 + 0x38) * 0x20);
      if (puVar15 == (undefined8 *)0x0) goto LAB_042e70cc;
      uVar10 = *param_4;
      puVar15[1] = param_4[1];
      *puVar15 = uVar10;
      puVar15[2] = param_4[2];
      puVar16 = (undefined8 *)param_4[3];
      puVar15[3] = puVar16;
      pcVar17 = (code *)*puVar16;
      if (pcVar17 != (code *)0x0) {
        uVar10 = (*pcVar17)(*param_4);
        *puVar15 = uVar10;
      }
      puVar15 = puVar15 + 4;
    }
    lVar5 = (long)puVar15 - *(long *)(lVar9 + 0x30);
    *(long *)(lVar9 + 0x38) = lVar5 >> 5;
    if (lVar5 != 0) {
      return;
    }
LAB_042e70cc:
                    /* WARNING: Does not return */
    pcVar17 = (code *)SoftwareBreakpoint(1,0x42e70d0);
    (*pcVar17)();
  }
  if (param_3 == 0) {
    puVar15 = (undefined8 *)FUN_00008790(8);
    *puVar15 = 0;
    piVar25 = *(int **)(param_1 + 0x130);
    if ((piVar25 != (int *)0x0) && (iVar8 = *piVar25, *piVar25 = iVar8 + 1, iVar8 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar17 = (code *)SoftwareBreakpoint(0,0x42e710c);
      (*pcVar17)();
    }
    uVar10 = *(undefined8 *)(param_1 + 0x138);
    puVar11 = (undefined4 *)FUN_00008790(0x58);
    *puVar11 = 1;
    *(undefined1 **)(puVar11 + 2) = &LAB_042ec4c8;
    *(code **)(puVar11 + 4) = FUN_040b2960;
    *(undefined1 **)(puVar11 + 6) = &LAB_04169bc0;
    *(code **)(puVar11 + 8) = FUN_042e712c;
    *(undefined8 *)(puVar11 + 10) = 0;
    *(int **)(puVar11 + 0xc) = piVar25;
    *(undefined8 *)(puVar11 + 0xe) = uVar10;
    if (*(char *)((long)param_2 + 0x17) < '\0') {
      FUN_00060a60(puVar11 + 0x10,*param_2,param_2[1]);
    }
    else {
      uVar10 = *param_2;
      *(undefined8 *)(puVar11 + 0x12) = param_2[1];
      *(undefined8 *)(puVar11 + 0x10) = uVar10;
      *(undefined8 *)(puVar11 + 0x14) = param_2[2];
    }
    plVar12 = (long *)FUN_00008790(400);
    uVar10 = *(undefined8 *)(param_1 + 0x68);
    local_b0 = (undefined8 ******)FUN_00008790(8);
    *local_b0 = (undefined8 *****)0x0;
    *local_b0 = (undefined8 *****)*puVar15;
    sStack_a8 = 0;
    FUN_0786cd50(plVar12,param_2,uVar10,&local_b0,puVar11);
    ppppppuVar7 = local_b0;
    local_b0 = (undefined8 *******)0x0;
    if (ppppppuVar7 != (undefined8 ******)0x0) {
      _objc_destroyWeak(ppppppuVar7);
      FUN_00005810(ppppppuVar7,8);
    }
    if ((plVar12 != (long *)0x0) &&
       (iVar8 = (int)plVar12[1], *(int *)(plVar12 + 1) = iVar8 + 1, iVar8 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar17 = (code *)SoftwareBreakpoint(0,0x42e7124);
      (*pcVar17)();
    }
    if (puVar15 != (undefined8 *)0x0) {
      _objc_destroyWeak(puVar15);
      FUN_00005810(puVar15,8);
    }
  }
  else {
    piVar25 = *(int **)(param_1 + 0x130);
    if ((piVar25 != (int *)0x0) && (iVar8 = *piVar25, *piVar25 = iVar8 + 1, iVar8 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar17 = (code *)SoftwareBreakpoint(0,0x42e7100);
      (*pcVar17)();
    }
    uVar10 = *(undefined8 *)(param_1 + 0x138);
    puVar11 = (undefined4 *)FUN_00008790(0x58);
    *puVar11 = 1;
    *(undefined1 **)(puVar11 + 2) = &LAB_042ec4c8;
    *(code **)(puVar11 + 4) = FUN_040b2960;
    *(undefined1 **)(puVar11 + 6) = &LAB_04169bc0;
    *(code **)(puVar11 + 8) = FUN_042e712c;
    *(undefined8 *)(puVar11 + 10) = 0;
    *(int **)(puVar11 + 0xc) = piVar25;
    *(undefined8 *)(puVar11 + 0xe) = uVar10;
    if (*(char *)((long)param_2 + 0x17) < '\0') {
      FUN_00060a60(puVar11 + 0x10,*param_2,param_2[1]);
    }
    else {
      uVar10 = *param_2;
      *(undefined8 *)(puVar11 + 0x12) = param_2[1];
      *(undefined8 *)(puVar11 + 0x10) = uVar10;
      *(undefined8 *)(puVar11 + 0x14) = param_2[2];
    }
    plVar12 = (long *)FUN_00008790(400);
    uVar10 = *(undefined8 *)(param_1 + 0x68);
    local_b0 = (undefined8 ******)FUN_00008790(8);
    *local_b0 = (undefined8 *****)0x0;
    sStack_a8 = 0;
    FUN_0786cd50(plVar12,param_2,uVar10,&local_b0,puVar11);
    ppppppuVar7 = local_b0;
    local_b0 = (undefined8 *******)0x0;
    if (ppppppuVar7 != (undefined8 ******)0x0) {
      _objc_destroyWeak(ppppppuVar7);
      FUN_00005810(ppppppuVar7,8);
    }
    *plVar12 = (long)&PTR_FUN_0da7dfc0;
    plVar12[2] = (long)&PTR_LAB_0da7e048;
    *(undefined1 *)(plVar12 + 0x2d) = 0;
    if ((plVar12 != (long *)0x0) &&
       (iVar8 = (int)plVar12[1], *(int *)(plVar12 + 1) = iVar8 + 1, iVar8 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar17 = (code *)SoftwareBreakpoint(0,0x42e7118);
      (*pcVar17)();
    }
  }
  local_78 = (undefined8 *)0x0;
  local_70 = 0;
  local_68 = 0;
  puVar16 = (undefined8 *)FUN_042bfa68(&local_78,param_4);
  puVar15 = local_78;
  uVar30 = (long)puVar16 - (long)local_78 >> 5;
  local_70 = uVar30;
  if ((long)puVar16 - (long)local_78 == 0) goto LAB_042e70cc;
  if ((plVar12 != (long *)0x0) &&
     (iVar8 = (int)plVar12[1], *(int *)(plVar12 + 1) = iVar8 + 1, iVar8 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar17 = (code *)SoftwareBreakpoint(0,0x42e70f4);
    (*pcVar17)();
  }
  if (uVar30 >> 0x3b != 0) {
                    /* WARNING: Subroutine does not return */
    FUN_040416c0();
  }
  puVar13 = (undefined8 *)FUN_00008790();
  puVar28 = puVar13;
  do {
    if (puVar28 == (undefined8 *)0x0) goto LAB_042e70cc;
    uVar10 = *puVar15;
    puVar28[1] = puVar15[1];
    *puVar28 = uVar10;
    puVar28[2] = puVar15[2];
    puVar18 = (undefined8 *)puVar15[3];
    puVar28[3] = puVar18;
    pcVar17 = (code *)*puVar18;
    if (pcVar17 != (code *)0x0) {
      uVar10 = (*pcVar17)(*puVar15);
      *puVar28 = uVar10;
    }
    puVar15 = puVar15 + 4;
    puVar28 = puVar28 + 4;
  } while (puVar15 != puVar16);
  uVar26 = (long)puVar28 - (long)puVar13 >> 5;
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(&local_b0,*param_2,param_2[1]);
  }
  else {
    sStack_a8 = param_2[1];
    local_b0 = (undefined8 ******)*param_2;
    local_a0 = param_2[2];
  }
  pppppppuVar1 = (undefined8 *******)local_b0;
  if (-1 < (long)local_a0._7_1_) {
    pppppppuVar1 = &local_b0;
  }
  sVar2 = sStack_a8;
  if (-1 < (long)local_a0) {
    sVar2 = (long)local_a0._7_1_;
  }
  local_98 = plVar12;
  local_90 = puVar13;
  local_88 = uVar26;
  local_80 = uVar30;
  uVar14 = FUN_00018f3c(pppppppuVar1,sVar2);
  uVar29 = *(ulong *)(param_1 + 0x78);
  if (uVar29 == 0) {
    uVar31 = 0;
  }
  else {
    uVar27 = uVar29 - 1;
    if ((uVar29 & uVar27) == 0) {
      uVar31 = uVar27 & uVar14;
    }
    else {
      uVar31 = uVar14;
      if (uVar29 <= uVar14) {
        uVar31 = 0;
        if (uVar29 != 0) {
          uVar31 = uVar14 / uVar29;
        }
        uVar31 = uVar14 - uVar31 * uVar29;
      }
    }
    plVar20 = *(long **)(*(long *)(param_1 + 0x70) + uVar31 * 8);
    if (plVar20 != (long *)0x0) {
      for (plVar20 = (long *)*plVar20; plVar20 != (long *)0x0; plVar20 = (long *)*plVar20) {
        uVar19 = plVar20[1];
        if (uVar19 == uVar14) {
          bVar4 = *(byte *)((long)plVar20 + 0x27);
          sVar3 = plVar20[3];
          if (-1 < (char)bVar4) {
            sVar3 = (ulong)bVar4;
          }
          if (sVar3 == sVar2) {
            puVar15 = (void *)plVar20[2];
            if (-1 < (char)bVar4) {
              puVar15 = plVar20 + 2;
            }
            iVar8 = _memcmp(puVar15,pppppppuVar1,sVar2);
            if (iVar8 == 0) goto LAB_042e6f54;
          }
        }
        else {
          if ((uVar29 & uVar27) == 0) {
            uVar19 = uVar19 & uVar27;
          }
          else if (uVar29 <= uVar19) {
            uVar6 = 0;
            if (uVar29 != 0) {
              uVar6 = uVar19 / uVar29;
            }
            uVar19 = uVar19 - uVar6 * uVar29;
          }
          if (uVar19 != uVar31) break;
        }
      }
    }
  }
  plVar20 = (long *)FUN_00008790(0x48);
  if (plVar20 == (long *)0x0) goto LAB_042e70cc;
  *plVar20 = 0;
  plVar20[1] = uVar14;
  if (plVar20 + 2 == (long *)0x0) goto LAB_042e70cc;
  plVar20[3] = sStack_a8;
  plVar20[2] = (long)local_b0;
  plVar20[4] = local_a0;
  local_b0 = (undefined8 *******)0x0;
  sStack_a8 = 0;
  local_98 = (long *)0x0;
  local_90 = (undefined8 *)0x0;
  plVar20[5] = (long)plVar12;
  plVar20[6] = (long)puVar13;
  local_88 = 0;
  local_80 = 0;
  local_a0 = 0;
  fVar32 = (float)(*(long *)(param_1 + 0x88) + 1);
  plVar20[7] = uVar26;
  plVar20[8] = uVar30;
  if (fVar32 <= *(float *)(param_1 + 0x90) * (float)uVar29) goto LAB_042e6f2c;
  uVar30 = (ulong)((uVar29 & uVar29 - 1) != 0);
  if (uVar29 < 3) {
    uVar30 = 1;
  }
  uVar30 = uVar30 | uVar29 << 1;
  uVar26 = (ulong)(fVar32 / *(float *)(param_1 + 0x90));
  if (uVar30 <= uVar26) {
    uVar30 = uVar26;
  }
  if (uVar30 - 1 == 0) {
    uVar30 = 2;
  }
  else if ((uVar30 & uVar30 - 1) != 0) {
    uVar30 = FUN_00f76630(uVar30);
    uVar29 = *(ulong *)(param_1 + 0x78);
  }
  if (uVar29 < uVar30) {
LAB_042e6e10:
    if (uVar30 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04b087f0();
    }
    uVar10 = FUN_00008790(uVar30 << 3);
    bVar24 = false;
LAB_042e6e24:
    lVar9 = *(long *)(param_1 + 0x70);
    *(undefined8 *)(param_1 + 0x70) = uVar10;
    if (lVar9 != 0) {
      FUN_00005810(lVar9,*(long *)(param_1 + 0x78) << 3);
    }
    *(ulong *)(param_1 + 0x78) = uVar30;
    if (!bVar24) {
      uVar26 = 0;
      do {
        *(undefined8 *)(*(long *)(param_1 + 0x70) + uVar26 * 8) = 0;
        uVar26 = uVar26 + 1;
      } while (uVar30 != uVar26);
      plVar21 = *(long **)(param_1 + 0x80);
      if (plVar21 != (long *)0x0) {
        uVar26 = plVar21[1];
        uVar29 = uVar30 - 1;
        if ((uVar30 & uVar29) == 0) {
          uVar26 = uVar26 & uVar29;
        }
        else if (uVar30 <= uVar26) {
          uVar31 = 0;
          if (uVar30 != 0) {
            uVar31 = uVar26 / uVar30;
          }
          uVar26 = uVar26 - uVar31 * uVar30;
        }
        *(undefined8 **)(*(long *)(param_1 + 0x70) + uVar26 * 8) = (undefined8 *)(param_1 + 0x80);
        plVar22 = (long *)*plVar21;
        while (plVar22 != (long *)0x0) {
          uVar31 = plVar22[1];
          if ((uVar30 & uVar29) == 0) {
            uVar31 = uVar31 & uVar29;
          }
          else if (uVar30 <= uVar31) {
            uVar27 = 0;
            if (uVar30 != 0) {
              uVar27 = uVar31 / uVar30;
            }
            uVar31 = uVar31 - uVar27 * uVar30;
          }
          plVar23 = plVar22;
          if (uVar31 != uVar26) {
            if (*(long *)(*(long *)(param_1 + 0x70) + uVar31 * 8) == 0) {
              *(long **)(*(long *)(param_1 + 0x70) + uVar31 * 8) = plVar21;
              uVar26 = uVar31;
            }
            else {
              *plVar21 = *plVar22;
              *plVar22 = **(undefined8 **)(*(long *)(param_1 + 0x70) + uVar31 * 8);
              **(long **)(*(long *)(param_1 + 0x70) + uVar31 * 8) = (long)plVar22;
              plVar23 = plVar21;
            }
          }
          plVar21 = plVar23;
          plVar22 = (long *)*plVar23;
        }
      }
    }
  }
  else if (uVar30 < uVar29) {
    uVar26 = (ulong)((float)*(ulong *)(param_1 + 0x88) / *(float *)(param_1 + 0x90));
    if ((uVar29 < 3) || ((uVar29 & uVar29 - 1) != 0)) {
      uVar26 = FUN_00f76630();
    }
    else if (1 < uVar26) {
      uVar26 = 1L << (-LZCOUNT(uVar26 - 1) & 0x3fU);
    }
    if (uVar30 <= uVar26) {
      uVar30 = uVar26;
    }
    if (uVar30 < uVar29) {
      if (uVar30 != 0) goto LAB_042e6e10;
      uVar10 = 0;
      bVar24 = true;
      goto LAB_042e6e24;
    }
  }
  uVar29 = *(ulong *)(param_1 + 0x78);
  if ((uVar29 & uVar29 - 1) == 0) {
    uVar31 = uVar29 - 1 & uVar14;
  }
  else {
    uVar31 = uVar14;
    if (uVar29 <= uVar14) {
      uVar30 = 0;
      if (uVar29 != 0) {
        uVar30 = uVar14 / uVar29;
      }
      uVar31 = uVar14 - uVar30 * uVar29;
    }
  }
LAB_042e6f2c:
  lVar9 = *(long *)(param_1 + 0x70);
  plVar21 = *(long **)(lVar9 + uVar31 * 8);
  if (plVar21 == (long *)0x0) {
    plVar21 = (long *)(param_1 + 0x80);
    *plVar20 = *plVar21;
    *plVar21 = (long)plVar20;
    *(long **)(lVar9 + uVar31 * 8) = plVar21;
    if (*plVar20 != 0) {
      uVar30 = *(ulong *)(*plVar20 + 8);
      if ((uVar29 & uVar29 - 1) == 0) {
        uVar30 = uVar30 & uVar29 - 1;
      }
      else if (uVar29 <= uVar30) {
        uVar26 = 0;
        if (uVar29 != 0) {
          uVar26 = uVar30 / uVar29;
        }
        uVar30 = uVar30 - uVar26 * uVar29;
      }
      *(long **)(*(long *)(param_1 + 0x70) + uVar30 * 8) = plVar20;
    }
  }
  else {
    *plVar20 = *plVar21;
    *plVar21 = (long)plVar20;
  }
  *(long *)(param_1 + 0x88) = *(long *)(param_1 + 0x88) + 1;
  puVar13 = local_90;
LAB_042e6f54:
  if (puVar13 != (undefined8 *)0x0) {
    puVar15 = puVar13;
    if ((local_88 & 0x7ffffffffffffff) != 0) {
      lVar9 = local_88 * -0x20;
      puVar16 = puVar13 + local_88 * 4;
      do {
        puVar28 = puVar16 + -4;
        if (puVar28 == (undefined8 *)0x0) goto LAB_042e70cc;
        if (*(code **)(puVar16[-1] + 8) != (code *)0x0) {
          (**(code **)(puVar16[-1] + 8))(*puVar28);
        }
        lVar9 = lVar9 + 0x20;
        puVar15 = local_90;
        puVar16 = puVar28;
      } while (lVar9 != 0);
    }
    local_88 = (long)puVar13 - (long)puVar15 >> 5;
    FUN_00005810(puVar15,local_80 << 5);
  }
  if (local_98 != (long *)0x0) {
    LOAcquire();
    iVar8 = (int)local_98[1];
    *(int *)(local_98 + 1) = iVar8 + -1;
    LORelease();
    if (iVar8 == 1) {
      (**(code **)(*local_98 + 0x20))();
    }
  }
  if ((long)local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 & 0x7fffffffffffffff);
  }
  FUN_0784a050(plVar12);
  puVar15 = local_78;
  if (local_78 != (undefined8 *)0x0) {
    if ((local_70 & 0x7ffffffffffffff) != 0) {
      lVar9 = local_70 * -0x20;
      puVar16 = local_78 + local_70 * 4;
      do {
        puVar28 = puVar16 + -4;
        if (puVar28 == (undefined8 *)0x0) goto LAB_042e70cc;
        if (*(code **)(puVar16[-1] + 8) != (code *)0x0) {
          (**(code **)(puVar16[-1] + 8))(*puVar28);
        }
        lVar9 = lVar9 + 0x20;
        puVar16 = puVar28;
      } while (lVar9 != 0);
    }
    local_70 = 0;
    FUN_00005810(puVar15,local_68 << 5);
  }
  if (plVar12 != (long *)0x0) {
    LOAcquire();
    iVar8 = (int)plVar12[1];
    *(int *)(plVar12 + 1) = iVar8 + -1;
    LORelease();
    if (iVar8 == 1) {
      (**(code **)(*plVar12 + 0x20))(plVar12);
    }
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x42.c:169852 sha256 fb2d264d078694cfe2d5c7faf47662b118dc5279d4f6f0cf3788ead1cec1de87 */
// ==== 0x42eb57c  FUN_042eb57c  (1400 bytes)

/* WARNING: Removing unreachable block (ram,0x042eb888) */

void FUN_042eb57c(long param_1,long *param_2)

{
  undefined1 *puVar1;
  size_t sVar2;
  size_t sVar3;
  undefined8 *puVar4;
  undefined1 uVar5;
  byte bVar6;
  char cVar7;
  ulong uVar8;
  code *pcVar9;
  bool bVar10;
  int iVar11;
  ulong uVar12;
  ulong extraout_x1;
  long *plVar13;
  ulong uVar14;
  long lVar15;
  long lVar16;
  ulong uVar17;
  undefined8 *puVar18;
  ulong uVar19;
  undefined8 *puVar20;
  undefined8 *puVar21;
  ulong uVar22;
  long lVar23;
  undefined8 *puVar24;
  undefined8 uVar25;
  undefined8 uVar26;
  long *local_b8;
  ulong local_b0;
  long local_a8;
  long local_a0;
  ulong uStack_98;
  long *local_90;
  long local_88;
  undefined8 local_80;
  long local_78;
  long local_70;
  ulong local_68;
  
  if (*(long *)(param_1 + 0x68) == 0) {
    return;
  }
  if (*(long *)(param_1 + 0x108) == 0) {
    return;
  }
  puVar1 = (undefined1 *)(param_1 + 0x100);
  if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_1 + 0x100);
  }
  uVar5 = *puVar1;
  *puVar1 = 1;
  local_78 = 0;
  local_70 = 0;
  local_68 = 0;
  uStack_98 = 0;
  local_a0 = 0;
  local_88 = 0;
  local_90 = (long *)0x0;
  local_80 = 0x3f800000;
  uVar19 = param_2[1];
  if (uVar19 != 0) {
    if (0xaaaaaaaaaaaaaaa < uVar19) goto LAB_042eba64;
    lVar15 = uVar19 * 0x18;
    local_78 = FUN_00008790(lVar15);
    local_70 = 0;
    lVar23 = *param_2;
    local_68 = uVar19;
    do {
      local_b8 = &local_a0;
      FUN_010d8c54(&local_b8,lVar23,lVar23);
      if ((extraout_x1 & 1) != 0) {
        FUN_00ad9ee8(&local_78,lVar23);
      }
      lVar23 = lVar23 + 0x18;
      lVar15 = lVar15 + -0x18;
    } while (lVar15 != 0);
  }
  lVar23 = *(long *)(*(long *)(param_1 + 0x108) + 200) * 0x18;
  if (lVar23 == 0) {
    puVar24 = (undefined8 *)0x0;
    puVar18 = (undefined8 *)0x0;
LAB_042eb620:
    bVar10 = true;
  }
  else {
    if (0xaaaaaaaaaaaaaaa < (ulong)((lVar23 >> 3) * -0x5555555555555555)) {
LAB_042eba64:
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    lVar16 = *(long *)(*(long *)(param_1 + 0x108) + 0xc0);
    puVar18 = (undefined8 *)FUN_00008790(lVar23);
    lVar15 = 0;
    do {
      puVar24 = (undefined8 *)((long)puVar18 + lVar15);
      if (puVar24 == (undefined8 *)0x0) goto LAB_042eba60;
      puVar21 = (undefined8 *)(lVar16 + lVar15);
      if (*(char *)((long)puVar21 + 0x17) < '\0') {
        puVar21 = (undefined8 *)(lVar16 + lVar15);
        FUN_00060a60(puVar24,*puVar21,puVar21[1]);
      }
      else {
        uVar26 = puVar21[1];
        uVar25 = *puVar21;
        puVar24[2] = puVar21[2];
        puVar24[1] = uVar26;
        *puVar24 = uVar25;
      }
      lVar15 = lVar15 + 0x18;
    } while (lVar23 - lVar15 != 0);
    puVar24 = (undefined8 *)((long)puVar18 + lVar15);
    puVar21 = puVar18;
    if (lVar15 == 0) goto LAB_042eb620;
    do {
      uVar19 = uStack_98;
      if (uStack_98 != 0 && local_88 != 0) {
        cVar7 = *(char *)((long)puVar21 + 0x17);
        puVar20 = (void *)*puVar21;
        if (-1 < (long)cVar7) {
          puVar20 = puVar21;
        }
        sVar2 = puVar21[1];
        if (-1 < cVar7) {
          sVar2 = (long)cVar7;
        }
        uVar12 = FUN_00018f3c(puVar20,sVar2);
        uVar22 = uVar19 - 1;
        if ((uVar19 & uVar22) == 0) {
          uVar17 = uVar12 & uVar22;
        }
        else {
          uVar17 = uVar12;
          if (uVar19 <= uVar12) {
            uVar17 = 0;
            if (uVar19 != 0) {
              uVar17 = uVar12 / uVar19;
            }
            uVar17 = uVar12 - uVar17 * uVar19;
          }
        }
        plVar13 = *(long **)(local_a0 + uVar17 * 8);
        if (plVar13 != (long *)0x0) {
          for (plVar13 = (long *)*plVar13; plVar13 != (long *)0x0; plVar13 = (long *)*plVar13) {
            uVar14 = plVar13[1];
            if (uVar12 == uVar14) {
              bVar6 = *(byte *)((long)plVar13 + 0x27);
              sVar3 = plVar13[3];
              if (-1 < (char)bVar6) {
                sVar3 = (ulong)bVar6;
              }
              if (sVar3 == sVar2) {
                puVar4 = (void *)plVar13[2];
                if (-1 < (char)bVar6) {
                  puVar4 = plVar13 + 2;
                }
                iVar11 = _memcmp(puVar4,puVar20,sVar2);
                if (iVar11 == 0) goto LAB_042eb9f8;
              }
            }
            else {
              if ((uVar19 & uVar22) == 0) {
                uVar14 = uVar14 & uVar22;
              }
              else if (uVar19 <= uVar14) {
                uVar8 = 0;
                if (uVar19 != 0) {
                  uVar8 = uVar14 / uVar19;
                }
                uVar14 = uVar14 - uVar8 * uVar19;
              }
              if (uVar14 != uVar17) break;
            }
          }
        }
      }
      FUN_07cd94c0(*(undefined8 *)(param_1 + 0x108),puVar21,0);
LAB_042eb9f8:
      bVar10 = puVar24 + -3 != puVar21;
      puVar21 = puVar21 + 3;
    } while (bVar10);
    bVar10 = false;
  }
  local_b8 = (long *)0x0;
  local_b0 = 0;
  local_a8 = 0;
  lVar15 = local_70 * 0x18;
  lVar16 = local_78;
  if (lVar15 != 0) {
    do {
      uVar19 = FUN_07cd9ea0(*(undefined8 *)(param_1 + 0x108),lVar16);
      if ((uVar19 & 1) == 0) {
        FUN_07cd94c0(*(undefined8 *)(param_1 + 0x108),lVar16,1);
      }
      iVar11 = FUN_07cd9ea0(*(undefined8 *)(param_1 + 0x108),lVar16);
      if (iVar11 != 0) {
        FUN_00ad9ee8(&local_b8,lVar16);
      }
      lVar15 = lVar15 + -0x18;
      lVar16 = lVar16 + 0x18;
    } while (lVar15 != 0);
    if (local_b0 != 0) {
      lVar15 = 0;
      uVar19 = 0;
      do {
        FUN_07cd9f40(*(undefined8 *)(param_1 + 0x108),(long)local_b8 + lVar15,uVar19);
        uVar19 = uVar19 + 1;
        lVar15 = lVar15 + 0x18;
      } while (uVar19 < local_b0);
      lVar15 = local_b0 * 0x18;
      goto LAB_042eb754;
    }
  }
  lVar15 = 0;
LAB_042eb754:
  plVar13 = local_b8;
  if (local_b8 != (long *)0x0) {
    if (lVar15 != 0) {
      puVar21 = (undefined8 *)(lVar15 + (long)local_b8);
      do {
        puVar20 = puVar21 + -3;
        if (puVar20 == (undefined8 *)0x0) goto LAB_042eba60;
        if (*(char *)((long)puVar21 + -1) < '\0') {
          FUN_00005810(*puVar20,puVar21[-1] & 0x7fffffffffffffff);
        }
        lVar15 = lVar15 + -0x18;
        puVar21 = puVar20;
      } while (lVar15 != 0);
    }
    local_b0 = ((long)plVar13 - (long)local_b8 >> 3) * -0x5555555555555555;
    FUN_00005810(local_b8,local_a8 * 0x18);
  }
  lVar15 = local_a0;
  plVar13 = local_90;
  if (puVar18 != (undefined8 *)0x0) {
    if (!bVar10) {
      do {
        puVar24 = puVar24 + -3;
        if (puVar24 == (undefined8 *)0x0) goto LAB_042eba60;
      } while (puVar24 != puVar18);
    }
    FUN_00005810(puVar18,lVar23);
    lVar15 = local_a0;
    plVar13 = local_90;
  }
  while (plVar13 != (long *)0x0) {
    local_a0 = lVar15;
    if (plVar13 == (long *)0xfffffffffffffff0) goto LAB_042eba60;
    lVar23 = *plVar13;
    if (*(char *)((long)plVar13 + 0x27) < '\0') {
      FUN_00005810(plVar13[2],plVar13[4] & 0x7fffffffffffffff);
    }
    FUN_00005810(plVar13,0x28);
    lVar15 = local_a0;
    plVar13 = (long *)lVar23;
  }
  local_a0 = 0;
  if (lVar15 != 0) {
    FUN_00005810(lVar15,uStack_98 << 3);
  }
  lVar23 = local_78;
  if (local_78 != 0) {
    if (local_70 * 0x18 != 0) {
      lVar15 = local_70 * -0x18;
      puVar18 = (undefined8 *)(local_70 * 0x18 + local_78);
      do {
        puVar24 = puVar18 + -3;
        if (puVar24 == (undefined8 *)0x0) {
LAB_042eba60:
                    /* WARNING: Does not return */
          pcVar9 = (code *)SoftwareBreakpoint(1,0x42eba64);
          (*pcVar9)();
        }
        if (*(char *)((long)puVar18 + -1) < '\0') {
          FUN_00005810(*puVar24,puVar18[-1] & 0x7fffffffffffffff);
        }
        lVar15 = lVar15 + 0x18;
        puVar18 = puVar24;
      } while (lVar15 != 0);
    }
    local_70 = (lVar23 - local_78 >> 3) * -0x5555555555555555;
    FUN_00005810(local_78,local_68 * 0x18);
  }
  if (puVar1 != (undefined1 *)0x0) {
    *puVar1 = uVar5;
  }
  if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4(puVar1);
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:83453 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x4376580  FUN_04376580  (376 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */

long FUN_04376580(long param_1)

{
  long *plVar1;
  code *pcVar2;
  int iVar3;
  long lVar4;
  ulong uVar5;
  int *piVar6;
  long *local_48;
  long *local_40;
  undefined8 local_38;
  
  lVar4 = param_1;
  if ((*(long *)(param_1 + 0x130) != 0) && (lVar4 = FUN_020cf42c(0x1d), (int)lVar4 != 0)) {
    local_38 = *(undefined8 *)(param_1 + 0x288);
    uVar5 = FUN_0473c9a8(&local_38);
    lVar4 = 0;
    if ((uVar5 & 1) == 0) {
      local_40 = (long *)0x0;
      (**(code **)(**(long **)(*(long *)(param_1 + 0x130) + 0xe8) + 0x10))(&local_40);
      lVar4 = 0;
      if (*local_40 != 0) {
        local_48 = (long *)FUN_00008790(8);
        *local_48 = 0;
        *local_48 = *local_40;
        piVar6 = *(int **)(param_1 + 0x610);
        if ((piVar6 != (int *)0x0) && (iVar3 = *piVar6, *piVar6 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x43766f8);
          (*pcVar2)();
        }
        lVar4 = 0;
        if (*(long *)(param_1 + 0x618) != 0) {
          lVar4 = *(long *)(param_1 + 0x618) + 0x68;
        }
        lVar4 = FUN_0473c4dc(&local_48,piVar6,lVar4);
        plVar1 = local_48;
        if (*(long *)(param_1 + 0x288) != lVar4) {
          *(long *)(param_1 + 0x288) = lVar4;
        }
        lVar4 = 0;
        local_48 = (long *)0x0;
        if (plVar1 != (long *)0x0) {
          _objc_destroyWeak(plVar1);
          lVar4 = FUN_00005810(plVar1,8);
        }
      }
      plVar1 = local_40;
      local_40 = (long *)0x0;
      if (plVar1 != (long *)0x0) {
        _objc_destroyWeak(plVar1);
        lVar4 = FUN_00005810(plVar1,8);
      }
    }
  }
  return lVar4;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:148225 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43dd450  ICloudKeychainPresentationDelegate::presentationAnchorForAuthorizationController:  (8 bytes)

/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x18 bytes */

ID ICloudKeychainPresentationDelegate::presentationAnchorForAuthorizationController_
             (ID param_1,SEL param_2,ID param_3)

{
  return *(ID *)(param_1 + 8);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:151809 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e14d4  ArcBrowser::setWindow:  (120 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowser::setWindow_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  undefined8 uVar2;
  ID local_28;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    _os_retain(param_3);
    uVar2 = *(undefined8 *)(param_1 + 0x28);
    local_28 = param_3;
    _os_retain(param_3);
    FUN_042fa9fc(uVar2,&local_28);
    _os_release(local_28);
    _os_release(param_3);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43e154c);
  (*pcVar1)();
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:151908 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e1734  ArcBrowser::takeOwnershipOfWebContents:  (408 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowser::takeOwnershipOfWebContents_(ID param_1,SEL param_2,ID param_3)

{
  ID IVar1;
  undefined8 uVar2;
  long lVar3;
  code *pcVar4;
  undefined8 local_50;
  long *local_48;
  ulong local_40;
  ulong local_38;
  
  if ((*(byte *)(param_1 + 0x38) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43e18c0);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_50 = 0;
    local_48 = (long *)0x0;
    local_40 = 0;
    IVar1 = 0;
  }
  else {
    IVar1 = _objc_msgSend(param_3,"web_contents",&local_50);
    if ((local_40 & 1) != 0) {
      lVar3 = *(long *)(param_1 + 0x28);
      if (((*(long *)(lVar3 + 0xf0) == 0) || (*(char *)(*(long *)(lVar3 + 0xf0) + 4) != '\0')) ||
         (*(long *)(lVar3 + 0xf8) == 0)) {
        local_38 = FUN_04b37104(0x25,"browser deinitialized",0x15,0x1dc,
                                "../../arc/browser/browser_impl.cc");
      }
      else {
        if ((*(long *)(lVar3 + 0xf0) == 0) || (*(char *)(*(long *)(lVar3 + 0xf0) + 4) != '\0')) {
          uVar2 = 0;
        }
        else {
          uVar2 = *(undefined8 *)(lVar3 + 0xf8);
        }
        FUN_0437b014(&local_38,local_50,uVar2);
      }
      if ((local_38 & 1) == 0) {
        FUN_04b35898();
      }
      if (((char)local_40 != '\0') && (local_48 != (long *)0x0)) {
        LOAcquire();
        lVar3 = local_48[1];
        local_48[1] = lVar3 + -1;
        LORelease();
        if (lVar3 == 0) {
          (**(code **)(*local_48 + 0x10))(local_48);
          if (local_48[2] != 0) {
            LOAcquire();
            lVar3 = local_48[2];
            local_48[2] = lVar3 + -1;
            LORelease();
            if (lVar3 != 0) {
              return;
            }
          }
          pcVar4 = *(code **)(*local_48 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(local_48,0x20);
          }
          else {
            (*pcVar4)(local_48);
          }
        }
      }
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00(IVar1);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:154604 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e6548  ArcBrowserApplication::init:  (832 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcBrowserApplication::init_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ID IVar2;
  undefined8 *puVar3;
  ID IVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  code *pcVar7;
  long *plVar8;
  undefined1 auVar9 [16];
  undefined1 auStack_60 [8];
  long *local_58;
  ID local_50;
  class_t *pcStack_48;
  
  pcStack_48 = &objc::class_t::ArcBrowserApplication;
  local_50 = param_1;
  IVar2 = NSObject::init((ID)&local_50,"init");
  if (IVar2 == 0) {
    return 0;
  }
  puVar3 = (undefined8 *)FUN_00008790(0x28);
  puVar3[1] = 0;
  puVar3[2] = 0;
  *puVar3 = &PTR_viewWillStartLiveResize_0da87850;
  if (puVar3 + 3 == (undefined8 *)0x0) goto LAB_043e6884;
  auVar9 = FUN_042b17e8();
  *(undefined1 (*) [16])(puVar3 + 3) = auVar9;
  plVar8 = *(long **)(IVar2 + 0x28);
  *(undefined8 **)(IVar2 + 0x20) = puVar3 + 3;
  *(undefined8 **)(IVar2 + 0x28) = puVar3;
  if (plVar8 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar8[1];
    plVar8[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar8 + 0x10))(plVar8);
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar1 = plVar8[2];
        plVar8[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043e65dc;
      }
      pcVar7 = *(code **)(*plVar8 + 0x20);
      if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar7)(plVar8);
      }
    }
  }
LAB_043e65dc:
  IVar4 = _objc_alloc(&objc::class_t::ArcLocalState);
  FUN_042c8400(auStack_60,*(undefined8 *)(**(long **)(IVar2 + 0x20) + 200));
  IVar4 = ArcLocalState::initWithLocalState_(IVar4,"initWithLocalState:",auStack_60);
  *(ID *)(IVar2 + 8) = IVar4;
  if (local_58 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_58[1];
    local_58[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar1 = local_58[2];
        local_58[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043e663c;
      }
      pcVar7 = *(code **)(*local_58 + 0x20);
      if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar7)(local_58);
      }
    }
  }
LAB_043e663c:
  if (param_3 == 0) goto LAB_043e66bc;
  puVar5 = (undefined8 *)FUN_00008790(0x28);
  puVar5[1] = 0;
  puVar5[2] = 0;
  *puVar5 = &PTR_viewWillStartLiveResize_0da87888;
  puVar3 = puVar5 + 3;
  if (puVar3 == (undefined8 *)0x0) {
LAB_043e6884:
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(1,0x43e6888);
    (*pcVar7)();
  }
  puVar5[3] = &PTR_FUN_0da878c0;
  _objc_initWeak(puVar5 + 4,param_3);
  plVar8 = *(long **)(IVar2 + 0x38);
  *(undefined8 **)(IVar2 + 0x30) = puVar3;
  *(undefined8 **)(IVar2 + 0x38) = puVar5;
  if (plVar8 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar8[1];
    plVar8[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar8 + 0x10))(plVar8);
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar1 = plVar8[2];
        plVar8[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043e6698;
      }
      pcVar7 = *(code **)(*plVar8 + 0x20);
      if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar7)(plVar8);
      }
    }
LAB_043e6698:
    puVar3 = *(undefined8 **)(IVar2 + 0x30);
    puVar5 = *(undefined8 **)(IVar2 + 0x38);
  }
  uVar6 = *(undefined8 *)(IVar2 + 0x20);
  if (puVar5 != (undefined8 *)0x0) {
    puVar5[2] = puVar5[2] + 1;
  }
  FUN_04332148(uVar6,puVar3,puVar5);
LAB_043e66bc:
  *(undefined8 *)(IVar2 + 0x10) = 0;
  *(undefined8 *)(IVar2 + 0x18) = 0;
  return IVar2;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:154987 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e6ec0  ArcBrowserApplication::run  (32 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcBrowserApplication::run(ID param_1,SEL param_2)

{
  int iVar1;
  
  iVar1 = FUN_042b1ba8(**(undefined8 **)(param_1 + 0x20));
  return (long)iVar1;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:155001 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e6ee0  ArcBrowserApplication::tryToTerminate:  (212 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

unsigned_long_long
ArcBrowserApplication::tryToTerminate_(ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  code *pcVar1;
  uint uVar2;
  undefined8 *puVar3;
  ID *local_58 [2];
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar3 = *(undefined8 **)(param_1 + 0x20);
  local_48 = &LAB_04044e30;
  local_40 = &PTR_FUN_0df6b728;
  local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
  *local_58[0] = param_3;
  uVar2 = FUN_042b2430(*puVar3,local_58);
  if (2 < uVar2) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43e6fb0);
    (*pcVar1)();
  }
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return (ulong)uVar2;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:155043 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e6fc8  ArcBrowserApplication::initialize:  (128 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

long_long ArcBrowserApplication::initialize_(ID param_1,SEL param_2,ID param_3)

{
  int iVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined1 auStack_f0 [104];
  undefined1 auStack_88 [104];
  
  puVar2 = *(undefined8 **)(param_1 + 0x20);
  FUN_043ec404(auStack_f0,param_3);
  uVar3 = *puVar2;
  FUN_0433260c(auStack_88,auStack_f0);
  iVar1 = FUN_042b0850(uVar3,auStack_88);
  FUN_04332524(auStack_88);
  FUN_04332524(auStack_f0);
  return (long)iVar1;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:155069 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e7048  ArcBrowserApplication::createBrowser:withDelegate:forWindow:  (900 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

ID ArcBrowserApplication::createBrowser_withDelegate_forWindow_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  bool bVar1;
  long *plVar2;
  long lVar3;
  ID IVar4;
  __shared_weak_count *p_Var5;
  code *pcVar6;
  undefined8 *puVar7;
  undefined8 uVar8;
  long *plVar9;
  Browser BVar10;
  undefined8 local_b0;
  long *plStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  ulong local_90;
  undefined8 local_88;
  long *local_80;
  ulong local_78;
  undefined8 local_70;
  long *plStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  
  local_70 = 0;
  plStack_68 = (long *)0x0;
  if (param_3 == 0) {
    local_88 = 0;
    local_80 = (long *)0x0;
    local_78 = 0;
    IVar4 = param_5;
LAB_043e73c8:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00(IVar4);
  }
  puVar7 = *(undefined8 **)(param_1 + 0x20);
  p_Var5 = (__shared_weak_count *)param_4;
  IVar4 = _objc_msgSend(param_3,"browser_context",&local_88);
  if ((local_78 & 1) == 0) goto LAB_043e73c8;
  if (param_5 == 0) {
    local_a0 = 0;
    uStack_98 = 0;
    local_90 = 0;
    uVar8 = *puVar7;
LAB_043e711c:
    bVar1 = false;
    uStack_58 = uStack_98;
    local_60 = local_a0;
    local_50 = local_90;
  }
  else {
    FUN_01b356b8(&local_a0,param_5);
    uVar8 = *puVar7;
    if (-1 < (long)local_90) goto LAB_043e711c;
    FUN_00060a60(&local_60,local_a0,uStack_98);
    bVar1 = true;
  }
  FUN_042b32d0(&local_70,uVar8,&local_88,&local_60);
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if (bVar1) {
    FUN_00005810(local_a0,local_90 & 0x7fffffffffffffff);
  }
  if (((char)local_78 != '\0') && (local_80 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_80[1];
    local_80[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_80 + 0x10))(local_80);
      if (local_80[2] != 0) {
        LOAcquire();
        lVar3 = local_80[2];
        local_80[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043e7180;
      }
      pcVar6 = *(code **)(*local_80 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(local_80,0x20);
      }
      else {
        (*pcVar6)(local_80);
      }
    }
  }
LAB_043e7180:
  IVar4 = _objc_alloc(&objc::class_t::ArcBrowser);
  plStack_a8 = plStack_68;
  local_b0 = local_70;
  if (plStack_68 != (long *)0x0) {
    plStack_68[1] = plStack_68[1] + 1;
  }
  BVar10.field0_0x0.field1_0x8 = p_Var5;
  BVar10.field0_0x0.field0_0x0 = (Impl *)&local_b0;
  IVar4 = ArcBrowser::initWithBrowser_(IVar4,"initWithBrowser:",BVar10);
  plVar2 = plStack_a8;
  if (plStack_a8 != (long *)0x0) {
    LOAcquire();
    lVar3 = plStack_a8[1];
    plStack_a8[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar9 = plStack_a8 + 2;
      (**(code **)(*plStack_a8 + 0x10))(plStack_a8);
      if (*plVar9 != 0) {
        LOAcquire();
        lVar3 = plVar2[2];
        plVar2[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043e71d4;
      }
      pcVar6 = *(code **)(*plVar2 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar6)(plVar2);
      }
    }
  }
LAB_043e71d4:
  ArcBrowser::setDelegate_(IVar4,"setDelegate:",param_4);
  plVar2 = plStack_68;
  if (plStack_68 != (long *)0x0) {
    LOAcquire();
    lVar3 = plStack_68[1];
    plStack_68[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar9 = plStack_68 + 2;
      (**(code **)(*plStack_68 + 0x10))(plStack_68);
      if (*plVar9 != 0) {
        LOAcquire();
        lVar3 = plVar2[2];
        plVar2[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) {
          return IVar4;
        }
      }
      pcVar6 = *(code **)(*plVar2 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar6)(plVar2);
      }
    }
  }
  return IVar4;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:155235 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e7528  ArcBrowserApplication::createPopupBrowser:withDelegate:  (720 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcBrowserApplication::createPopupBrowser_withDelegate_
             (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  __shared_weak_count *p_Var4;
  code *pcVar5;
  undefined8 *puVar6;
  long *plVar7;
  Browser BVar8;
  undefined8 local_80;
  long *plStack_78;
  undefined8 local_68;
  long *local_60;
  ulong local_58;
  undefined8 local_50;
  long *plStack_48;
  
  local_50 = 0;
  plStack_48 = (long *)0x0;
  if (param_3 == 0) {
    local_68 = 0;
    local_60 = (long *)0x0;
    local_58 = 0;
    IVar3 = param_4;
LAB_043e77f4:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00(IVar3);
  }
  puVar6 = *(undefined8 **)(param_1 + 0x20);
  p_Var4 = (__shared_weak_count *)param_4;
  IVar3 = _objc_msgSend(param_3,"browser_context",&local_68);
  if ((local_58 & 1) == 0) goto LAB_043e77f4;
  FUN_042b3804(&local_50,*puVar6,&local_68);
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_60[1];
    local_60[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar2 = local_60[2];
        local_60[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043e75b4;
      }
      pcVar5 = *(code **)(*local_60 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar5)(local_60);
      }
    }
  }
LAB_043e75b4:
  IVar3 = _objc_alloc(&objc::class_t::ArcBrowser);
  plStack_78 = plStack_48;
  local_80 = local_50;
  if (plStack_48 != (long *)0x0) {
    plStack_48[1] = plStack_48[1] + 1;
  }
  BVar8.field0_0x0.field1_0x8 = p_Var4;
  BVar8.field0_0x0.field0_0x0 = (Impl *)&local_80;
  IVar3 = ArcBrowser::initWithBrowser_(IVar3,"initWithBrowser:",BVar8);
  plVar1 = plStack_78;
  if (plStack_78 != (long *)0x0) {
    LOAcquire();
    lVar2 = plStack_78[1];
    plStack_78[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar7 = plStack_78 + 2;
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (*plVar7 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043e7608;
      }
      pcVar5 = *(code **)(*plVar1 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar5)(plVar1);
      }
    }
  }
LAB_043e7608:
  ArcBrowser::setDelegate_(IVar3,"setDelegate:",param_4);
  plVar1 = plStack_48;
  if (plStack_48 != (long *)0x0) {
    LOAcquire();
    lVar2 = plStack_48[1];
    plStack_48[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar7 = plStack_48 + 2;
      (**(code **)(*plStack_48 + 0x10))(plStack_48);
      if (*plVar7 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return IVar3;
        }
      }
      pcVar5 = *(code **)(*plVar1 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar5)(plVar1);
      }
    }
  }
  return IVar3;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:155369 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e793c  ArcBrowserApplication::createAIService:  (260 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcBrowserApplication::createAIService_(ID param_1,SEL param_2,ID *param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_40 [8];
  long *local_38;
  
  IVar2 = _objc_alloc(&objc::class_t::ArcAIService);
  FUN_042b3ae0(auStack_40,**(undefined8 **)(param_1 + 0x20));
  IVar2 = ArcAIService::initWithAIService_(IVar2,"initWithAIService:",auStack_40);
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_38[1];
    local_38[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_38 + 0x10))(local_38);
      if (local_38[2] != 0) {
        LOAcquire();
        lVar1 = local_38[2];
        local_38[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return IVar2;
        }
      }
      pcVar3 = *(code **)(*local_38 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_38,0x20);
      }
      else {
        (*pcVar3)(local_38);
      }
    }
  }
  return IVar2;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:155762 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e8368  ArcBrowserApplication::createBrowserContext:isHidden:type:completion:error:  (412 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x34 bytes */

bool ArcBrowserApplication::createBrowserContext_isHidden_type_completion_error_
               (ID param_1,SEL param_2,ID param_3,bool param_4,unsigned_long_long param_5,ID param_6
               ,undefined4 param_7,ID *param_8)

{
  code *pcVar1;
  uint uVar2;
  bool bVar3;
  SEL extraout_x1;
  uint *puVar4;
  undefined8 *puVar5;
  uint *local_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  ulong local_88;
  uint *local_80;
  ID *local_78 [2];
  undefined1 *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar5 = *(undefined8 **)(param_1 + 0x20);
  if ((param_5 != 0) && (param_5 != 1)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43e8504);
    (*pcVar1)();
  }
  local_80 = (uint *)0x0;
  if (param_3 == 0) {
    local_98 = 0;
    uStack_90 = 0;
    local_88 = 0;
  }
  else {
    FUN_01b356b8(&local_98,param_3);
    param_2 = extraout_x1;
  }
  local_78[0] = (ID *)FUN_00008790(8,param_2);
  *local_78[0] = param_6;
  local_68 = &LAB_043eb7a0;
  local_60 = &PTR_FUN_0df6b748;
  FUN_042b3d74(&local_80,*puVar5,param_5,param_4,&local_98,local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78[0]);
  }
  if ((long)local_88 < 0) {
    FUN_00005810(local_98,local_88 & 0x7fffffffffffffff);
  }
  local_a0 = local_80;
  puVar4 = local_80;
  if (((ulong)local_80 & 1) == 0) {
    uVar2 = *local_80;
    *local_80 = uVar2 + 1;
    puVar4 = (uint *)(ulong)uVar2;
  }
  bVar3 = FUN_04402548(puVar4,&local_a0,param_7);
  if (((ulong)local_a0 & 1) == 0) {
    FUN_04b35898();
  }
  if (((ulong)local_80 & 1) == 0) {
    FUN_04b35898();
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
    return bVar3;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:155927 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e8794  ArcBrowserApplication::loadBrowserContext:type:completion:  (320 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

void ArcBrowserApplication::loadBrowserContext_type_completion_
               (ID param_1,SEL param_2,ID param_3,unsigned_long_long param_4,ID param_5,
               undefined4 param_6)

{
  code *pcVar1;
  SEL extraout_x1;
  undefined8 *puVar2;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  ID *local_68 [2];
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar2 = *(undefined8 **)(param_1 + 0x20);
  if ((param_4 != 0) && (param_4 != 1)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43e88d4);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_80 = 0;
    uStack_78 = 0;
    local_70 = 0;
  }
  else {
    FUN_01b356b8(&local_80,param_3);
    param_2 = extraout_x1;
  }
  local_68[0] = (ID *)FUN_00008790(8,param_2);
  *local_68[0] = param_5;
  local_58 = &LAB_043eb7a0;
  local_50 = &PTR_FUN_0df6b768;
  FUN_042b5c28(*puVar2,param_4,&local_80,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if ((long)local_70 < 0) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:156038 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43e8b58  ArcBrowserApplication::deleteBrowserContext:error:  (224 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool ArcBrowserApplication::deleteBrowserContext_error_
               (ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  uint uVar1;
  bool bVar2;
  uint *puVar3;
  undefined8 *puVar4;
  uint *local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  uint *local_38;
  
  local_38 = (uint *)0x0;
  puVar4 = *(undefined8 **)(param_1 + 0x20);
  if (param_3 == 0) {
    local_50 = 0;
    uStack_48 = 0;
    local_40 = 0;
  }
  else {
    FUN_01b356b8(&local_50,param_3);
  }
  FUN_042b6394(&local_38,*puVar4,&local_50);
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  local_58 = local_38;
  puVar3 = local_38;
  if (((ulong)local_38 & 1) == 0) {
    uVar1 = *local_38;
    *local_38 = uVar1 + 1;
    puVar3 = (uint *)(ulong)uVar1;
  }
  bVar2 = FUN_04402548(puVar3,&local_58,param_4);
  if (((ulong)local_58 & 1) == 0) {
    FUN_04b35898();
  }
  if (((ulong)local_38 & 1) == 0) {
    FUN_04b35898();
  }
  return bVar2;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:157156 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43ea810  ArcBrowserApplication::restoreBrowser:withSessionID:browserCreatedCallback:finishedCallback:  (532 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

void ArcBrowserApplication::restoreBrowser_withSessionID_browserCreatedCallback_finishedCallback_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,uint param_6,ID param_7,
               undefined4 param_8)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined8 *puVar4;
  undefined4 local_98;
  undefined1 local_94;
  undefined8 local_90;
  long *local_88;
  ulong local_80;
  ID *local_78 [2];
  undefined1 *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  if (param_3 == 0) {
    local_90 = 0;
    local_88 = (long *)0x0;
    local_80 = 0;
    IVar2 = (ulong)param_6;
  }
  else {
    puVar4 = *(undefined8 **)(param_1 + 0x20);
    IVar2 = _objc_msgSend(param_3,"browser_context",&local_90);
    if ((local_80 & 1) != 0) {
      IVar2 = _objc_msgSend(param_4,"sessionId");
      local_98 = (undefined4)IVar2;
      local_94 = (undefined1)(IVar2 >> 0x20);
      if ((IVar2 >> 0x20 & 1) != 0) {
        local_78[0] = (ID *)FUN_00008790(0x10);
        *local_78[0] = param_5;
        local_78[0][1] = (ulong)param_6;
        local_68 = &LAB_043ec004;
        local_60 = &PTR_FUN_0df6b848;
        FUN_042bdee4(*puVar4,&local_90,&local_98,local_78);
        if ((code *)local_60[1] != (code *)0x0) {
          (*(code *)local_60[1])(local_78[0]);
        }
        if (((char)local_80 != '\0') && (local_88 != (long *)0x0)) {
          LOAcquire();
          lVar1 = local_88[1];
          local_88[1] = lVar1 + -1;
          LORelease();
          if (lVar1 == 0) {
            (**(code **)(*local_88 + 0x10))(local_88);
            if (local_88[2] != 0) {
              LOAcquire();
              lVar1 = local_88[2];
              local_88[2] = lVar1 + -1;
              LORelease();
              if (lVar1 != 0) goto LAB_043ea93c;
            }
            pcVar3 = *(code **)(*local_88 + 0x20);
            if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40))
            {
              FUN_00005810(local_88,0x20);
            }
            else {
              (*pcVar3)(local_88);
            }
          }
        }
LAB_043ea93c:
        if (*(long *)PTR____stack_chk_guard_0d824020 != local_58) {
                    /* WARNING: Subroutine does not return */
          ___stack_chk_fail(0);
        }
        return;
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00(IVar2);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:158379 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43ecc2c  ArcBrowserContextAutocomplete::autocomplete:cursorPosition:preventInlineAutocomplete:completion:  (392 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x2c bytes */

void ArcBrowserContextAutocomplete::
     autocomplete_cursorPosition_preventInlineAutocomplete_completion_
               (ID param_1,SEL param_2,ID param_3,long_long param_4,bool param_5,ID param_6,
               undefined4 param_7)

{
  ID *pIVar1;
  ID *pIVar2;
  undefined **ppuVar3;
  long *plVar4;
  undefined8 local_90;
  undefined8 uStack_88;
  ulong local_80;
  ID *local_78;
  undefined8 uStack_70;
  code *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  pIVar1 = (ID *)FUN_00008790(8);
  *pIVar1 = param_6;
  plVar4 = *(long **)(param_1 + 8);
  if (param_3 == 0) {
    local_90 = 0;
    uStack_88 = 0;
    local_80 = 0;
  }
  else {
    FUN_01b356b8(&local_90,param_3);
  }
  pIVar2 = (ID *)FUN_00008790(8);
  *pIVar2 = *pIVar1;
  ppuVar3 = &PTR_FUN_0df6b868;
  if (*plVar4 != 0) {
    uStack_70 = 0;
    local_68 = FUN_043eceb0;
    local_60 = &PTR_FUN_0df6b868;
    local_78 = pIVar2;
    FUN_042c35e4(*plVar4,&local_90,param_4,param_5,&local_78);
    if ((code *)local_60[1] != (code *)0x0) {
      (*(code *)local_60[1])(local_78);
    }
    ppuVar3 = (undefined **)&DAT_0c4b3500;
  }
  if ((code *)ppuVar3[1] != (code *)0x0) {
    (*(code *)ppuVar3[1])(pIVar2);
  }
  if ((long)local_80 < 0) {
    FUN_00005810(local_90,local_80 & 0x7fffffffffffffff);
  }
  if (pIVar1 != (ID *)0x0) {
    FUN_00005810(pIVar1,8);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:159676 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43eeb48  objc::Categories::ArcBrowserContext_Extensions_::setPermissionsForOrigin:withPermissionTypes:withSiteSetting:  (748 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     setPermissionsForOrigin_withPermissionTypes_withSiteSetting_
               (ID param_1,SEL param_2,ID param_3,ID param_4,unsigned_long_long param_5)

{
  ulong uVar1;
  bool bVar2;
  undefined4 *puVar3;
  ulong uVar4;
  code *pcVar5;
  ID IVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  long lVar9;
  size_t sVar10;
  ulong uVar11;
  undefined4 *puVar12;
  long lVar13;
  undefined4 *local_a8;
  ulong local_a0;
  ulong local_98;
  undefined8 local_90 [2];
  undefined7 local_80;
  char cStack_79;
  undefined1 auStack_78 [8];
  long *local_70;
  byte local_68;
  
  _objc_msgSend(param_1,"browser_context",auStack_78);
  if ((local_68 & 1) != 0) {
    FUN_043abdf8(local_90,param_3);
    local_a8 = (undefined4 *)0x0;
    local_a0 = 0;
    local_98 = 0;
    puVar12 = &DAT_0c9d7f60;
    lVar13 = 0x1d0;
    do {
      IVar6 = _objc_msgSend(param_4,"containsIndex:",*(undefined8 *)(puVar12 + -2));
      uVar4 = local_a0;
      if ((int)IVar6 != 0) {
        if (local_a0 == local_98) {
          uVar1 = local_a0 + 1;
          if (uVar1 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
            FUN_040416c0();
          }
          uVar11 = local_a0 * 2;
          if (uVar11 < uVar1 || uVar11 - uVar1 == 0) {
            uVar11 = uVar1;
          }
          if (0x1ffffffffffffffe < local_a0) {
            uVar11 = 0x3fffffffffffffff;
          }
          if (uVar11 == 0) {
            puVar7 = (undefined4 *)0x0;
          }
          else {
            if (uVar11 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
              FUN_04b087f0();
            }
            puVar7 = (undefined4 *)FUN_00008790(uVar11 << 2);
          }
          puVar8 = puVar7 + uVar4;
          if (puVar8 == (undefined4 *)0x0) goto LAB_043eec3c;
          *puVar8 = *puVar12;
          puVar8 = puVar8 + -local_a0;
          sVar10 = local_a0 * 4;
          _memcpy(puVar8,local_a8,sVar10);
          puVar3 = local_a8;
          local_a0 = (long)(sVar10 + 4) >> 2;
          bVar2 = local_a8 != (undefined4 *)0x0;
          local_a8 = puVar8;
          if (bVar2) {
            lVar9 = local_98 << 2;
            local_98 = uVar11;
            FUN_00005810(puVar3,lVar9);
            uVar11 = local_98;
          }
        }
        else {
          if (local_a8 + local_a0 == (undefined4 *)0x0) goto LAB_043eec3c;
          local_a8[local_a0] = *puVar12;
          puVar7 = local_a8;
          uVar11 = local_98;
        }
        local_98 = uVar11;
        lVar9 = (long)puVar7 + ((uVar4 * 4 + 4) - (long)local_a8);
        local_a0 = lVar9 >> 2;
        if (lVar9 == 0) goto LAB_043eec3c;
      }
      puVar12 = puVar12 + 4;
      lVar13 = lVar13 + -0x10;
    } while (lVar13 != 0);
    if (4 < param_5) {
                    /* WARNING: Does not return */
      pcVar5 = (code *)SoftwareBreakpoint(0x550b,0x43eef60);
      (*pcVar5)();
    }
    FUN_04333920(auStack_78,local_90,&local_a8,param_5);
    if (local_a8 != (undefined4 *)0x0) {
      if ((local_a0 & 0x3fffffffffffffff) != 0) {
        lVar13 = local_a0 * -4;
        puVar12 = local_a8 + local_a0;
        do {
          puVar12 = puVar12 + -1;
          if (puVar12 == (undefined4 *)0x0) goto LAB_043eec3c;
          lVar13 = lVar13 + 4;
        } while (lVar13 != 0);
      }
      local_a0 = 0;
      FUN_00005810(local_a8,local_98 << 2);
    }
    if (cStack_79 < '\0') {
      FUN_00005810(local_90[0],CONCAT17(cStack_79,local_80) & 0x7fffffffffffffff);
    }
    if ((local_68 != 0) && (local_70 != (long *)0x0)) {
      LOAcquire();
      lVar13 = local_70[1];
      local_70[1] = lVar13 + -1;
      LORelease();
      if (lVar13 == 0) {
        (**(code **)(*local_70 + 0x10))(local_70);
        if (local_70[2] != 0) {
          LOAcquire();
          lVar13 = local_70[2];
          local_70[2] = lVar13 + -1;
          LORelease();
          if (lVar13 != 0) {
            return;
          }
        }
        pcVar5 = *(code **)(*local_70 + 0x20);
        if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
          FUN_00005810(local_70,0x20);
        }
        else {
          (*pcVar5)(local_70);
        }
      }
    }
    return;
  }
LAB_043eec3c:
                    /* WARNING: Does not return */
  pcVar5 = (code *)SoftwareBreakpoint(1,0x43eec40);
  (*pcVar5)();
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:160109 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43ef75c  objc::Categories::ArcBrowserContext_Extensions_::getBookmarksTree  (476 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::getBookmarksTree(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  undefined8 local_60;
  long *local_58;
  byte local_50;
  undefined8 local_48;
  long *local_40;
  ulong local_38;
  
  local_48 = 0;
  local_40 = (long *)0x0;
  local_38 = 0;
  _objc_msgSend(param_1,"browser_context",&local_60);
  if ((local_50 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43ef934);
    (*pcVar4)();
  }
  FUN_042c9f90(&local_48,local_60);
  if ((local_50 != 0) && (local_58 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_58[1];
    local_58[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar2 = local_58[2];
        local_58[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043ef7bc;
      }
      pcVar4 = *(code **)(*local_58 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar4)(local_58);
      }
    }
  }
LAB_043ef7bc:
  if ((char)local_38 == '\0') {
    IVar3 = 0;
  }
  else {
    IVar3 = _objc_alloc(&class_t::ArcBookmarkTreeNode);
    if ((local_38 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04b08a00();
    }
    IVar3 = ArcBookmarkTreeNode::initWithBookmarkTreeNode_
                      (IVar3,"initWithBookmarkTreeNode:",&local_48);
    plVar1 = local_40;
    if (((char)local_38 != '\0') && (local_40 != (long *)0x0)) {
      LOAcquire();
      lVar2 = local_40[1];
      local_40[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        plVar5 = local_40 + 2;
        (**(code **)(*local_40 + 0x10))(local_40);
        if (*plVar5 != 0) {
          LOAcquire();
          lVar2 = plVar1[2];
          plVar1[2] = lVar2 + -1;
          LORelease();
          if (lVar2 != 0) {
            return IVar3;
          }
        }
        pcVar4 = *(code **)(*plVar1 + 0x20);
        if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
          FUN_00005810(plVar1,0x20);
        }
        else {
          (*pcVar4)(plVar1);
        }
      }
    }
  }
  return IVar3;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:161982 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43f2ca8  objc::Categories::ArcBrowserContext_Extensions_::getAllCookies:completion:  (476 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::getAllCookies_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_98 [2];
  undefined7 local_88;
  char cStack_81;
  undefined8 local_80;
  long *local_78;
  byte local_70;
  ID *local_68 [2];
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_80,param_3,param_4,(ulong)param_5);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f2e80);
    (*pcVar2)();
  }
  FUN_043abdf8(local_98,param_3);
  local_58 = FUN_043e5ef8;
  local_50 = &PTR_FUN_0df6b9a8;
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_4;
  FUN_042cba00(local_80,local_98,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if (cStack_81 < '\0') {
    FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  if ((local_70 != 0) && (local_78 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_78 + 0x10))(local_78);
      if (local_78[2] != 0) {
        LOAcquire();
        lVar1 = local_78[2];
        local_78[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f2db8;
      }
      pcVar2 = *(code **)(*local_78 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_78,0x20);
      }
      else {
        (*pcVar2)(local_78);
      }
    }
  }
LAB_043f2db8:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:162057 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43f2f30  objc::Categories::ArcBrowserContext_Extensions_::getLocalStorage:key:completion:  (568 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::getLocalStorage_key_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,undefined4 param_6)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_b0;
  undefined8 uStack_a8;
  ulong local_a0;
  undefined8 local_98 [2];
  undefined7 local_88;
  char cStack_81;
  undefined8 local_80;
  long *local_78;
  byte local_70;
  ID *local_68 [2];
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_80);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f3164);
    (*pcVar2)();
  }
  FUN_043abdf8(local_98,param_3);
  if (param_4 == 0) {
    local_b0 = 0;
    uStack_a8 = 0;
    local_a0 = 0;
  }
  else {
    FUN_01b356b8(&local_b0,param_4);
  }
  local_58 = FUN_043f88dc;
  local_50 = &PTR_FUN_0df6b9c8;
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_5;
  FUN_042cc3f0(local_80,local_98,&local_b0,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if ((long)local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 & 0x7fffffffffffffff);
  }
  if (cStack_81 < '\0') {
    FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  if ((local_70 != 0) && (local_78 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_78 + 0x10))(local_78);
      if (local_78[2] != 0) {
        LOAcquire();
        lVar1 = local_78[2];
        local_78[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f3094;
      }
      pcVar2 = *(code **)(*local_78 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_78,0x20);
      }
      else {
        (*pcVar2)(local_78);
      }
    }
  }
LAB_043f3094:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:162250 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43f35bc  objc::Categories::ArcBrowserContext_Extensions_::createWebContents:  (508 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::createWebContents_
             (ID param_1,SEL param_2,ID param_3)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 local_58;
  long *local_50;
  byte local_48;
  undefined8 local_40;
  long *local_38;
  
  local_40 = 0;
  local_38 = (long *)0x0;
  _objc_msgSend(param_1,"browser_context",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43f37b8);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_70 = 0;
    uStack_68 = 0;
    local_60 = 0;
  }
  else {
    _objc_msgSend(param_3,"nativeWebContentsConfiguration",&local_70);
  }
  FUN_042d7064(&local_40,local_58,&local_70);
  if ((local_48 != 0) && (local_50 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_50 + 0x10))(local_50);
      if (local_50[2] != 0) {
        LOAcquire();
        lVar2 = local_50[2];
        local_50[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f3648;
      }
      pcVar4 = *(code **)(*local_50 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_50,0x20);
      }
      else {
        (*pcVar4)(local_50);
      }
    }
  }
LAB_043f3648:
  IVar3 = _objc_alloc(&class_t::ArcWebContents);
  IVar3 = ArcWebContents::initWithWebContents_(IVar3,"initWithWebContents:",&local_40);
  plVar1 = local_38;
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_38[1];
    local_38[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = local_38 + 2;
      (**(code **)(*local_38 + 0x10))(local_38);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return IVar3;
        }
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
  return IVar3;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:162640 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43f4390  objc::Categories::ArcBrowserContext_Extensions_::importPasswords:  (340 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::importPasswords_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  undefined8 local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f44e4);
    (*pcVar2)();
  }
  if (param_3 == 0) {
    local_60 = 0;
    uStack_58 = 0;
    local_50 = 0;
  }
  else {
    FUN_01b356b8(&local_60,param_3);
  }
  FUN_042d5d2c(local_48,&local_60);
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_40[1];
    local_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar2 = *(code **)(*local_40 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar2)(local_40);
      }
    }
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:163152 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43f541c  objc::Categories::ArcBrowserContext_Extensions_::id  (360 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::id(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long local_68;
  long *local_60;
  byte local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  undefined4 local_38;
  
  IVar2 = _objc_alloc(&class_t::ArcBrowserContextID);
  _objc_msgSend(param_1,"browser_context",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f5584);
    (*pcVar3)();
  }
  if (*(char *)(local_68 + 0xa7) < '\0') {
    FUN_00060a60(&local_50,*(undefined8 *)(local_68 + 0x90),*(undefined8 *)(local_68 + 0x98));
  }
  else {
    uStack_48 = *(undefined8 *)(local_68 + 0x98);
    local_50 = *(undefined8 *)(local_68 + 0x90);
    local_40 = *(ulong *)(local_68 + 0xa0);
  }
  local_38 = *(undefined4 *)(local_68 + 0xa8);
  IVar2 = ArcBrowserContextID::initWithBrowserContextID_
                    (IVar2,"initWithBrowserContextID:",&local_50);
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_60[1];
    local_60[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar1 = local_60[2];
        local_60[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return IVar2;
        }
      }
      pcVar3 = *(code **)(*local_60 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar3)(local_60);
      }
    }
  }
  return IVar2;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:163394 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43f5cd4  objc::Categories::ArcBrowserContext_Extensions_::productConnectionClosed:withReason:  (448 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::productConnectionClosed_withReason_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long lVar1;
  undefined8 uVar2;
  code *pcVar3;
  undefined8 local_78;
  undefined8 uStack_70;
  ulong local_68;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  long local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f5e94);
    (*pcVar3)();
  }
  if (param_3 == 0) {
    local_60 = 0;
    uStack_58 = 0;
    local_50 = 0;
  }
  else {
    FUN_01b356b8(&local_60,param_3);
  }
  if (param_4 == 0) {
    local_78 = 0;
    uStack_70 = 0;
    local_68 = 0;
  }
  else {
    FUN_01b356b8(&local_78,param_4);
  }
  if (*(long *)(local_48 + 0x88) != 0) {
    uVar2 = FUN_04135760(*(undefined8 *)(*(long *)(local_48 + 0x88) + 0x18));
    FUN_0416cc3c(uVar2,&local_60,&local_78);
  }
  if ((long)local_68 < 0) {
    FUN_00005810(local_78,local_68 & 0x7fffffffffffffff);
  }
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_40[1];
    local_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar3 = *(code **)(*local_40 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar3)(local_40);
      }
    }
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:163478 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43f5f60  objc::Categories::ArcBrowserContext_Extensions_::createAutocompleteControllerWithProviders:  (352 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::createAutocompleteControllerWithProviders_
             (ID param_1,SEL param_2,unsigned_long_long param_3)

{
  long lVar1;
  undefined8 uVar2;
  unique_ptr<arc::ArcAutocompleteController,_std::default_delete<arc::ArcAutocompleteController>>
  uVar3;
  ID IVar4;
  code *pcVar5;
  long local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x43f60c0);
    (*pcVar5)();
  }
  uVar2 = FUN_00008790(0x78);
  FUN_042c2f5c(uVar2,*(undefined8 *)(local_48 + 0x88),(uint)param_3 & 3);
  uVar3.field0_0x0 = (ArcAutocompleteController *)FUN_00008790(8);
  *(undefined8 *)uVar3.field0_0x0 = uVar2;
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_40[1];
    local_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f5fdc;
      }
      pcVar5 = *(code **)(*local_40 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar5)(local_40);
      }
    }
  }
LAB_043f5fdc:
  IVar4 = _objc_alloc(&class_t::ArcBrowserContextAutocomplete);
  if (IVar4 == 0) {
    if (uVar3.field0_0x0 != (ArcAutocompleteController *)0x0) {
      lVar1 = *(long *)uVar3.field0_0x0;
      *(undefined8 *)uVar3.field0_0x0 = 0;
      if (lVar1 != 0) {
        FUN_042c33cc();
      }
      FUN_00005810(uVar3.field0_0x0,8);
    }
    IVar4 = 0;
  }
  else {
    IVar4 = ArcBrowserContextAutocomplete::initWithAutocompleteController_
                      (IVar4,"initWithAutocompleteController:",uVar3);
  }
  return IVar4;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:164931 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43f9a98  objc::Categories::ArcBrowserContextPageContentExtraction_Extensions_::getExtractedContentForWebContents:completion:  (620 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContextPageContentExtraction_Extensions_::
     getExtractedContentForWebContents_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_88;
  long *local_80;
  ulong local_78;
  undefined8 local_70;
  long *local_68;
  byte local_60;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context_page_content_extraction",&local_70,param_3,param_4,
                (ulong)param_5);
  if ((local_60 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f9cf4);
    (*pcVar2)();
  }
  if (param_3 == 0) {
    local_88 = 0;
    local_80 = (long *)0x0;
    local_78 = 0;
LAB_043f9cfc:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  _objc_msgSend(param_3,"web_contents",&local_88);
  if ((local_78 & 1) == 0) goto LAB_043f9cfc;
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_4;
  local_48 = FUN_043f9e60;
  local_40 = &PTR_FUN_0df6bac8;
  FUN_042f0254(local_70,&local_88,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (((char)local_78 != '\0') && (local_80 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_80[1];
    local_80[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_80 + 0x10))(local_80);
      if (local_80[2] != 0) {
        LOAcquire();
        lVar1 = local_80[2];
        local_80[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f9b8c;
      }
      pcVar2 = *(code **)(*local_80 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_80,0x20);
      }
      else {
        (*pcVar2)(local_80);
      }
    }
  }
LAB_043f9b8c:
  if ((local_60 != 0) && (local_68 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_68[1];
    local_68[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_68 + 0x10))(local_68);
      if (local_68[2] != 0) {
        LOAcquire();
        lVar1 = local_68[2];
        local_68[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f9bac;
      }
      pcVar2 = *(code **)(*local_68 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_68,0x20);
      }
      else {
        (*pcVar2)(local_68);
      }
    }
  }
LAB_043f9bac:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_38) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:167047 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43fd304  ArcBrowserContextExtensions::installExtensionFromWebstore:unattended:completion:  (360 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x24 bytes */

void ArcBrowserContextExtensions::installExtensionFromWebstore_unattended_completion_
               (ID param_1,SEL param_2,ID param_3,bool param_4,ID param_5,undefined4 param_6)

{
  code *pcVar1;
  SEL extraout_x1;
  undefined8 uVar2;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  ID *local_68 [2];
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x30) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x43fd468);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_80 = 0;
    uStack_78 = 0;
    local_70 = 0;
  }
  else {
    FUN_01b356b8(&local_80,param_3);
    param_2 = extraout_x1;
  }
  uVar2 = *(undefined8 *)(param_1 + 0x20);
  local_58 = &LAB_043fec84;
  local_50 = &PTR_FUN_0df6bae8;
  local_68[0] = (ID *)FUN_00008790(0x10,param_2);
  *local_68[0] = param_3;
  local_68[0][1] = param_5;
  FUN_042e6790(uVar2,&local_80,param_4,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if ((long)local_70 < 0) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:167270 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43fd6e8  ArcBrowserContextExtensions::runJSInExtensionBackgroundPage:js_code:  (256 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool ArcBrowserContextExtensions::runJSInExtensionBackgroundPage_js_code_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  code *pcVar1;
  bool bVar2;
  SEL extraout_x1;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  undefined8 local_48;
  undefined8 uStack_40;
  ulong local_38;
  
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    if (param_3 == 0) {
      local_48 = 0;
      uStack_40 = 0;
      local_38 = 0;
    }
    else {
      FUN_01b356b8(&local_48,param_3);
      param_2 = extraout_x1;
    }
    if (param_4 == 0) {
      local_60 = 0;
      uStack_58 = 0;
      local_50 = 0;
    }
    else {
      FUN_01b356b8(&local_60,param_4,param_2);
    }
    bVar2 = FUN_042e8148(*(undefined8 *)(param_1 + 0x20),&local_48,&local_60);
    if ((long)local_50 < 0) {
      FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
    }
    if ((long)local_38 < 0) {
      FUN_00005810(local_48,local_38 & 0x7fffffffffffffff);
    }
    return bVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43fd7e8);
  (*pcVar1)();
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:167323 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43fd80c  ArcBrowserContextExtensions::getAllExtensionCommands  (364 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContextExtensions::getAllExtensionCommands(ID param_1,SEL param_2)

{
  const_void__ pvVar1;
  code *pcVar2;
  ID IVar3;
  ID IVar4;
  undefined1 *puVar5;
  long lVar6;
  const_void__ local_68;
  long local_60;
  long local_58;
  
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    local_68 = (const_void__)0x0;
    local_60 = 0;
    local_58 = 0;
    FUN_042eaf60(&local_68,*(undefined8 *)(param_1 + 0x20));
    IVar3 = NSMutableArray::new((ID)&_OBJC_CLASS___NSMutableArray,"new");
    pvVar1 = local_68;
    for (lVar6 = local_60 * 0x70; lVar6 != 0; lVar6 = lVar6 + -0x70) {
      IVar4 = _objc_alloc(&objc::class_t::ArcExtensionCommand);
      IVar4 = ArcExtensionCommand::initWithExtensionCommand_
                        (IVar4,"initWithExtensionCommand:",pvVar1);
      NSMutableArray::addObject_(IVar3,"addObject:",IVar4);
      pvVar1 = (const_void__)((long)pvVar1 + 0x70);
    }
    IVar3 = NSMutableArray::copy(IVar3,"copy");
    pvVar1 = local_68;
    if (local_68 != (const_void__)0x0) {
      if (local_60 * 0x70 != 0) {
        puVar5 = (undefined1 *)(local_60 * 0x70 + (long)local_68);
        lVar6 = local_60 * -0x70;
        do {
          if (puVar5 == section_00000068.sectname + 8) goto LAB_043fd988;
          puVar5 = (undefined1 *)FUN_0441e178();
          lVar6 = lVar6 + 0x70;
        } while (lVar6 != 0);
      }
      local_60 = ((long)pvVar1 - (long)local_68 >> 4) * 0x6db6db6db6db6db7;
      FUN_00005810(local_68,local_58 * 0x70);
    }
    return IVar3;
  }
LAB_043fd988:
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x43fd98c);
  (*pcVar2)();
}



/* SOURCE /home/exedev/work/inputs/decompile/0x43.c:167506 sha256 d67c5b68938849e6e87359f524c5da11f8d201d030901dc4f27ddc61604f3957 */
// ==== 0x43fdd3c  ArcBrowserContextExtensions::setPinnedExtensionOrder:  (932 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserContextExtensions::setPinnedExtensionOrder_(ID param_1,SEL param_2,ID param_3)

{
  ulong uVar1;
  bool bVar2;
  code *pcVar3;
  ID IVar4;
  undefined8 *puVar5;
  ulong uVar6;
  undefined8 *puVar7;
  undefined8 *puVar8;
  long lVar9;
  ulong uVar10;
  ulong uVar11;
  long lVar12;
  undefined8 local_168;
  undefined8 uStack_160;
  ulong local_158;
  undefined8 local_150;
  long lStack_148;
  long *local_140;
  undefined8 uStack_138;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined8 *local_108;
  ulong local_100;
  ulong local_f8;
  undefined1 auStack_f0 [128];
  long local_70;
  
  local_70 = *(long *)PTR____stack_chk_guard_0d824020;
  local_108 = (undefined8 *)0x0;
  local_100 = 0;
  local_f8 = 0;
  IVar4 = _objc_msgSend(param_3,"count");
  puVar7 = (undefined8 *)0x0;
  if (IVar4 != 0) {
    if (0xaaaaaaaaaaaaaaa < IVar4) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    puVar7 = (undefined8 *)FUN_00008790(IVar4 * 0x18);
    local_100 = 0;
    local_108 = puVar7;
    local_f8 = IVar4;
  }
  uStack_128 = 0;
  local_130 = 0;
  uStack_118 = 0;
  uStack_120 = 0;
  lStack_148 = 0;
  local_150 = 0;
  uStack_138 = 0;
  local_140 = (long *)0x0;
  IVar4 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_150,auStack_f0,
                        0x10);
  if (IVar4 != 0) {
    uVar10 = 0;
    uVar11 = 0;
    lVar12 = *local_140;
    lVar9 = lVar12;
    do {
      if (lVar9 != lVar12) {
        _objc_enumerationMutation(param_3);
      }
      lVar9 = *(long *)(lStack_148 + uVar11 * 8);
      if (lVar9 == 0) {
        local_168 = 0;
        uStack_160 = 0;
        local_158 = 0;
      }
      else {
        FUN_01b356b8(&local_168,lVar9);
      }
      if (uVar10 == local_f8) {
        uVar1 = uVar10 + 1;
        if (0xaaaaaaaaaaaaaaa < uVar1) {
                    /* WARNING: Subroutine does not return */
          FUN_04047380(0);
        }
        uVar6 = uVar10 * 2;
        if (uVar6 < uVar1 || uVar6 - uVar1 == 0) {
          uVar6 = uVar1;
        }
        if (0x555555555555554 < uVar10) {
          uVar6 = 0xaaaaaaaaaaaaaaa;
        }
        if (uVar6 == 0) {
          puVar7 = (undefined8 *)0x0;
        }
        else {
          if (0xaaaaaaaaaaaaaaa < uVar6) {
                    /* WARNING: Subroutine does not return */
            FUN_04b087f0();
          }
          puVar7 = (undefined8 *)FUN_00008790(uVar6 * 0x18);
        }
        puVar5 = puVar7 + uVar10 * 3;
        if (puVar5 == (undefined8 *)0x0) goto LAB_043fe0bc;
        puVar5[2] = local_158;
        puVar5[1] = uStack_160;
        *puVar5 = local_168;
        uStack_160 = 0;
        local_158 = 0;
        local_168 = 0;
        puVar5 = puVar5 + local_100 * -3;
        if (local_100 * 0x18 != 0) {
          _memcpy(puVar5,local_108,local_100 * 0x18);
        }
        puVar8 = local_108;
        bVar2 = local_108 != (undefined8 *)0x0;
        local_108 = puVar5;
        if (bVar2) {
          lVar9 = local_f8 * 0x18;
          local_f8 = uVar6;
          FUN_00005810(puVar8,lVar9);
          uVar6 = local_f8;
        }
      }
      else {
        puVar5 = puVar7 + uVar10 * 3;
        if (puVar5 == (undefined8 *)0x0) goto LAB_043fe0bc;
        puVar5[2] = local_158;
        puVar5[1] = uStack_160;
        *puVar5 = local_168;
        uStack_160 = 0;
        local_158 = 0;
        local_168 = 0;
        uVar6 = local_f8;
      }
      local_f8 = uVar6;
      puVar5 = local_108;
      lVar9 = (long)puVar7 + ((uVar10 * 0x18 + 0x18) - (long)local_108);
      uVar10 = (lVar9 >> 3) * -0x5555555555555555;
      local_100 = uVar10;
      if (lVar9 == 0) goto LAB_043fe0bc;
      if ((long)local_158 < 0) {
        FUN_00005810(local_168,local_158 & 0x7fffffffffffffff);
      }
      uVar11 = uVar11 + 1;
      if (IVar4 <= uVar11) {
        IVar4 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_150,
                              auStack_f0,0x10);
        if (IVar4 == 0) break;
        uVar11 = 0;
      }
      lVar9 = *local_140;
      puVar7 = puVar5;
    } while( true );
  }
  if ((*(byte *)(param_1 + 0x30) & 1) == 0) {
LAB_043fe0bc:
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43fe0c0);
    (*pcVar3)();
  }
  FUN_042eb57c(*(undefined8 *)(param_1 + 0x20),&local_108);
  puVar7 = local_108;
  if (local_108 != (undefined8 *)0x0) {
    if (local_100 * 0x18 != 0) {
      lVar9 = local_100 * -0x18;
      puVar5 = local_108 + local_100 * 3;
      do {
        puVar8 = puVar5 + -3;
        if (puVar8 == (undefined8 *)0x0) goto LAB_043fe0bc;
        if (*(char *)((long)puVar5 + -1) < '\0') {
          FUN_00005810(*puVar8,puVar5[-1] & 0x7fffffffffffffff);
        }
        lVar9 = lVar9 + 0x18;
        puVar5 = puVar8;
      } while (lVar9 != 0);
    }
    local_100 = ((long)puVar7 - (long)local_108 >> 3) * -0x5555555555555555;
    FUN_00005810(local_108,local_f8 * 0x18);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_70) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:14167 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x4410cd0  objc::Categories::ArcWebContents_Extensions_::attachToPaneHostView:  (316 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcWebContents_Extensions_::attachToPaneHostView_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  code *pcVar2;
  long local_50;
  long *local_48;
  byte local_40;
  undefined8 local_38;
  
  if (param_3 != 0) {
    _objc_msgSend(param_1,"web_contents",&local_50);
    if ((local_40 & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x4410e0c);
      (*pcVar2)();
    }
    FUN_04376580(local_50);
    local_38 = *(undefined8 *)(local_50 + 0x288);
    FUN_04739114(param_3,&local_38);
    if ((local_40 != 0) && (local_48 != (long *)0x0)) {
      LOAcquire();
      lVar1 = local_48[1];
      local_48[1] = lVar1 + -1;
      LORelease();
      if (lVar1 == 0) {
        (**(code **)(*local_48 + 0x10))(local_48);
        if (local_48[2] != 0) {
          LOAcquire();
          lVar1 = local_48[2];
          local_48[2] = lVar1 + -1;
          LORelease();
          if (lVar1 != 0) {
            return;
          }
        }
        pcVar2 = *(code **)(*local_48 + 0x20);
        if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
          FUN_00005810(local_48,0x20);
        }
        else {
          (*pcVar2)(local_48);
        }
      }
    }
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:14224 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x4410ec0  objc::Categories::ArcWebContents_Extensions_::detachFromPaneHostView:  (16 bytes)

/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcWebContents_Extensions_::detachFromPaneHostView_
               (ID param_1,SEL param_2,ID param_3)

{
  if (param_3 != 0) {
    FUN_047391c8(param_3);
    return;
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:14325 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x4411214  objc::Categories::ArcWebContents_Extensions_::devTools  (712 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcWebContents_Extensions_::devTools(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  __shared_weak_count *in_x3;
  code *pcVar4;
  long *plVar5;
  long lVar6;
  undefined8 uVar7;
  long *plVar8;
  DevTools DVar9;
  undefined8 local_88;
  long *local_80;
  long local_78;
  long *local_70;
  byte local_68;
  undefined8 local_60;
  long *plStack_58;
  
  IVar3 = _objc_getAssociatedObject(param_1,"devTools");
  if (IVar3 != 0) {
    return IVar3;
  }
  _objc_msgSend(param_1,"web_contents",&local_78);
  if ((local_68 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x44114dc);
    (*pcVar4)();
  }
  lVar6 = *(long *)(local_78 + 0x2b8);
  if (lVar6 == 0) {
    plVar5 = (long *)0x0;
    uVar7 = 0;
LAB_044112bc:
    if (local_70 != (long *)0x0) {
      LOAcquire();
      lVar2 = local_70[1];
      local_70[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        (**(code **)(*local_70 + 0x10))(local_70);
        if (local_70[2] != 0) {
          LOAcquire();
          lVar2 = local_70[2];
          local_70[2] = lVar2 + -1;
          LORelease();
          if (lVar2 != 0) goto LAB_044112d4;
        }
        pcVar4 = *(code **)(*local_70 + 0x20);
        if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
          FUN_00005810(local_70,0x20);
        }
        else {
          (*pcVar4)(local_70);
        }
      }
    }
LAB_044112d4:
    if (lVar6 == 0) {
      return 0;
    }
  }
  else {
    FUN_042c8400(&local_60,lVar6);
    plVar5 = plStack_58;
    uVar7 = local_60;
    if ((local_68 & 1) != 0) goto LAB_044112bc;
  }
  IVar3 = _objc_alloc(&class_t::ArcWebContentsDevTools);
  if (plVar5 != (long *)0x0) {
    plVar5[1] = plVar5[1] + 1;
  }
  DVar9.field0_0x0.field1_0x8 = in_x3;
  DVar9.field0_0x0.field0_0x0 = (Impl *)&local_88;
  local_88 = uVar7;
  local_80 = plVar5;
  IVar3 = ArcWebContentsDevTools::init_(IVar3,"init:",DVar9);
  plVar1 = local_80;
  if (local_80 != (long *)0x0) {
    LOAcquire();
    lVar6 = local_80[1];
    local_80[1] = lVar6 + -1;
    LORelease();
    if (lVar6 == 0) {
      plVar8 = local_80 + 2;
      (**(code **)(*local_80 + 0x10))(local_80);
      if (*plVar8 != 0) {
        LOAcquire();
        lVar6 = plVar1[2];
        plVar1[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) goto LAB_04411324;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_04411324:
  _objc_setAssociatedObject(param_1,"devTools",IVar3,1);
  if (plVar5 != (long *)0x0) {
    LOAcquire();
    lVar6 = plVar5[1];
    plVar5[1] = lVar6 + -1;
    LORelease();
    if (lVar6 == 0) {
      (**(code **)(*plVar5 + 0x10))(plVar5);
      if (plVar5[2] != 0) {
        LOAcquire();
        lVar6 = plVar5[2];
        plVar5[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) {
          return IVar3;
        }
      }
      pcVar4 = *(code **)(*plVar5 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar5,0x20);
      }
      else {
        (*pcVar4)(plVar5);
      }
    }
  }
  return IVar3;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:14467 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x441161c  objc::Categories::ArcWebContents_Extensions_::mediaSession  (516 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcWebContents_Extensions_::mediaSession(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  ID in_x4;
  code *pcVar3;
  MediaSession MVar4;
  long local_68;
  long *local_60;
  byte local_58;
  Impl aIStack_50 [8];
  long *local_48;
  
  IVar2 = _objc_getAssociatedObject(param_1,"mediaSession");
  if (IVar2 != 0) {
    return IVar2;
  }
  IVar2 = _objc_alloc(&class_t::ArcWebContentsMediaSession);
  _objc_msgSend(param_1,"web_contents",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4411820);
    (*pcVar3)();
  }
  FUN_042c8400(aIStack_50,*(undefined8 *)(local_68 + 0x2c8));
  MVar4.field0_0x0.field1_0x8 = (__shared_weak_count *)param_1;
  MVar4.field0_0x0.field0_0x0 = aIStack_50;
  IVar2 = ArcWebContentsMediaSession::init_forWebContents_(IVar2,"init:forWebContents:",MVar4,in_x4)
  ;
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_044116c4;
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
LAB_044116c4:
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_60[1];
    local_60[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar1 = local_60[2];
        local_60[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_044116e4;
      }
      pcVar3 = *(code **)(*local_60 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar3)(local_60);
      }
    }
  }
LAB_044116e4:
  _objc_setAssociatedObject(param_1,"mediaSession",IVar2,1);
  return IVar2;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:39325 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x442fcd0  ArcDownloadItem::remainingTimeInSeconds  (144 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcDownloadItem::remainingTimeInSeconds(ID param_1,SEL param_2)

{
  code *pcVar1;
  ulong local_20;
  long_long local_18;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fd54);
    (*pcVar1)();
  }
  local_20 = 0;
  local_18 = 0;
  if (*(long *)(param_1 + 0x10) == 0) {
    local_20 = FUN_04b37104(0x35,"impl is null",0xc,0x23,"../../arc/browser/public/download_item.cc"
                           );
    if (local_20 != 1) goto LAB_0442fd38;
    FUN_04b37cec(&local_20);
  }
  else {
    FUN_0430ddd0(&local_20);
  }
  if (local_20 == 1) {
    return local_18;
  }
LAB_0442fd38:
  if ((local_20 & 1) == 0) {
    FUN_04b35898();
  }
  return 0xffffffffffffffff;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:39469 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x442fe90  ArcDownloadItem::remove  (88 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcDownloadItem::remove(char *param_1)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  
  if ((param_1[0x20] & 1U) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fee8);
    (*pcVar1)();
  }
  lVar4 = *(long *)(param_1 + 0x10);
  if ((lVar4 != 0) &&
     (lVar3 = (**(code **)(**(long **)(lVar4 + 0x18) + 0xf8))(), param_1 = (char *)0x0, lVar3 != 0))
  {
                    /* WARNING: Could not recover jumptable at 0x0442fed4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar2 = (**(code **)(**(long **)(lVar4 + 0x18) + 0x188))();
    return iVar2;
  }
  return (int)param_1;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:53934 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x443ee54  ArcCreditCardInfoSaveRequest::save  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCreditCardInfoSaveRequest::save(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_0433487c(*(undefined8 *)(param_1 + 8));
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443ee68);
  (*pcVar1)();
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:54062 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x443f038  ArcLoginCredentialsSaveRequest::save  (32 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcLoginCredentialsSaveRequest::save(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443f058);
    (*pcVar1)();
  }
  if (*(long *)(*(long *)(param_1 + 8) + 0x30) != 0) {
    FUN_07d64988();
    return;
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:54107 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x443f080  ArcLoginCredentialsSaveRequest::neverAskAgain  (32 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcLoginCredentialsSaveRequest::neverAskAgain(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443f0a0);
    (*pcVar1)();
  }
  if (*(long *)(*(long *)(param_1 + 8) + 0x30) != 0) {
    FUN_07d64b14();
    return;
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:55887 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x4440dec  ArcWebContents::setNavigationDelegate:  (832 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContents::setNavigationDelegate_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ArcWebContentsNavigationDelegateBridge *pAVar2;
  __shared_weak_count *p_Var3;
  __shared_weak_count *in_x4;
  code *pcVar4;
  ArcWebContentsNavigationDelegateBridge *pAVar5;
  shared_ptr<ArcWebContentsNavigationDelegateBridge> sVar6;
  undefined8 local_50;
  undefined1 auStack_48 [8];
  
  if (param_3 != 0) {
    pAVar2 = (ArcWebContentsNavigationDelegateBridge *)FUN_00008790(0x30);
    pAVar5 = pAVar2 + 8;
    *(long *)pAVar5 = 0;
    *(long *)(pAVar2 + 0x10) = 0;
    *(undefined ***)pAVar2 = &PTR_viewWillStartLiveResize_0da88e10;
    if (pAVar2 + 0x18 == (ArcWebContentsNavigationDelegateBridge *)0x0) goto LAB_04441128;
    _objc_initWeak(&local_50,param_1);
    _objc_initWeak(auStack_48,local_50);
    *(undefined ***)(pAVar2 + 0x18) = &PTR_FUN_0da88fe0;
    _objc_copyWeak(pAVar2 + 0x20,auStack_48);
    _objc_initWeak(pAVar2 + 0x28,param_3);
    _objc_destroyWeak(auStack_48);
    _objc_destroyWeak(&local_50);
    if (param_1 == 0) {
      if (pAVar2 != (ArcWebContentsNavigationDelegateBridge *)0x0) {
        LOAcquire();
        lVar1 = *(long *)pAVar5;
        *(long *)pAVar5 = lVar1 + -1;
        LORelease();
        if (lVar1 == 0) {
          (**(code **)(*(long *)pAVar2 + 0x10))(pAVar2);
          if (*(long *)(pAVar2 + 0x10) != 0) {
            LOAcquire();
            lVar1 = *(long *)(pAVar2 + 0x10);
            *(long *)(pAVar2 + 0x10) = lVar1 + -1;
            LORelease();
            if (lVar1 != 0) goto LAB_04440ecc;
          }
          pcVar4 = *(code **)(*(long *)pAVar2 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(pAVar2,0x20);
          }
          else {
            (*pcVar4)(pAVar2);
          }
        }
      }
    }
    else {
      sVar6.field1_0x8 = in_x4;
      sVar6.field0_0x0 = pAVar2;
      setWeb_contents_navigation_delegate_bridge_
                ((ArcWebContents *)param_1,(ID)"setWeb_contents_navigation_delegate_bridge:",
                 (SEL)(pAVar2 + 0x18),sVar6);
    }
LAB_04440ecc:
    if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
      sVar6 = web_contents_navigation_delegate_bridge
                        (param_1,"web_contents_navigation_delegate_bridge");
      p_Var3 = sVar6.field1_0x8;
      if (p_Var3 == (__shared_weak_count *)0x0) {
        FUN_04345ad8(param_1 + 0x118,sVar6.field0_0x0,0);
        return;
      }
      lVar1 = *(long *)(p_Var3 + 0x10);
      *(long *)(p_Var3 + 0x10) = lVar1 + 1;
      FUN_04345ad8(lVar1,param_1 + 0x118,sVar6.field0_0x0,p_Var3);
      LOAcquire();
      lVar1 = *(long *)(p_Var3 + 8);
      *(long *)(p_Var3 + 8) = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) {
        return;
      }
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40)) {
        (*pcVar4)(p_Var3);
        return;
      }
      FUN_00005810(p_Var3,0x20);
      return;
    }
    goto LAB_04441128;
  }
  sVar6 = web_contents_navigation_delegate_bridge(param_1,"web_contents_navigation_delegate_bridge")
  ;
  p_Var3 = sVar6.field1_0x8;
  if (p_Var3 != (__shared_weak_count *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(p_Var3 + 8);
    *(long *)(p_Var3 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04440f9c;
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(p_Var3,0x20);
      }
      else {
        (*pcVar4)(p_Var3);
      }
    }
  }
LAB_04440f9c:
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_04375b40(*(undefined8 *)(param_1 + 0x118),0,0);
    return;
  }
LAB_04441128:
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x444112c);
  (*pcVar4)();
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:56029 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x444112c  ArcWebContents::setUIDelegate:  (832 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContents::setUIDelegate_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ArcWebContentsUIDelegateBridge *pAVar2;
  __shared_weak_count *p_Var3;
  __shared_weak_count *in_x4;
  code *pcVar4;
  ArcWebContentsUIDelegateBridge *pAVar5;
  shared_ptr<ArcWebContentsUIDelegateBridge> sVar6;
  undefined8 local_50;
  undefined1 auStack_48 [8];
  
  if (param_3 != 0) {
    pAVar2 = (ArcWebContentsUIDelegateBridge *)FUN_00008790(0x30);
    pAVar5 = pAVar2 + 8;
    *(long *)pAVar5 = 0;
    *(long *)(pAVar2 + 0x10) = 0;
    *(undefined ***)pAVar2 = &PTR_viewWillStartLiveResize_0da88e48;
    if (pAVar2 + 0x18 == (ArcWebContentsUIDelegateBridge *)0x0) goto LAB_04441468;
    _objc_initWeak(&local_50,param_1);
    _objc_initWeak(auStack_48,local_50);
    *(undefined ***)(pAVar2 + 0x18) = &PTR_FUN_0da89030;
    _objc_copyWeak(pAVar2 + 0x20,auStack_48);
    _objc_initWeak(pAVar2 + 0x28,param_3);
    _objc_destroyWeak(auStack_48);
    _objc_destroyWeak(&local_50);
    if (param_1 == 0) {
      if (pAVar2 != (ArcWebContentsUIDelegateBridge *)0x0) {
        LOAcquire();
        lVar1 = *(long *)pAVar5;
        *(long *)pAVar5 = lVar1 + -1;
        LORelease();
        if (lVar1 == 0) {
          (**(code **)(*(long *)pAVar2 + 0x10))(pAVar2);
          if (*(long *)(pAVar2 + 0x10) != 0) {
            LOAcquire();
            lVar1 = *(long *)(pAVar2 + 0x10);
            *(long *)(pAVar2 + 0x10) = lVar1 + -1;
            LORelease();
            if (lVar1 != 0) goto LAB_0444120c;
          }
          pcVar4 = *(code **)(*(long *)pAVar2 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(pAVar2,0x20);
          }
          else {
            (*pcVar4)(pAVar2);
          }
        }
      }
    }
    else {
      sVar6.field1_0x8 = in_x4;
      sVar6.field0_0x0 = pAVar2;
      setWeb_contents_ui_delegate_bridge_
                ((ArcWebContents *)param_1,(ID)"setWeb_contents_ui_delegate_bridge:",
                 (SEL)(pAVar2 + 0x18),sVar6);
    }
LAB_0444120c:
    if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
      sVar6 = web_contents_ui_delegate_bridge(param_1,"web_contents_ui_delegate_bridge");
      p_Var3 = sVar6.field1_0x8;
      if (p_Var3 == (__shared_weak_count *)0x0) {
        FUN_04345ba0(param_1 + 0x118,sVar6.field0_0x0,0);
        return;
      }
      lVar1 = *(long *)(p_Var3 + 0x10);
      *(long *)(p_Var3 + 0x10) = lVar1 + 1;
      FUN_04345ba0(lVar1,param_1 + 0x118,sVar6.field0_0x0,p_Var3);
      LOAcquire();
      lVar1 = *(long *)(p_Var3 + 8);
      *(long *)(p_Var3 + 8) = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) {
        return;
      }
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40)) {
        (*pcVar4)(p_Var3);
        return;
      }
      FUN_00005810(p_Var3,0x20);
      return;
    }
    goto LAB_04441468;
  }
  sVar6 = web_contents_ui_delegate_bridge(param_1,"web_contents_ui_delegate_bridge");
  p_Var3 = sVar6.field1_0x8;
  if (p_Var3 != (__shared_weak_count *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(p_Var3 + 8);
    *(long *)(p_Var3 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_044412dc;
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(p_Var3,0x20);
      }
      else {
        (*pcVar4)(p_Var3);
      }
    }
  }
LAB_044412dc:
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_04375cdc(*(undefined8 *)(param_1 + 0x118),0,0);
    return;
  }
LAB_04441468:
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x444146c);
  (*pcVar4)();
}



/* SOURCE /home/exedev/work/inputs/decompile/0x44.c:57643 sha256 f6174f6af1a42d2ea22ac0ba64b09c801dd28eb464b151be9fc83d6c36da4c30 */
// ==== 0x444302c  ArcWebContents::executeJavascript:reason:completion:  (396 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

void ArcWebContents::executeJavascript_reason_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,undefined4 param_6)

{
  code *pcVar1;
  SEL extraout_x1;
  undefined8 local_98;
  undefined8 uStack_90;
  ulong local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  ID *local_68 [2];
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x44431b4);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_80 = 0;
    uStack_78 = 0;
    local_70 = 0;
  }
  else {
    FUN_01b356b8(&local_80,param_3);
    param_2 = extraout_x1;
  }
  local_58 = FUN_04445338;
  local_50 = &PTR_FUN_0df6c498;
  local_68[0] = (ID *)FUN_00008790(8,param_2);
  *local_68[0] = param_5;
  if (param_4 == 0) {
    local_98 = 0;
    uStack_90 = 0;
    local_88 = 0;
  }
  else {
    FUN_01b356b8(&local_98,param_4);
  }
  FUN_04377d70(*(undefined8 *)(param_1 + 0x118),&local_80,local_68,&local_98);
  if ((long)local_88 < 0) {
    FUN_00005810(local_98,local_88 & 0x7fffffffffffffff);
  }
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if ((long)local_70 < 0) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x47.c:38680 sha256 bb1591434d36a02ea930150ea8ff23c3ac98d73e582c1df171700e46c205f9d7 */
// ==== 0x4737eb4  WebContentsPaneHostView::attachWebContentsHostView:  (2656 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x18 bytes */

void WebContentsPaneHostView::attachWebContentsHostView_(ID param_1,SEL param_2,NativeView param_3)

{
  undefined1 auVar1 [16];
  float fVar2;
  code *pcVar3;
  long lVar4;
  ulong uVar5;
  ID IVar6;
  ID *pIVar7;
  ID IVar8;
  ID *pIVar9;
  ID *pIVar10;
  ID *pIVar11;
  long *plVar12;
  long lVar13;
  ID IVar14;
  long lVar15;
  uint uVar16;
  double extraout_d0;
  double extraout_d0_00;
  double extraout_d0_01;
  double extraout_d0_02;
  undefined4 in_s1;
  undefined4 in_register_00005024;
  double in_d2;
  double in_d3;
  float fVar17;
  undefined1 local_f0 [16];
  undefined8 local_e0;
  long local_d8;
  undefined8 local_d0;
  ulong local_c8;
  ID *local_c0;
  long local_b8;
  long local_b0;
  long local_a8;
  ID local_a0;
  ID local_98;
  ID local_90;
  ID local_88;
  long local_80;
  double dVar18;
  
  local_80 = *(long *)PTR____stack_chk_guard_0d824020;
  lVar13 = (long)attached_web_contents_host_view_;
  lVar4 = *(long *)param_3.field0_0x0;
  if (*(long *)(param_1 + lVar13) == lVar4) {
    if (*(long *)PTR____stack_chk_guard_0d824020 == local_80) {
      syncHostedWebContentsState(param_1,"syncHostedWebContentsState");
      return;
    }
    goto LAB_0473890c;
  }
  lVar15 = (long)bridge_;
  if (*(long *)(param_1 + lVar13) != 0 && *(long *)(param_1 + lVar15) != 0) {
    local_e0 = *(undefined8 *)(param_1 + lVar13);
    local_d8 = lVar4;
    FUN_0473cad8(&local_d8,&local_e0);
  }
  detachWebContentsHostView(param_1,"detachWebContentsHostView");
  IVar14 = *(ID *)(param_1 + lVar13);
                    /* WARNING: Load size is inaccurate */
  if (IVar14 != *param_3.field0_0x0) {
    *(ID *)(param_1 + lVar13) = *param_3.field0_0x0;
    IVar14 = *(ID *)(param_1 + lVar13);
  }
  if (IVar14 == 0) {
    *(undefined8 *)(param_1 + lVar13) = 0;
  }
  else {
    uVar5 = FUN_00014cc0(&PTR_s_ArcMacCoalescedResizing_0d41f79e_1_0e304a40);
    if ((uVar5 & 1) != 0) {
      _objc_msgSend(IVar14,"setTranslatesAutoresizingMaskIntoConstraints:",0);
      _objc_msgSend(param_1,"bounds");
      _objc_msgSend(IVar14,"setFrame:");
    }
    local_f0._0_8_ = 0;
    local_f0._8_8_ = 0;
    _objc_msgSend(param_1,"bounds");
    _objc_msgSend(param_1,"bounds");
    _objc_msgSend(param_1,"bounds");
    _objc_msgSend(param_1,"bounds");
    local_d0 = (ID *)CONCAT44((float)(double)CONCAT44(in_register_00005024,in_s1),
                              (float)extraout_d0_00);
    auVar1._8_4_ = SUB84(in_d3,0);
    auVar1._0_8_ = in_d2;
    auVar1._12_4_ = (int)((ulong)in_d3 >> 0x20);
    local_c8 = CONCAT44(-(uint)(9.536743732496689e-07 < in_d3),
                        -(uint)(9.536743732496689e-07 < in_d2)) &
               CONCAT44((float)auVar1._8_8_,(float)in_d2);
    local_f0 = FUN_001d7a7c(&local_d0);
    IVar6 = _objc_msgSend(param_1,"window");
    IVar6 = _objc_msgSend(IVar6,"screen");
    if (IVar6 == 0) {
      IVar6 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
      if (IVar6 != 0) {
        IVar6 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
        NSScreen::backingScaleFactor(IVar6,"backingScaleFactor");
        dVar18 = extraout_d0;
        goto LAB_04738204;
      }
      fVar17 = 1.0;
    }
    else {
      IVar6 = _objc_msgSend(param_1,"window");
      IVar6 = _objc_msgSend(IVar6,"screen");
      _objc_msgSend(IVar6,"backingScaleFactor");
      dVar18 = extraout_d0_01;
LAB_04738204:
      fVar17 = (float)dVar18;
    }
    IVar6 = _objc_msgSend(param_1,"window");
    IVar6 = _objc_msgSend(IVar6,"screen");
    if (IVar6 == 0) {
      IVar6 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
    }
    _objc_msgSend(IVar6,"maximumRefreshInterval");
    fVar2 = 60.0;
    if (0.0 < extraout_d0_02) {
      fVar2 = (float)(1.0 / extraout_d0_02);
    }
    IVar6 = _objc_msgSend(param_1,"window");
    IVar6 = _objc_msgSend(IVar6,"screen");
    if (IVar6 == 0) {
      IVar6 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
    }
    IVar6 = _objc_msgSend(IVar6,"deviceDescription");
    IVar6 = _objc_msgSend(IVar6,"objectForKeyedSubscript:",&cf_NSScreenNumber);
    if (IVar6 == 0) {
      IVar6 = 0;
    }
    else {
      IVar6 = _objc_msgSend(IVar6,"longLongValue");
    }
    pIVar7 = (ID *)FUN_00008790(8);
    *pIVar7 = param_1;
    lVar4 = *(long *)(param_1 + lVar15);
    if (lVar4 != 0) {
      lVar13 = *(long *)(param_1 + lVar13);
      IVar8 = _objc_msgSend(param_1,"window");
      if (IVar8 == 0) {
        uVar16 = 0;
      }
      else {
        IVar8 = _objc_msgSend(param_1,"isHiddenOrHasHiddenAncestor");
        uVar16 = (uint)IVar8 ^ 1;
      }
      pIVar9 = (ID *)FUN_00008790(8);
      *pIVar9 = 0;
      *pIVar9 = *pIVar7;
      if (lVar13 == 0) {
                    /* WARNING: Does not return */
        pcVar3 = (code *)SoftwareBreakpoint(0,0x4738a28);
        (*pcVar3)();
      }
      plVar12 = (long *)(lVar4 + 0x78);
      if (*plVar12 == lVar13) {
        pIVar10 = (ID *)FUN_00008790(8);
        *pIVar10 = 0;
        *pIVar10 = *pIVar9;
        if (*(long *)(lVar4 + 0x68) != 0) {
          pIVar11 = (ID *)FUN_00008790(8);
          *pIVar11 = 0;
          *pIVar11 = *pIVar10;
          local_d0 = pIVar11;
          FUN_0473a048(lVar4,&local_d0);
          if (pIVar11 != (ID *)0x0) {
            FUN_00005810(pIVar11,8);
          }
          FUN_0473a110(fVar17,fVar2,lVar4,local_f0,IVar6);
          FUN_0473a3c0(lVar4,uVar16);
        }
      }
      else {
        if (*plVar12 != 0) {
          local_b0 = *plVar12;
          local_a8 = lVar13;
          FUN_0473cad8(&local_a8,&local_b0);
        }
        FUN_04738ad4(lVar4);
        local_b8 = *plVar12;
        if (local_b8 != lVar13) {
          *plVar12 = lVar13;
          local_b8 = *plVar12;
        }
        local_c0 = (ID *)FUN_00008790(8);
        *local_c0 = 0;
        *local_c0 = *pIVar9;
        FUN_0473cf0c(&local_b8,lVar4,local_f0,uVar16,&local_c0);
        pIVar10 = local_c0;
        local_c0 = (ID *)0x0;
        if (pIVar10 != (ID *)0x0) {
          FUN_00005810(pIVar10,8);
        }
        pIVar10 = (ID *)FUN_00008790(8);
        *pIVar10 = 0;
        *pIVar10 = *pIVar9;
        if (*(long *)(lVar4 + 0x68) != 0) {
          pIVar11 = (ID *)FUN_00008790(8);
          *pIVar11 = 0;
          *pIVar11 = *pIVar10;
          local_d0 = pIVar11;
          FUN_0473a048(lVar4,&local_d0);
          if (pIVar11 != (ID *)0x0) {
            FUN_00005810(pIVar11,8);
          }
          FUN_0473a110(fVar17,fVar2,lVar4,local_f0,IVar6);
          FUN_0473a3c0(lVar4,uVar16);
        }
      }
      local_d0 = (ID *)0x0;
      if (pIVar10 != (ID *)0x0) {
        FUN_00005810(pIVar10,8);
      }
      if (pIVar9 != (ID *)0x0) {
        FUN_00005810(pIVar9,8);
      }
    }
    _objc_msgSend(param_1,"addSubview:positioned:relativeTo:",IVar14,0xffffffffffffffff,
                  *(undefined8 *)(param_1 + (long)compositor_overlay_view_));
    _objc_msgSend(IVar14,"setTranslatesAutoresizingMaskIntoConstraints:",0);
    IVar6 = _objc_msgSend(IVar14,"leadingAnchor");
    IVar8 = _objc_msgSend(param_1,"leadingAnchor");
    local_a0 = _objc_msgSend(IVar6,"constraintEqualToAnchor:",IVar8);
    IVar6 = _objc_msgSend(IVar14,"trailingAnchor");
    IVar8 = _objc_msgSend(param_1,"trailingAnchor");
    local_98 = _objc_msgSend(IVar6,"constraintEqualToAnchor:",IVar8);
    IVar6 = _objc_msgSend(IVar14,"topAnchor");
    IVar8 = _objc_msgSend(param_1,"topAnchor");
    local_90 = _objc_msgSend(IVar6,"constraintEqualToAnchor:",IVar8);
    IVar14 = _objc_msgSend(IVar14,"bottomAnchor");
    IVar6 = _objc_msgSend(param_1,"bottomAnchor");
    local_88 = _objc_msgSend(IVar14,"constraintEqualToAnchor:",IVar6);
    IVar14 = NSArray::arrayWithObjects_count_
                       ((ID)&_OBJC_CLASS___NSArray,"arrayWithObjects:count:",&local_a0,4);
    _objc_msgSend(param_1,"addConstraints:",IVar14);
    IVar14 = _objc_msgSend(param_1,"window");
    IVar14 = _objc_msgSend(IVar14,"firstResponder");
    if (((IVar14 == param_1) && (*(long *)(param_1 + lVar15) != 0)) &&
       (lVar4 = *(long *)(*(long *)(param_1 + lVar15) + 0x78), lVar4 != 0)) {
      local_d0 = (ID *)lVar4;
      FUN_0473d7e4(&local_d0);
    }
    syncHostedWebContentsState(param_1,"syncHostedWebContentsState");
    if (pIVar7 != (ID *)0x0) {
      FUN_00005810(pIVar7,8);
    }
  }
  lVar4 = 0;
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_80) {
    return;
  }
LAB_0473890c:
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(lVar4);
}



/* SOURCE /home/exedev/work/inputs/decompile/0x47.c:38948 sha256 bb1591434d36a02ea930150ea8ff23c3ac98d73e582c1df171700e46c205f9d7 */
// ==== 0x4738a30  WebContentsPaneHostView::detachWebContentsHostView  (164 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

void WebContentsPaneHostView::detachWebContentsHostView(ID param_1,SEL param_2)

{
  ID IVar1;
  ID self;
  long lVar2;
  
  lVar2 = (long)attached_web_contents_host_view_;
  self = *(ID *)(param_1 + lVar2);
  if (self != 0) {
    if (*(long *)(param_1 + (long)bridge_) != 0) {
      FUN_04738ad4();
    }
    IVar1 = _objc_msgSend(self,"superview");
    if (IVar1 == param_1) {
      _objc_msgSend(self,"removeFromSuperview");
    }
    *(undefined8 *)(param_1 + lVar2) = 0;
    return;
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x47.c:39006 sha256 bb1591434d36a02ea930150ea8ff23c3ac98d73e582c1df171700e46c205f9d7 */
// ==== 0x4738bb0  WebContentsPaneHostView::syncHostedWebContentsState  (1088 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void WebContentsPaneHostView::syncHostedWebContentsState(ID param_1,SEL param_2)

{
  undefined1 auVar1 [16];
  ID IVar2;
  ID IVar3;
  ID *pIVar4;
  ID *pIVar5;
  uint uVar6;
  long lVar7;
  double extraout_d0;
  double extraout_d0_00;
  double extraout_d0_01;
  double extraout_d0_02;
  undefined4 in_s1;
  undefined4 in_register_00005024;
  double in_d2;
  double in_d3;
  float fVar8;
  float fVar10;
  undefined1 local_80 [16];
  undefined8 local_70;
  ulong local_68;
  double dVar9;
  
  lVar7 = (long)bridge_;
  if (*(long *)(param_1 + lVar7) == 0) {
    return;
  }
  if (*(long *)(param_1 + (long)attached_web_contents_host_view_) == 0) {
    return;
  }
  IVar2 = _objc_msgSend(param_1,"window");
  if (IVar2 == 0) {
    uVar6 = 0;
  }
  else {
    IVar2 = _objc_msgSend(param_1,"isHiddenOrHasHiddenAncestor");
    uVar6 = (uint)IVar2 ^ 1;
  }
  _objc_msgSend(param_1,"bounds");
  _objc_msgSend(param_1,"bounds");
  _objc_msgSend(param_1,"bounds");
  _objc_msgSend(param_1,"bounds");
  local_70 = (ID *)CONCAT44((float)(double)CONCAT44(in_register_00005024,in_s1),(float)extraout_d0);
  auVar1._8_4_ = SUB84(in_d3,0);
  auVar1._0_8_ = in_d2;
  auVar1._12_4_ = (int)((ulong)in_d3 >> 0x20);
  local_68 = CONCAT44(-(uint)(9.536743732496689e-07 < in_d3),-(uint)(9.536743732496689e-07 < in_d2))
             & CONCAT44((float)auVar1._8_8_,(float)in_d2);
  local_80 = FUN_001d7a7c(&local_70);
  lVar7 = *(long *)(param_1 + lVar7);
  IVar2 = _objc_msgSend(param_1,"window");
  IVar2 = _objc_msgSend(IVar2,"screen");
  if (IVar2 == 0) {
    IVar2 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
    if (IVar2 == 0) {
      fVar8 = 1.0;
      goto LAB_04738d58;
    }
    IVar2 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
    NSScreen::backingScaleFactor(IVar2,"backingScaleFactor");
    dVar9 = extraout_d0_02;
  }
  else {
    IVar2 = _objc_msgSend(param_1,"window");
    IVar2 = _objc_msgSend(IVar2,"screen");
    _objc_msgSend(IVar2,"backingScaleFactor");
    dVar9 = extraout_d0_00;
  }
  fVar8 = (float)dVar9;
LAB_04738d58:
  IVar2 = _objc_msgSend(param_1,"window");
  IVar2 = _objc_msgSend(IVar2,"screen");
  if (IVar2 == 0) {
    IVar2 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
  }
  IVar2 = _objc_msgSend(IVar2,"deviceDescription");
  IVar2 = _objc_msgSend(IVar2,"objectForKeyedSubscript:",&cf_NSScreenNumber);
  if (IVar2 == 0) {
    IVar2 = 0;
  }
  else {
    IVar2 = _objc_msgSend(IVar2,"longLongValue");
  }
  IVar3 = _objc_msgSend(param_1,"window");
  IVar3 = _objc_msgSend(IVar3,"screen");
  if (IVar3 == 0) {
    IVar3 = NSScreen::mainScreen((ID)&_OBJC_CLASS___NSScreen,"mainScreen");
  }
  _objc_msgSend(IVar3,"maximumRefreshInterval");
  if (0.0 < extraout_d0_01) {
    fVar10 = (float)(1.0 / extraout_d0_01);
  }
  else {
    fVar10 = 60.0;
  }
  pIVar4 = (ID *)FUN_00008790(8);
  *pIVar4 = param_1;
  if (*(long *)(lVar7 + 0x68) != 0) {
    pIVar5 = (ID *)FUN_00008790(8);
    *pIVar5 = 0;
    *pIVar5 = *pIVar4;
    local_70 = pIVar5;
    FUN_0473a048(lVar7,&local_70);
    if (pIVar5 != (ID *)0x0) {
      FUN_00005810(pIVar5,8);
    }
    FUN_0473a110(fVar8,fVar10,lVar7,local_80,IVar2);
    FUN_0473a3c0(lVar7,uVar6);
  }
  if (pIVar4 != (ID *)0x0) {
    FUN_00005810(pIVar4,8);
  }
  return;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x47.c:39184 sha256 bb1591434d36a02ea930150ea8ff23c3ac98d73e582c1df171700e46c205f9d7 */
// ==== 0x47390dc  FUN_047390dc  (56 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */

ID FUN_047390dc(ID param_1,SEL param_2)

{
  ID IVar1;
  
  IVar1 = _objc_alloc(&objc::class_t::WebContentsPaneHostView);
  IVar1 = WebContentsPaneHostView::initWithFrame_
                    (IVar1,"initWithFrame:",*(CGRect *)PTR__NSZeroRect_0d8254f0);
  return IVar1;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x47.c:39200 sha256 bb1591434d36a02ea930150ea8ff23c3ac98d73e582c1df171700e46c205f9d7 */
// ==== 0x4739114  FUN_04739114  (160 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04739114(ID param_1,undefined8 *param_2)

{
  ID IVar1;
  undefined8 local_28;
  
  IVar1 = _objc_msgSend((ID)&objc::class_t::WebContentsPaneHostView,"class");
  IVar1 = _objc_msgSend(param_1,"isMemberOfClass:",IVar1);
  if ((int)IVar1 != 0) {
    local_28 = *param_2;
    _objc_msgSend(param_1,"attachWebContentsHostView:",&local_28);
  }
  return 0;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x47.c:39221 sha256 bb1591434d36a02ea930150ea8ff23c3ac98d73e582c1df171700e46c205f9d7 */
// ==== 0x47391c8  FUN_047391c8  (96 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_047391c8(ID param_1)

{
  ID IVar1;
  
  IVar1 = _objc_msgSend((ID)&objc::class_t::WebContentsPaneHostView,"class");
  IVar1 = _objc_msgSend(param_1,"isMemberOfClass:",IVar1);
  if ((int)IVar1 != 0) {
    _objc_msgSend(param_1,"detachWebContentsHostView");
  }
  return 0;
}



/* SOURCE /home/exedev/work/inputs/decompile/0x83.c:86368 sha256 37105cd32dd0b385f4b2ff4154e5e46eab8a33ef6e18c156830f917c91ccc2a1 */
// ==== 0x8374b4c  ArcPerformanceController::setHighEfficiencyModeEnabled:  (116 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcPerformanceController::setHighEfficiencyModeEnabled_(ID param_1,SEL param_2,bool param_3)

{
  int iVar1;
  
  if (((DAT_0e4a2f20 & 1) == 0) && (iVar1 = FUN_0c44b610(&DAT_0e4a2f20), iVar1 != 0)) {
    thunk_FUN_00008790(0x40,param_3);
    DAT_0e4a2f18 = FUN_0433d1bc();
    FUN_0c44b760(&DAT_0e4a2f20);
  }
  FUN_07b02940(DAT_0e49d488,param_3);
  return;
}


