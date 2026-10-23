// ==== 0x42aae64  FUN_042aae64  (1276 bytes)

/* WARNING: Removing unreachable block (ram,0x042ab160) */
/* WARNING: Removing unreachable block (ram,0x042ab228) */
/* WARNING: Removing unreachable block (ram,0x042ab178) */

void FUN_042aae64(long param_1)

{
  code *pcVar1;
  int iVar2;
  undefined8 uVar3;
  long lVar4;
  undefined8 *puVar5;
  ulong uVar6;
  undefined4 *puVar7;
  undefined8 *puVar8;
  ulong uVar9;
  int iVar10;
  undefined8 *puVar11;
  ulong uVar12;
  undefined6 local_258;
  undefined2 uStack_252;
  undefined6 uStack_250;
  char local_24a [2];
  undefined7 local_248;
  char cStack_241;
  undefined8 *local_240;
  char local_238 [8];
  undefined7 local_230;
  char cStack_229;
  undefined1 auStack_228 [128];
  undefined8 local_1a8;
  undefined8 uStack_1a0;
  ulong local_198;
  undefined8 local_190;
  undefined8 uStack_188;
  undefined8 uStack_180;
  undefined8 uStack_178;
  undefined4 local_170;
  undefined6 local_160;
  undefined2 uStack_15a;
  undefined6 uStack_158;
  char local_152 [2];
  undefined7 local_150;
  char cStack_149;
  undefined8 local_148;
  char local_140 [8];
  undefined7 local_138;
  char cStack_131;
  undefined1 auStack_130 [128];
  undefined8 *local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 uStack_88;
  ulong local_80;
  long local_78;
  
  local_78 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar7 = *(undefined4 **)(param_1 + 8);
  *(undefined8 *)(param_1 + 8) = 0;
  if (puVar7 != (undefined4 *)0x0) {
    uVar3 = FUN_02736300();
    FUN_02dd6dc0(uVar3,*puVar7,puVar7[1]);
    FUN_00005810(puVar7,8);
  }
  lVar4 = FUN_02736300();
  if ((*(int *)(lVar4 + 0x50) < 1) ||
     ((*(int *)(lVar4 + 0x50) == *(int *)(lVar4 + 0x54) && ((*(byte *)(lVar4 + 0x59) & 1) != 0)))) {
    uVar3 = FUN_02bed438();
    cStack_229 = '\b';
    if ("shutdown" < local_238 && &local_240 < (undefined8 **)0xd2393a2) goto LAB_042ab34c;
    local_240 = (undefined8 *)0x6e776f6474756873;
    local_238[0] = '\0';
    cStack_241 = '\x0e';
    if ("Quit main loop" < local_24a && &local_258 < "uit main loop") goto LAB_042ab34c;
    local_258 = 0x6d2074697551;
    uStack_252 = 0x6961;
    uStack_250 = 0x706f6f6c206e;
    local_24a[0] = '\0';
    FUN_08b903b4(auStack_228,&local_240,&local_258);
    FUN_02bed570(uVar3,auStack_228);
    FUN_08b90450(auStack_228);
    if (cStack_241 < '\0') {
      FUN_00005810(CONCAT26(uStack_252,local_258),
                   CONCAT17(cStack_241,local_248) & 0x7fffffffffffffff);
    }
    if (-1 < cStack_229) goto LAB_042aafd4;
    uVar6 = CONCAT17(cStack_229,local_230) & 0x7fffffffffffffff;
    puVar5 = local_240;
  }
  else {
    iVar10 = 0;
    uVar9 = 0;
    puVar8 = (undefined8 *)0x0;
    uVar12 = 0;
    do {
      uVar3 = FUN_02736300();
      iVar2 = FUN_02913f10(uVar3,iVar10);
      puVar5 = puVar8;
      uVar6 = uVar12;
      if (iVar2 != 0) {
        FUN_01261c00(&local_a8,iVar10);
        if (uVar9 == uVar12) {
          uVar9 = uVar12 + 1;
          if (0xaaaaaaaaaaaaaaa < uVar9) {
                    /* WARNING: Subroutine does not return */
            FUN_04047380();
          }
          uVar6 = uVar12 * 2;
          if (uVar6 < uVar9 || uVar6 - uVar9 == 0) {
            uVar6 = uVar9;
          }
          if (0x555555555555554 < uVar12) {
            uVar6 = 0xaaaaaaaaaaaaaaa;
          }
          if (uVar6 == 0) {
            lVar4 = 0;
          }
          else {
            if (0xaaaaaaaaaaaaaaa < uVar6) {
                    /* WARNING: Subroutine does not return */
              FUN_04b087f0();
            }
            lVar4 = FUN_00008790(uVar6 * 0x18);
          }
          puVar11 = (undefined8 *)(lVar4 + uVar12 * 0x18);
          if (puVar11 == (undefined8 *)0x0) goto LAB_042ab34c;
          puVar11[2] = local_98;
          puVar11[1] = uStack_a0;
          *puVar11 = local_a8;
          uStack_a0 = 0;
          local_98 = 0;
          local_a8 = 0;
          puVar5 = puVar11 + uVar12 * -3;
          if (uVar12 != 0) {
            _memcpy(puVar5,puVar8,uVar12 * 0x18);
          }
          if (puVar8 != (undefined8 *)0x0) {
            FUN_00005810(puVar8,uVar12 * 0x18);
          }
        }
        else {
          puVar11 = puVar8 + uVar9 * 3;
          if (puVar11 == (undefined8 *)0x0) goto LAB_042ab34c;
          puVar11[2] = local_98;
          puVar11[1] = uStack_a0;
          *puVar11 = local_a8;
          uStack_a0 = 0;
          local_98 = 0;
          local_a8 = 0;
        }
        lVar4 = (long)puVar11 + (0x18 - (long)puVar5);
        if (lVar4 == 0) goto LAB_042ab34c;
        uVar9 = (lVar4 >> 3) * -0x5555555555555555;
      }
      iVar10 = iVar10 + 1;
      puVar8 = puVar5;
      uVar12 = uVar6;
    } while (iVar10 != 0x2a);
    uVar3 = FUN_02bed438();
    cStack_131 = '\b';
    if ("shutdown" < local_140 && &local_148 < "hutdown") {
LAB_042ab34c:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x42ab350);
      (*pcVar1)();
    }
    local_148 = 0x6e776f6474756873;
    local_140[0] = '\0';
    cStack_149 = '\x0e';
    if ("Quit main loop" < local_152 && &local_160 < "uit main loop") goto LAB_042ab34c;
    local_160 = 0x6d2074697551;
    uStack_15a = 0x6961;
    uStack_158 = 0x706f6f6c206e;
    local_152[0] = '\0';
    FUN_0136e22c(&local_1a8,puVar5,uVar9,",",1);
    local_98 = CONCAT17(7,(undefined7)local_98);
    if ("origins" < (char *)((ulong)&local_a8 | 7) && &local_a8 < "rigins") goto LAB_042ab34c;
    local_a8 = 0x736e696769726f;
    uStack_88 = uStack_1a0;
    local_90 = local_1a8;
    local_80 = local_198;
    local_1a8 = 0;
    uStack_1a0 = 0;
    local_198 = 0;
    uStack_188 = 0;
    local_190 = 0;
    uStack_178 = 0;
    uStack_180 = 0;
    local_170 = 0x3f800000;
    local_b0 = &local_190;
    FUN_00ba2240(&local_b0,&local_a8,&local_a8);
    FUN_02bed328(auStack_130,&local_148,&local_160,&local_190);
    FUN_02bed570(uVar3,auStack_130);
    FUN_08b90450(auStack_130);
    FUN_04b8e340(&local_190);
    if ((long)local_198 < 0) {
      FUN_00005810(local_1a8,local_198 & 0x7fffffffffffffff);
    }
    if (cStack_149 < '\0') {
      FUN_00005810(CONCAT26(uStack_15a,local_160),
                   CONCAT17(cStack_149,local_150) & 0x7fffffffffffffff);
    }
    if (cStack_131 < '\0') {
      FUN_00005810(local_148,CONCAT17(cStack_131,local_138) & 0x7fffffffffffffff);
    }
    if (puVar5 == (undefined8 *)0x0) goto LAB_042aafd4;
    if (uVar9 != 0) {
      lVar4 = uVar9 * -0x18;
      puVar8 = puVar5 + uVar9 * 3;
      do {
        puVar11 = puVar8 + -3;
        if (puVar11 == (undefined8 *)0x0) goto LAB_042ab34c;
        if (*(char *)((long)puVar8 + -1) < '\0') {
          FUN_00005810(*puVar11,puVar8[-1] & 0x7fffffffffffffff);
        }
        lVar4 = lVar4 + 0x18;
        puVar8 = puVar11;
      } while (lVar4 != 0);
    }
    uVar6 = uVar6 * 0x18;
  }
  FUN_00005810(puVar5,uVar6);
LAB_042aafd4:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_78) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail();
  }
  return;
}


