// ==== 0x122014  FUN_00122014  (732 bytes)

void FUN_00122014(ulong param_1,ulong param_2)

{
  ushort uVar1;
  code *pcVar2;
  bool bVar3;
  SEL extraout_x1;
  SEL extraout_x1_00;
  long lVar4;
  long lVar5;
  undefined *puVar6;
  long lVar7;
  ulong uVar8;
  long *plVar9;
  ulong uVar10;
  long lVar11;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined8 local_40;
  undefined8 uStack_38;
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((param_2 != 0) &&
     ((param_1 & 0xfffffff800000000) == DAT_0e2f2200 || (DAT_0e2f2218 & param_1) == DAT_0e2f2210)) {
    uVar10 = param_1 & 0xfffffffc00000000;
    if (uVar10 == DAT_0e2f2208) {
      puVar6 = &DAT_0e2f6290;
      uVar10 = 0x3ffffffff;
      lVar11 = 2;
    }
    else {
      if (uVar10 != DAT_0e2f2200 && (DAT_0e2f2218 & param_1) != DAT_0e2f2210) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x1222a0);
        (*pcVar2)();
      }
      bVar3 = uVar10 != DAT_0e2f2200;
      puVar6 = &DAT_0e2f2290;
      if (bVar3) {
        puVar6 = &DAT_0e2fa290;
      }
      uVar10 = 0x3ffffffff;
      if (bVar3) {
        uVar10 = ~DAT_0e2f2218;
      }
      lVar11 = 3;
      if (!bVar3) {
        lVar11 = 1;
      }
    }
    uVar1 = *(ushort *)(puVar6 + ((uVar10 & param_1) >> 0x15) * 2);
    if (uVar1 == 0xfffe) {
      lVar7 = 0;
      lVar4 = 1;
    }
    else if (uVar1 == 0xffff) {
      lVar4 = 0;
      lVar7 = 0;
    }
    else {
      lVar7 = (param_1 & 0xffffffffffe00000) + (ulong)uVar1 * -0x200000;
      lVar4 = 2;
    }
    if (lVar4 != 0) {
      if (CARRY8(param_1,param_2)) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x1222a8);
        (*pcVar2)();
      }
      lVar5 = (&DAT_0e2f2220)[lVar11];
      if (lVar4 == 2) {
        uVar10 = lVar7 + (1 << (ulong)(*(int *)PTR__vm_page_shift_0d824150 + 2U & 0x1f));
        uVar8 = (long)*(int *)PTR__vm_page_shift_0d824150 + 2;
        lVar11 = (uVar10 & 0xffffffffffe00000) + lVar5 +
                 ((uVar10 & 0x1fffff) >> (uVar8 & 0x3f)) * 0x20;
        uVar10 = lVar11 + ((ulong)*(byte *)(lVar11 + 0x1e) & 0x3f) * 0x20;
        lVar11 = *(long *)PTR__vm_page_size_0d824160;
        uVar10 = (((lVar11 - 1U & uVar10) >> 5) << (uVar8 & 0x3f)) +
                 (uVar10 - lVar5 & 0xffffffffffe00000);
        if (uVar10 == 0) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x122278);
          (*pcVar2)();
        }
      }
      else {
        uVar8 = (long)*(int *)PTR__vm_page_shift_0d824150 + 2;
        lVar4 = lVar5 + (param_1 & 0xffffffffffe00000) +
                ((param_1 & 0x1fffff) >> (uVar8 & 0x3f)) * 0x20;
        uVar10 = lVar4 + ((ulong)*(byte *)(lVar4 + 0x1e) & 0x3f) * -0x20;
        lVar4 = *(long *)(uVar10 + 0x10);
        if (lVar4 == 0) {
          local_40 = 0;
          uStack_38 = 0;
          thunk_FUN_083c5b7c(&local_40,"address",7);
          ArcWebContentsConfiguration::_cxx_destruct((ID)&local_40,extraout_x1);
          local_50 = 0;
          uStack_48 = 0;
          thunk_FUN_083c5b7c(&local_50,"pool",4,lVar11);
          ArcWebContentsConfiguration::_cxx_destruct((ID)&local_50,extraout_x1_00);
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x1222f0);
          (*pcVar2)();
        }
        lVar11 = *(long *)PTR__vm_page_size_0d824160;
        lVar7 = (((lVar11 - 1U & uVar10) >> 5) << (uVar8 & 0x3f)) +
                (uVar10 - lVar5 & 0xffffffffffe00000);
        uVar10 = lVar7 + ((param_1 - lVar7) * *(long *)(lVar4 + 0x28) >> 0x2a) *
                         *(long *)(lVar4 + 0x18);
      }
      lVar4 = (uVar10 & 0xffffffffffe00000) + lVar5 + ((uVar10 & 0x1fffff) >> (uVar8 & 0x3f)) * 0x20
      ;
      uVar8 = lVar4 + ((ulong)*(byte *)(lVar4 + 0x1e) & 0x3f) * -0x20;
      if (*(int *)(uVar8 + 0x18) < 0) {
        plVar9 = (long *)(uVar8 + 0x20);
      }
      else {
        plVar9 = (long *)(*(long *)(uVar8 + 0x10) + 0x18);
      }
      bVar3 = param_1 + param_2 < uVar10 ||
              (*plVar9 + uVar10) - (ulong)*(uint *)(*(long *)(uVar8 & -lVar11) + 0x80) <
              param_1 + param_2;
      goto LAB_00122064;
    }
  }
  bVar3 = false;
LAB_00122064:
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(bVar3);
}


