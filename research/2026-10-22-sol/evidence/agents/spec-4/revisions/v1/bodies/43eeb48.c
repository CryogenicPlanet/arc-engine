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


