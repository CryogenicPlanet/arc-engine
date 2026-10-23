// ==== 0x7cd9f40  FUN_07cd9f40  (608 bytes)

void FUN_07cd9f40(long param_1,undefined1 *param_2,ulong param_3)

{
  code *pcVar1;
  int iVar2;
  ulong uVar3;
  ulong uVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  long lVar7;
  undefined1 *puVar8;
  long lVar9;
  undefined1 *puVar10;
  undefined1 *local_70;
  long local_68;
  undefined8 uStack_60;
  undefined1 *local_58;
  
  uVar3 = FUN_07cd9bdc();
  if ((uVar3 & 1) == 0) {
    lVar7 = *(long *)(param_1 + 200);
    if (lVar7 == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x7cda198);
      (*pcVar1)();
    }
    puVar8 = *(undefined1 **)(param_1 + 0xc0);
    lVar9 = lVar7 * 0x18;
    puVar6 = puVar8;
    local_70 = param_2;
    if (lVar9 != 0) {
      puVar10 = puVar8 + lVar7 * 0x18;
      do {
        uVar3 = FUN_040beea0(&local_70,puVar6);
        puVar8 = puVar6;
        if ((uVar3 & 1) != 0) break;
        puVar6 = puVar6 + 0x18;
        lVar9 = lVar9 + -0x18;
        puVar8 = puVar10;
      } while (lVar9 != 0);
      lVar7 = *(long *)(param_1 + 200);
      puVar6 = *(undefined1 **)(param_1 + 0xc0);
    }
    puVar10 = puVar6 + lVar7 * 0x18;
    if (puVar8 == puVar10) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x7cda1a4);
      (*pcVar1)();
    }
    uVar3 = ((long)puVar8 - (long)puVar6 >> 3) * -0x5555555555555555;
    if (uVar3 - param_3 != 0) {
      if (uVar3 < param_3) {
        if (puVar6 + param_3 * 0x18 + 0x18 <= puVar10) {
          puVar10 = puVar6 + param_3 * 0x18 + 0x18;
        }
        do {
          puVar8 = puVar10;
          if (puVar8 == puVar6) break;
          uVar4 = FUN_07cd9bdc(param_1,puVar8 + -0x18);
          puVar10 = puVar8 + -0x18;
        } while ((uVar4 & 1) != 0);
      }
      else {
        puVar8 = puVar6 + param_3 * 0x18;
        while ((puVar10 != puVar8 && (iVar2 = FUN_07cd9bdc(param_1,puVar8), iVar2 != 0))) {
          puVar8 = puVar8 + 0x18;
        }
      }
      local_68 = 0;
      uStack_60 = 0;
      local_70 = (undefined1 *)0x0;
      FUN_02f555f4(*(undefined8 *)(param_1 + 0x78),"extensions.pinned_extensions",&local_70);
      if (puVar8 == (undefined1 *)(*(long *)(param_1 + 0xc0) + *(long *)(param_1 + 200) * 0x18)) {
        puVar6 = local_70 + local_68 * 0x18;
      }
      else {
        lVar7 = local_68 * 0x18;
        puVar6 = local_70;
        if (lVar7 != 0) {
          puVar5 = local_70 + local_68 * 0x18;
          puVar10 = local_70;
          local_58 = puVar8;
          do {
            uVar4 = FUN_040beea0(&local_58,puVar10);
            puVar6 = puVar10;
            if ((uVar4 & 1) != 0) break;
            puVar10 = puVar10 + 0x18;
            lVar7 = lVar7 + -0x18;
            puVar6 = puVar5;
          } while (lVar7 != 0);
        }
      }
      lVar7 = local_68 * 0x18;
      local_58 = param_2;
      puVar8 = local_70;
      while ((lVar7 != 0 && (uVar4 = FUN_040beea0(&local_58,puVar8), (uVar4 & 1) == 0))) {
        puVar8 = puVar8 + 0x18;
        lVar7 = lVar7 + -0x18;
      }
      if (puVar8 == local_70 + local_68 * 0x18) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x7cda1b0);
        (*pcVar1)();
      }
      if (uVar3 < param_3) {
        puVar10 = puVar8 + 0x18;
        puVar5 = puVar8;
      }
      else {
        puVar5 = puVar6;
        puVar10 = puVar8;
        puVar6 = puVar8 + 0x18;
      }
      FUN_04d385d8(puVar5,puVar10,puVar6);
      thunk_FUN_06af9204(*(undefined8 *)(param_1 + 0x78),&local_70);
      local_58 = (undefined1 *)&local_70;
      FUN_01a80e40(&local_58);
    }
  }
  return;
}


