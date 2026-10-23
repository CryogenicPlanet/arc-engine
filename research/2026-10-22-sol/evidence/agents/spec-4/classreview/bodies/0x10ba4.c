// ==== 0x10ba4  FUN_00010ba4  (748 bytes)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00010ba4(void *param_1)

{
  long *plVar1;
  undefined1 auVar2 [16];
  undefined8 uVar3;
  undefined8 *puVar4;
  long lVar5;
  undefined8 uVar6;
  code *UNRECOVERED_JUMPTABLE;
  ulong uVar7;
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  undefined1 auVar10 [16];
  undefined1 auVar11 [16];
  undefined1 auVar12 [16];
  undefined1 auVar13 [16];
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  void *local_40;
  long local_38;
  
  uVar3 = DAT_0e2f1b18;
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  UNRECOVERED_JUMPTABLE = *(code **)(PTR_PTR_0e2fe388 + 0x40);
  if (UNRECOVERED_JUMPTABLE == FUN_03b4ad8c) {
    if (((DAT_0e4a4060 & 1) != 0) && (param_1 != (void *)0x0)) {
      lVar5 = thunk_FUN_0838f5e4();
      uVar7 = *(ulong *)(lVar5 + 0x28) & (ulong)((long)param_1 * 0x4bfdb9df5a6f243b) >> 0x20;
      if (*(ulong *)(lVar5 + 0x10) <= uVar7) {
                    /* WARNING: Does not return */
        UNRECOVERED_JUMPTABLE = (code *)SoftwareBreakpoint(1,0x10e90);
        (*UNRECOVERED_JUMPTABLE)();
      }
      lVar5 = *(long *)(lVar5 + 8);
      local_40 = param_1;
      uVar6 = thunk_FUN_009e0900(&local_40,8);
      auVar8._8_8_ = uVar6;
      auVar8._0_8_ = uVar6;
      auVar11 = NEON_ushl(auVar8,_DAT_0d72dd00,8);
      auVar13 = NEON_ushl(auVar8,_DAT_0d72d770,8);
      auVar15 = NEON_ushl(auVar8,_DAT_0d72dd10,8);
      auVar9[9] = 0xff;
      auVar9._0_9_ = _DAT_0d72dd20;
      auVar9[10] = 0xff;
      auVar9[0xb] = 0xff;
      auVar9[0xc] = 0xff;
      auVar9[0xd] = 0xff;
      auVar9[0xe] = 0xff;
      auVar9[0xf] = 0xff;
      auVar9 = NEON_ushl(auVar8,auVar9,8);
      auVar10[0] = auVar9[0] & 0x3f;
      auVar10._1_7_ = 0;
      auVar10[8] = auVar9[8] & 0x3f;
      auVar10._9_7_ = 0;
      auVar16[0] = auVar15[0] & 0x3f;
      auVar16._1_7_ = 0;
      auVar16[8] = auVar15[8] & 0x3f;
      auVar16._9_7_ = 0;
      auVar14[0] = auVar13[0] & 0x3f;
      auVar14._1_7_ = 0;
      auVar14[8] = auVar13[8] & 0x3f;
      auVar14._9_7_ = 0;
      auVar12[0] = auVar11[0] & 0x3f;
      auVar12._1_7_ = 0;
      auVar12[8] = auVar11[8] & 0x3f;
      auVar12._9_7_ = 0;
      auVar11[8] = 1;
      auVar11._0_8_ = 1;
      auVar11._9_7_ = 0;
      auVar11 = NEON_ushl(auVar11,auVar12,8);
      auVar13[8] = 1;
      auVar13._0_8_ = 1;
      auVar13._9_7_ = 0;
      auVar13 = NEON_ushl(auVar13,auVar14,8);
      auVar15[8] = 1;
      auVar15._0_8_ = 1;
      auVar15._9_7_ = 0;
      auVar15 = NEON_ushl(auVar15,auVar16,8);
      auVar2[8] = 1;
      auVar2._0_8_ = 1;
      auVar2._9_7_ = 0;
      auVar9 = NEON_ushl(auVar2,auVar10,8);
      plVar1 = (long *)(lVar5 + uVar7 * 0x10);
      if ((CONCAT17(auVar13[7] | auVar11[7] | auVar9[7] | auVar15[7] |
                    auVar13[0xf] | auVar11[0xf] | auVar9[0xf] | auVar15[0xf],
                    CONCAT16(auVar13[6] | auVar11[6] | auVar9[6] | auVar15[6] |
                             auVar13[0xe] | auVar11[0xe] | auVar9[0xe] | auVar15[0xe],
                             CONCAT15(auVar13[5] | auVar11[5] | auVar9[5] | auVar15[5] |
                                      auVar13[0xd] | auVar11[0xd] | auVar9[0xd] | auVar15[0xd],
                                      CONCAT14(auVar13[4] | auVar11[4] | auVar9[4] | auVar15[4] |
                                               auVar13[0xc] | auVar11[0xc] |
                                               auVar9[0xc] | auVar15[0xc],
                                               CONCAT13(auVar13[3] | auVar11[3] |
                                                        auVar9[3] | auVar15[3] |
                                                        auVar13[0xb] | auVar11[0xb] |
                                                        auVar9[0xb] | auVar15[0xb],
                                                        CONCAT12(auVar13[2] | auVar11[2] |
                                                                 auVar9[2] | auVar15[2] |
                                                                 auVar13[10] | auVar11[10] |
                                                                 auVar9[10] | auVar15[10],
                                                                 CONCAT11(auVar13[1] | auVar11[1] |
                                                                          auVar9[1] | auVar15[1] |
                                                                          auVar13[9] | auVar11[9] |
                                                                          auVar9[9] | auVar15[9],
                                                                          auVar13[0] | auVar11[0] |
                                                                          auVar9[0] | auVar15[0] |
                                                                          auVar13[8] | auVar11[8] |
                                                                          auVar9[8] | auVar15[8]))))
                                     ))) & (plVar1[1] ^ 0xffffffffffffffffU)) == 0) {
        for (puVar4 = (undefined8 *)*plVar1; puVar4 != (undefined8 *)0x0;
            puVar4 = (undefined8 *)puVar4[1]) {
          if ((void *)*puVar4 == param_1) {
            uVar7 = thunk_FUN_0838f178();
            if ((uVar7 & 1) == 0) {
              thunk_FUN_0838f688(uVar3,param_1);
            }
            break;
          }
        }
      }
    }
    UNRECOVERED_JUMPTABLE = *(code **)(DAT_0e2f1bd8 + 0x40);
    if (UNRECOVERED_JUMPTABLE == FUN_00010e90) {
      UNRECOVERED_JUMPTABLE = *(code **)(PTR_PTR_0e2fe380 + 0x40);
      if (UNRECOVERED_JUMPTABLE == FUN_03b624d8) {
        if (((param_1 == (void *)0x0) || (((ulong)param_1 & 0xfffffff800000000) == DAT_0e2f2200)) ||
           ((DAT_0e2f2218 & (ulong)param_1) == DAT_0e2f2210)) {
          if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
            FUN_00010f1c(param_1);
            return;
          }
          goto LAB_00010d30;
        }
      }
      else {
        if (UNRECOVERED_JUMPTABLE != (code *)&LAB_03b626d8) goto LAB_00010c90;
        if (((param_1 == (void *)0x0) || (((ulong)param_1 & 0xfffffff800000000) == DAT_0e2f2200)) ||
           ((DAT_0e2f2218 & (ulong)param_1) == DAT_0e2f2210)) {
          if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
            FUN_000314f8(param_1);
            return;
          }
          goto LAB_00010d30;
        }
      }
      if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
        _free(param_1);
        return;
      }
      goto LAB_00010d30;
    }
  }
LAB_00010c90:
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
                    /* WARNING: Could not recover jumptable at 0x00010cc0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(param_1,0);
    return;
  }
LAB_00010d30:
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


