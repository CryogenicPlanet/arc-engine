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


