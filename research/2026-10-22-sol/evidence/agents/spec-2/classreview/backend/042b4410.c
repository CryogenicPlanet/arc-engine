// ==== 0x42b4410  FUN_042b4410  (2228 bytes)

/* WARNING: Removing unreachable block (ram,0x042b4a44) */
/* WARNING: Removing unreachable block (ram,0x042b4a14) */
/* WARNING: Removing unreachable block (ram,0x042b49e4) */
/* WARNING: Removing unreachable block (ram,0x042b4c48) */
/* WARNING: Removing unreachable block (ram,0x042b4c60) */
/* WARNING: Removing unreachable block (ram,0x042b49fc) */
/* WARNING: Removing unreachable block (ram,0x042b4a2c) */
/* WARNING: Removing unreachable block (ram,0x042b4a5c) */

void FUN_042b4410(undefined8 *param_1,long param_2,int param_3,long *param_4)

{
  long lVar1;
  long lVar2;
  bool bVar3;
  size_t sVar4;
  char *pcVar5;
  code *pcVar6;
  long *****ppppplVar7;
  long lVar8;
  long ******pppppplVar9;
  undefined8 uVar10;
  ulong uVar11;
  long lVar12;
  long lVar13;
  long ****pppplVar14;
  undefined1 auVar15 [16];
  long *****local_320;
  long lStack_318;
  ulong local_310;
  undefined8 local_300;
  undefined8 uStack_2f8;
  undefined8 uStack_2f0;
  undefined8 uStack_2e8;
  undefined4 local_2e0;
  char *local_2d0;
  undefined8 local_2c8;
  ulong uStack_2c0;
  undefined7 local_2b8;
  undefined1 uStack_2b1;
  undefined7 uStack_2b0;
  char local_2a9;
  undefined7 local_2a8;
  char cStack_2a1;
  undefined1 auStack_2a0 [128];
  long ****local_220;
  undefined8 uStack_218;
  undefined8 uStack_210;
  undefined8 uStack_208;
  undefined4 local_200;
  char *local_1f0;
  undefined8 local_1e8;
  ulong uStack_1e0;
  undefined7 local_1d8;
  undefined1 uStack_1d1;
  undefined7 uStack_1d0;
  char local_1c9;
  undefined7 local_1c8;
  char cStack_1c1;
  undefined1 auStack_1c0 [128];
  long *****local_140;
  long lStack_138;
  ulong local_130;
  long *****local_120;
  long lStack_118;
  undefined8 local_110;
  undefined8 *local_108;
  undefined7 local_100;
  undefined4 uStack_f9;
  char cStack_f5;
  undefined4 uStack_f4;
  undefined8 local_f0;
  long *****local_e8;
  long lStack_e0;
  ulong local_d8;
  char local_d0 [23];
  undefined1 uStack_b9;
  char local_b8 [23];
  undefined1 uStack_a1;
  char local_a0 [23];
  undefined1 uStack_89;
  char local_88 [23];
  undefined1 uStack_71;
  long local_68;
  
  local_68 = *(long *)PTR____stack_chk_guard_0d824020;
  lStack_118 = 0;
  local_120 = (long *****)0x0;
  local_110 = 0;
  if (*(char *)((long)param_4 + 0x17) < '\0') {
    if (param_4[1] != 0) {
      param_4 = (long *)*param_4;
      goto LAB_042b4468;
    }
LAB_042b4514:
    FUN_01a29860(DAT_0e481618);
    local_100 = 0;
    uStack_f9 = 0;
    cStack_f5 = '\0';
    uStack_f4 = 0;
    local_f0 = 0;
    FUN_01b92818(&local_140,&local_100,"Default",7);
    if ((long)local_110 < 0) {
      FUN_00005810(local_120,local_110 & 0x7fffffffffffffff);
    }
    lStack_118 = lStack_138;
    local_120 = local_140;
    local_110 = local_130;
  }
  else {
    if (*(char *)((long)param_4 + 0x17) == '\0') goto LAB_042b4514;
LAB_042b4468:
    FUN_01c39a30(&local_100,param_4);
    if ((long)local_110 < 0) {
      FUN_00005810(local_120,local_110 & 0x7fffffffffffffff);
    }
    lStack_118 = CONCAT44(uStack_f4,CONCAT13(cStack_f5,uStack_f9._1_3_));
    local_120 = (long *****)CONCAT17((undefined1)uStack_f9,local_100);
    local_110 = local_f0;
  }
  lVar8 = FUN_042c1ad4(param_2 + 0xd8,&local_120);
  if (((lVar8 == 0) || (lVar12 = *(long *)(lVar8 + 0x30), lVar12 == 0)) ||
     (*(long *)(lVar12 + 8) == -1)) {
    lVar8 = FUN_01a29860(DAT_0e481618);
    local_140 = (long *****)0x0;
    lStack_138 = 0;
    local_130 = 0;
    pppppplVar9 = (long ******)local_120;
    if (-1 < (long)local_110._7_1_) {
      pppppplVar9 = &local_120;
    }
    lVar12 = lStack_118;
    if (-1 < (long)local_110) {
      lVar12 = (long)local_110._7_1_;
    }
    FUN_01b92818(&local_140,lVar8 + 0x50,pppppplVar9,lVar12);
    lVar12 = FUN_027f2c40(lVar8,&local_140);
    uVar10 = FUN_01a298f8(lVar8);
    lVar8 = FUN_01a299e0(uVar10,&local_140);
    if ((lVar12 != 0) && (lVar8 != 0)) {
      auVar15 = FUN_042b4cc4(param_2,lVar12);
      if ((long)local_130 < 0) {
        FUN_00005810(local_140,local_130 & 0x7fffffffffffffff);
      }
      goto LAB_042b45e0;
    }
    uVar10 = FUN_02bed438();
    cStack_2a1 = '\x0f';
    if ("browser_context" < &local_2a9 && &local_2b8 < "rowser_context") goto LAB_042b4cc0;
    local_2b8 = 0x726573776f7262;
    uStack_2b1 = 0x5f;
    uStack_2b0 = 0x747865746e6f63;
    local_2a9 = '\0';
    local_2d0 = (char *)FUN_00008790(0x40);
    uStack_2c0 = 0x8000000000000040;
    local_2c8 = 0x34;
    if ((local_2d0 + 0x34 < local_2d0) ||
       ("Attempt to use a browser context that does not exist" < local_2d0 + 0x34 &&
        local_2d0 < "ttempt to use a browser context that does not exist")) goto LAB_042b4cc0;
    builtin_strncpy(local_2d0,"Attempt to use a browser context that does not exist",0x35);
    if ((long)local_110 < 0) {
      FUN_00060a60(&local_320,local_120,lStack_118);
    }
    else {
      lStack_318 = lStack_118;
      local_320 = local_120;
      local_310 = local_110;
    }
    local_f0 = CONCAT17(0xb,(undefined7)local_f0);
    if ("profile_dir" < &cStack_f5 && &local_100 < "rofile_dir") goto LAB_042b4cc0;
    local_100 = 0x656c69666f7270;
    uStack_f9 = 0x7269645f;
    cStack_f5 = '\0';
    lStack_e0 = lStack_318;
    local_e8 = local_320;
    local_d8 = local_310;
    local_320 = (long *****)0x0;
    lStack_318 = 0;
    pcVar5 = "true";
    if (lVar12 == 0) {
      pcVar5 = "false";
    }
    local_310 = 0;
    uStack_b9 = 0x13;
    if ("profile_is_not_null" < local_d0 + 0x13 && local_d0 < "rofile_is_not_null")
    goto LAB_042b4cc0;
    builtin_strncpy(local_d0,"profile_is_not_null",0x14);
    sVar4 = 4;
    if (lVar12 == 0) {
      sVar4 = 5;
    }
    uStack_a1 = (undefined1)sVar4;
    lVar13 = 0x4c;
    if (lVar12 == 0) {
      lVar13 = 0x4d;
    }
    if (pcVar5 < (char *)((long)&local_100 + lVar13) && local_b8 <= pcVar5) goto LAB_042b4cc0;
    _memcpy(local_b8,pcVar5,sVar4);
    *(char *)((long)&local_100 + lVar13) = '\0';
    pcVar5 = "true";
    if (lVar8 == 0) {
      pcVar5 = "false";
    }
    uStack_89 = 0xe;
    if ("has_attributes" < local_a0 + 0xe && local_a0 < "as_attributes") goto LAB_042b4cc0;
    builtin_strncpy(local_a0,"has_attributes",0xf);
    sVar4 = 4;
    if (lVar8 == 0) {
      sVar4 = 5;
    }
    uStack_71 = (undefined1)sVar4;
    lVar12 = 0x7c;
    if (lVar8 == 0) {
      lVar12 = 0x7d;
    }
    if (pcVar5 < (char *)((long)&local_100 + lVar12) && local_88 <= pcVar5) goto LAB_042b4cc0;
    _memcpy(local_88,pcVar5,sVar4);
    *(char *)((long)&local_100 + lVar12) = '\0';
    uStack_2f8 = 0;
    local_300 = 0;
    uStack_2e8 = 0;
    uStack_2f0 = 0;
    local_2e0 = 0x3f800000;
    local_108 = &local_300;
    FUN_00ba2240(&local_108,&local_100,&local_100);
    local_108 = &local_300;
    FUN_00ba2240(&local_108,local_d0,local_d0);
    local_108 = &local_300;
    FUN_00ba2240(&local_108,local_a0,local_a0);
    FUN_02bed328(auStack_2a0,&local_2b8,&local_2d0,&local_300);
    FUN_02bed570(uVar10,auStack_2a0);
    FUN_08b90450(auStack_2a0);
    FUN_04b8e340(&local_300);
    if ((long)local_310 < 0) {
      FUN_00005810(local_320,local_310 & 0x7fffffffffffffff);
    }
    if ((long)uStack_2c0 < 0) {
      FUN_00005810(local_2d0,uStack_2c0 & 0x7fffffffffffffff);
    }
    if (cStack_2a1 < '\0') {
      FUN_00005810(CONCAT17(uStack_2b1,local_2b8),
                   CONCAT17(cStack_2a1,local_2a8) & 0x7fffffffffffffff);
    }
    *(undefined1 *)param_1 = 0;
    *(undefined1 *)(param_1 + 2) = 0;
    if (-1 < (long)local_130) goto LAB_042b4614;
    uVar11 = local_130 & 0x7fffffffffffffff;
    pppppplVar9 = (long ******)local_140;
  }
  else {
    if (*(long *)(lVar12 + 8) == -1) {
      uVar10 = FUN_02bed438();
      cStack_1c1 = '\x0f';
      if ("browser_context" < &local_1c9 && &local_1d8 < "rowser_context") {
LAB_042b4cc0:
                    /* WARNING: Does not return */
        pcVar6 = (code *)SoftwareBreakpoint(1,0x42b4cc4);
        (*pcVar6)();
      }
      local_1d8 = 0x726573776f7262;
      uStack_1d1 = 0x5f;
      uStack_1d0 = 0x747865746e6f63;
      local_1c9 = '\0';
      local_1f0 = (char *)FUN_00008790(0x40);
      uStack_1e0 = 0x8000000000000040;
      local_1e8 = 0x31;
      if ((local_1f0 + 0x31 < local_1f0) ||
         ("Attempt to use an expired browser context wrapper" < local_1f0 + 0x31 &&
          local_1f0 < "ttempt to use an expired browser context wrapper")) goto LAB_042b4cc0;
      builtin_strncpy(local_1f0,"Attempt to use an expired browser context wrapper",0x32);
      if ((long)local_110 < 0) {
        FUN_00060a60(&local_140,local_120,lStack_118);
      }
      else {
        lStack_138 = lStack_118;
        local_140 = local_120;
        local_130 = local_110;
      }
      local_f0 = CONCAT17(0xb,(undefined7)local_f0);
      if ("profile_dir" < &cStack_f5 && &local_100 < "rofile_dir") goto LAB_042b4cc0;
      local_100 = 0x656c69666f7270;
      uStack_f9 = 0x7269645f;
      cStack_f5 = '\0';
      lStack_e0 = lStack_138;
      local_e8 = local_140;
      local_d8 = local_130;
      local_140 = (long *****)0x0;
      lStack_138 = 0;
      local_130 = 0;
      uStack_218 = 0;
      local_220 = (long ****)0x0;
      uStack_208 = 0;
      uStack_210 = 0;
      local_200 = 0x3f800000;
      local_320 = &local_220;
      FUN_00ba2240(&local_320,&local_100,&local_100);
      FUN_02bed328(auStack_1c0,&local_1d8,&local_1f0,&local_220);
      FUN_02bed570(uVar10,auStack_1c0);
      FUN_08b90450(auStack_1c0);
      FUN_04b8e340(&local_220);
      if ((long)local_130 < 0) {
        FUN_00005810(local_140,local_130 & 0x7fffffffffffffff);
      }
      if ((long)uStack_1e0 < 0) {
        FUN_00005810(local_1f0,uStack_1e0 & 0x7fffffffffffffff);
      }
      if (cStack_1c1 < '\0') {
        FUN_00005810(CONCAT17(uStack_1d1,local_1d8),
                     CONCAT17(cStack_1c1,local_1c8) & 0x7fffffffffffffff);
      }
    }
    lVar12 = *(long *)(lVar8 + 0x30);
    if (lVar12 == 0) {
      auVar15 = ZEXT816(0) << 0x40;
    }
    else {
      lVar13 = *(long *)(lVar12 + 8);
      do {
        if (lVar13 == -1) {
          auVar15 = ZEXT816(0);
          goto LAB_042b45e0;
        }
        lVar1 = lVar13 + 1;
        lVar2 = *(long *)(lVar12 + 8);
        bVar3 = lVar2 != lVar13;
        lVar13 = lVar2;
      } while (bVar3);
      *(long *)(lVar12 + 8) = lVar1;
      auVar15._8_8_ = lVar12;
      auVar15._0_8_ = *(undefined8 *)(lVar8 + 0x28);
    }
LAB_042b45e0:
    pppppplVar9 = auVar15._8_8_;
    if (param_3 == 0) {
      FUN_042c8400(&local_100,auVar15._0_8_);
    }
    else {
      FUN_042d730c();
    }
    param_1[1] = CONCAT44(uStack_f4,CONCAT13(cStack_f5,uStack_f9._1_3_));
    *param_1 = CONCAT17((undefined1)uStack_f9,local_100);
    *(undefined1 *)(param_1 + 2) = 1;
    if (pppppplVar9 == (long ******)0x0) goto LAB_042b4614;
    LOAcquire();
    ppppplVar7 = pppppplVar9[1];
    pppppplVar9[1] = (long *****)((long)ppppplVar7 + -1);
    LORelease();
    if (ppppplVar7 != (long *****)0x0) goto LAB_042b4614;
    (*(code *)(*pppppplVar9)[2])(pppppplVar9);
    if (pppppplVar9[2] != (long *****)0x0) {
      LOAcquire();
      ppppplVar7 = pppppplVar9[2];
      pppppplVar9[2] = (long *****)((long)ppppplVar7 + -1);
      LORelease();
      if (ppppplVar7 != (long *****)0x0) goto LAB_042b4614;
    }
    pppplVar14 = (*pppppplVar9)[4];
    if (((pppplVar14 != (long ****)FUN_00f01bc0) && (pppplVar14 != (long ****)FUN_00f01bc0)) &&
       (pppplVar14 != (long ****)FUN_013efb40)) {
      (*(code *)pppplVar14)(pppppplVar9);
      goto LAB_042b4614;
    }
    uVar11 = 0x20;
  }
  FUN_00005810(pppppplVar9,uVar11);
LAB_042b4614:
  if ((long)local_110 < 0) {
    FUN_00005810(local_120,local_110 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_68) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail();
  }
  return;
}


