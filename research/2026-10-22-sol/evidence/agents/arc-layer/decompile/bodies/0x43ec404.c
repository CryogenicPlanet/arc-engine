// ==== 0x43ec404  FUN_043ec404  (1360 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043ec404(undefined8 *param_1,ID param_2)

{
  int iVar1;
  ulong uVar2;
  bool bVar3;
  ulong uVar4;
  code *pcVar5;
  ID IVar6;
  undefined8 *puVar7;
  undefined8 *puVar8;
  undefined4 uVar9;
  long lVar10;
  ulong uVar11;
  long lVar12;
  undefined8 *puVar13;
  size_t sVar14;
  long lVar15;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined *local_90;
  undefined8 uStack_88;
  undefined1 *local_80;
  undefined *puStack_78;
  undefined8 **local_70;
  undefined8 *local_68;
  ulong local_60;
  ulong local_58;
  
  param_1[9] = 0;
  param_1[8] = 0;
  param_1[0xc] = 0;
  param_1[0xb] = 0;
  param_1[10] = 0;
  param_1[5] = 0;
  param_1[4] = 0;
  param_1[7] = 0;
  param_1[6] = 0;
  param_1[1] = 0;
  *param_1 = 0;
  param_1[3] = 0;
  param_1[2] = 0;
  *(undefined1 *)(param_1 + 0xc) = 1;
  IVar6 = _objc_msgSend(param_2,"argc");
  *(int *)param_1 = (int)IVar6;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_58 = 0;
  IVar6 = _objc_msgSend(param_2,"argc");
  uVar2 = local_58;
  uVar4 = local_60;
  iVar1 = (int)IVar6 + 1;
  if (local_58 < (ulong)(long)iVar1) {
    uVar11 = (ulong)iVar1;
    if (iVar1 < 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    puVar7 = (undefined8 *)FUN_00008790(uVar11 * 0x18);
    puVar8 = local_68;
    sVar14 = uVar4 * 0x18;
    if (sVar14 != 0) {
      _memcpy(puVar7,local_68,sVar14);
    }
    local_60 = ((long)sVar14 >> 3) * -0x5555555555555555;
    local_68 = puVar7;
    local_58 = uVar11;
    if (puVar8 != (undefined8 *)0x0) {
      FUN_00005810(puVar8,uVar2 * 0x18);
    }
  }
  IVar6 = _objc_msgSend(param_2,"argv");
  local_90 = PTR___NSConcreteGlobalBlock_0d824130;
  uStack_88 = 0xd0800000;
  local_80 = &LAB_043ea6ac;
  puStack_78 = &DAT_0df6b218;
  local_70 = &local_68;
  _objc_msgSend(IVar6,"enumerateObjectsUsingBlock:",&local_90);
  uVar4 = local_60;
  if (local_60 == local_58) {
    uVar2 = local_60 + 1;
    if (0xaaaaaaaaaaaaaaa < uVar2) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380(0);
    }
    uVar11 = local_60 * 2;
    if (uVar11 < uVar2 || uVar11 - uVar2 == 0) {
      uVar11 = uVar2;
    }
    if (0x555555555555554 < local_60) {
      uVar11 = 0xaaaaaaaaaaaaaaa;
    }
    if (uVar11 == 0) {
      puVar8 = (undefined8 *)0x0;
    }
    else {
      if (0xaaaaaaaaaaaaaaa < uVar11) {
                    /* WARNING: Subroutine does not return */
        FUN_04b087f0();
      }
      puVar8 = (undefined8 *)FUN_00008790(uVar11 * 0x18);
    }
    uVar2 = local_60;
    puVar7 = puVar8 + uVar4 * 3;
    if (puVar7 == (undefined8 *)0x0) goto LAB_043ec9f4;
    *puVar7 = 0;
    puVar7[2] = 0;
    puVar7 = puVar7 + local_60 * -3;
    if (local_60 * 0x18 != 0) {
      _memcpy(puVar7,local_68,local_60 * 0x18);
    }
    puVar13 = local_68;
    local_60 = ((long)(uVar2 * 0x18 + 0x18) >> 3) * -0x5555555555555555;
    bVar3 = local_68 != (undefined8 *)0x0;
    local_68 = puVar7;
    if (bVar3) {
      lVar12 = local_58 * 0x18;
      local_58 = uVar11;
      FUN_00005810(puVar13,lVar12);
      uVar11 = local_58;
    }
  }
  else {
    puVar8 = local_68 + local_60 * 3;
    if (puVar8 == (undefined8 *)0x0) goto LAB_043ec9f4;
    *puVar8 = 0;
    puVar8[2] = 0;
    puVar8 = local_68;
    uVar11 = local_58;
  }
  local_58 = uVar11;
  lVar12 = (long)puVar8 + ((uVar4 * 0x18 + 0x18) - (long)local_68);
  local_60 = (lVar12 >> 3) * -0x5555555555555555;
  if (lVar12 == 0) {
LAB_043ec9f4:
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x43ec9f8);
    (*pcVar5)();
  }
  lVar12 = param_1[1];
  if (lVar12 != 0) {
    lVar10 = param_1[2] * 0x18;
    if (lVar10 != 0) {
      lVar15 = param_1[2] * -0x18;
      puVar8 = (undefined8 *)(lVar10 + lVar12);
      do {
        puVar7 = puVar8 + -3;
        if (puVar7 == (undefined8 *)0x0) goto LAB_043ec9f4;
        if (*(char *)((long)puVar8 + -1) < '\0') {
          FUN_00005810(*puVar7,puVar8[-1] & 0x7fffffffffffffff);
        }
        lVar15 = lVar15 + 0x18;
        puVar8 = puVar7;
      } while (lVar15 != 0);
    }
    FUN_00005810(lVar12,param_1[3] * 0x18);
  }
  param_1[1] = local_68;
  param_1[2] = local_60;
  param_1[3] = local_58;
  local_60 = 0;
  local_58 = 0;
  local_68 = (undefined8 *)0x0;
  IVar6 = _objc_msgSend(param_2,"profilePath");
  if (IVar6 == 0) {
    local_a8 = 0;
    uStack_a0 = 0;
    local_98 = 0;
  }
  else {
    FUN_01b356b8(&local_a8,IVar6);
  }
  if (*(char *)((long)param_1 + 0x37) < '\0') {
    FUN_00005810(param_1[4],param_1[6] & 0x7fffffffffffffff);
  }
  param_1[5] = uStack_a0;
  param_1[4] = local_a8;
  param_1[6] = local_98;
  IVar6 = _objc_msgSend(param_2,"applicationID");
  if (IVar6 == 0) {
    uVar9 = 0;
  }
  else {
    if (IVar6 != 1) goto LAB_043ec9e4;
    uVar9 = 1;
  }
  *(undefined4 *)(param_1 + 7) = uVar9;
  IVar6 = _objc_msgSend(param_2,"isProduction");
  *(char *)(param_1 + 0xb) = (char)IVar6;
  IVar6 = _objc_msgSend(param_2,"sessionRestorationPolicy");
  if (IVar6 == 0) {
    uVar9 = 0;
  }
  else {
    if (IVar6 != 1) {
LAB_043ec9e4:
                    /* WARNING: Does not return */
      pcVar5 = (code *)SoftwareBreakpoint(0x550b,0x43ec9e8);
      (*pcVar5)();
    }
    uVar9 = 1;
  }
  *(undefined4 *)((long)param_1 + 0x5c) = uVar9;
  IVar6 = _objc_msgSend(param_2,"shouldLoadAllProfilesAtStartup");
  *(char *)(param_1 + 0xc) = (char)IVar6;
  IVar6 = _objc_msgSend(param_2,"quitRunLoopOnTermination");
  *(char *)((long)param_1 + 0x61) = (char)IVar6;
  IVar6 = _objc_msgSend(param_2,"userDefaultsSuiteName");
  if (IVar6 != 0) {
    IVar6 = _objc_msgSend(param_2,"userDefaultsSuiteName");
    if (IVar6 == 0) {
      local_a8 = 0;
      uStack_a0 = 0;
      local_98 = 0;
    }
    else {
      FUN_01b356b8(&local_a8,IVar6);
    }
    if (*(char *)((long)param_1 + 0x57) < '\0') {
      FUN_00005810(param_1[8],param_1[10] & 0x7fffffffffffffff);
    }
    param_1[9] = uStack_a0;
    param_1[8] = local_a8;
    param_1[10] = local_98;
  }
  puVar8 = local_68;
  if (local_68 != (undefined8 *)0x0) {
    if (local_60 * 0x18 != 0) {
      lVar12 = local_60 * -0x18;
      puVar7 = local_68 + local_60 * 3;
      do {
        puVar13 = puVar7 + -3;
        if (puVar13 == (undefined8 *)0x0) goto LAB_043ec9f4;
        if (*(char *)((long)puVar7 + -1) < '\0') {
          FUN_00005810(*puVar13,puVar7[-1] & 0x7fffffffffffffff);
        }
        lVar12 = lVar12 + 0x18;
        puVar7 = puVar13;
      } while (lVar12 != 0);
    }
    local_60 = ((long)puVar8 - (long)local_68 >> 3) * -0x5555555555555555;
    FUN_00005810(local_68,local_58 * 0x18);
  }
  return 0;
}


