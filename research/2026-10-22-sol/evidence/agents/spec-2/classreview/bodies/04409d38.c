// ==== 0x4409d38  ArcNotification::category  (740 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::category(ID param_1,SEL param_2)

{
  ulong uVar1;
  undefined8 **ppuVar2;
  undefined8 *puVar3;
  long lVar4;
  ulong uVar5;
  code *pcVar6;
  ID IVar7;
  ulong uVar8;
  long lVar9;
  long lVar10;
  undefined8 **ppuVar11;
  undefined8 *puVar12;
  undefined8 *puVar13;
  long local_b0;
  long local_a8;
  long local_a0;
  undefined8 **local_98;
  ulong local_90;
  ulong local_88;
  undefined8 **local_80;
  undefined8 **local_78;
  ulong local_70;
  ulong local_68;
  
  local_98 = (undefined8 **)0x0;
  local_90 = 0;
  local_88 = 0;
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    FUN_0433e888(&local_b0,*(undefined8 *)(param_1 + 0x10));
    lVar4 = local_b0;
    lVar9 = local_a8 * 0x50;
    if (lVar9 != 0) {
      lVar10 = 0;
      do {
        uVar5 = local_90;
        ppuVar11 = local_98;
        puVar3 = (undefined8 *)(lVar4 + lVar10);
        if (local_90 == local_88) {
          uVar1 = local_90 + 1;
          if (0x555555555555555 < uVar1) {
                    /* WARNING: Subroutine does not return */
            FUN_04047380();
          }
          uVar8 = local_88 * 2;
          if (uVar8 < uVar1 || uVar8 - uVar1 == 0) {
            uVar8 = uVar1;
          }
          if (0x2aaaaaaaaaaaaa9 < local_88) {
            uVar8 = 0x555555555555555;
          }
          local_80 = (undefined8 **)0x0;
          if (uVar8 != 0) {
            if (0x555555555555555 < uVar8) {
                    /* WARNING: Subroutine does not return */
              FUN_04b087f0();
            }
            local_80 = (undefined8 **)FUN_00008790(uVar8 * 0x30);
          }
          ppuVar11 = local_80 + uVar5 * 6;
          local_70 = 0;
          local_78 = ppuVar11;
          local_68 = uVar8;
          if (ppuVar11 == (undefined8 **)0x0) goto LAB_04409e98;
          puVar12 = (undefined8 *)(lVar4 + lVar10);
          if (*(char *)((long)puVar12 + 0x17) < '\0') {
            FUN_00054420(ppuVar11,*puVar12,puVar12[1]);
          }
          else {
            puVar13 = (undefined8 *)puVar3[1];
            puVar12 = (undefined8 *)*puVar3;
            ppuVar11[2] = (undefined8 *)puVar3[2];
            ppuVar11[1] = puVar13;
            *ppuVar11 = puVar12;
          }
          if (*(char *)(lVar4 + lVar10 + 0x47) < '\0') {
            FUN_00054420(ppuVar11 + 3,puVar3[6],*(undefined8 *)(lVar4 + lVar10 + 0x38));
          }
          else {
            puVar13 = (undefined8 *)puVar3[7];
            puVar12 = (undefined8 *)puVar3[6];
            ppuVar11[5] = (undefined8 *)puVar3[8];
            ppuVar11[4] = puVar13;
            ppuVar11[3] = puVar12;
          }
          ppuVar11 = ppuVar11 + 6;
          ppuVar2 = local_78 + local_90 * -6;
          _memcpy(ppuVar2,local_98,local_90 * 0x30);
          uVar5 = local_68;
          local_90 = ((long)ppuVar11 - (long)ppuVar2 >> 4) * -0x5555555555555555;
          local_70 = 0;
          local_68 = local_88;
          local_88 = uVar5;
          local_80 = local_98;
          local_78 = local_98;
          local_98 = ppuVar2;
          FUN_0577fc24(&local_80);
        }
        else {
          FUN_0440a5f0(&local_98,lVar4 + lVar10,puVar3 + 6);
          ppuVar11 = ppuVar11 + uVar5 * 6 + 6;
        }
        local_90 = ((long)ppuVar11 - (long)local_98 >> 4) * -0x5555555555555555;
        if ((long)ppuVar11 - (long)local_98 == 0) goto LAB_04409e98;
        lVar10 = lVar10 + 0x50;
      } while (lVar9 - lVar10 != 0);
    }
    lVar4 = local_b0;
    if (local_b0 != 0) {
      if (local_a8 * 0x50 != 0) {
        lVar10 = local_a8 * 0x50 + local_b0;
        lVar9 = local_a8 * -0x50;
        do {
          if (lVar10 == 0x50) goto LAB_04409e98;
          lVar10 = FUN_0433e338();
          lVar9 = lVar9 + 0x50;
        } while (lVar9 != 0);
      }
      local_a8 = (lVar4 - local_b0 >> 4) * -0x3333333333333333;
      FUN_00005810(local_b0,local_a0 * 0x50);
    }
    if (((*(byte *)(param_1 + 0x20) & 1) != 0) &&
       (FUN_0433e240(&local_80,*(undefined8 *)(*(long *)(param_1 + 0x10) + 0x368),
                     *(long *)(param_1 + 0x10) + 0x10), (*(byte *)(param_1 + 0x20) & 1) != 0)) {
      IVar7 = FUN_0473ed28(&local_80,&local_98,*(int *)(*(long *)(param_1 + 0x10) + 0x310) != 0);
      if ((long)local_70 < 0) {
        FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
      }
      local_80 = &local_98;
      FUN_0577ee50(&local_80);
      return IVar7;
    }
  }
LAB_04409e98:
                    /* WARNING: Does not return */
  pcVar6 = (code *)SoftwareBreakpoint(1,0x4409e9c);
  (*pcVar6)();
}


