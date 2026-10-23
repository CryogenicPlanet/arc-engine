// ==== 0x4345480  FUN_04345480  (444 bytes)

void FUN_04345480(undefined **param_1,undefined8 *param_2,undefined8 param_3)

{
  byte bVar1;
  uint uVar2;
  undefined **ppuVar3;
  undefined1 *puVar4;
  code *pcVar5;
  undefined **ppuVar6;
  long lVar7;
  undefined8 *puVar8;
  ulong uVar9;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined **local_e8;
  undefined1 *local_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined1 auStack_c8 [128];
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  bVar1 = *(byte *)((long)param_2 + 0x17);
  uVar9 = param_2[1];
  if (-1 < (char)bVar1) {
    uVar9 = (ulong)bVar1;
  }
  if (uVar9 != 0) {
    puVar8 = (undefined8 *)*param_2;
    if (-1 < (char)bVar1) {
      puVar8 = param_2;
    }
    local_e8 = &PTR_FUN_0db241f0;
    uStack_d0 = 0;
    local_d8 = 0x80;
    local_110 = 0;
    uStack_f8 = 0;
    uStack_100 = 0;
    uStack_108 = 0xffffffff00000000;
    local_e0 = auStack_c8;
    FUN_00271380(puVar8,uVar9,&local_e8,&local_110);
    uVar2 = (uint)local_110;
    local_e8 = &PTR_FUN_0db241f0;
    if ((local_e0 != auStack_c8) && (local_e0 != (undefined1 *)0x0)) {
      FUN_00010ba4();
    }
    if ((uVar2 & 0xfffffffe) != 2) {
      local_e8 = (undefined **)0x0;
      local_e0 = (undefined1 *)0x0;
      local_d8 = 0;
      lVar7 = (long)*(char *)((long)param_2 + 0x17);
      puVar8 = param_2;
      if (lVar7 < 0) {
        puVar8 = (undefined8 *)*param_2;
        lVar7 = param_2[1];
      }
      FUN_00da58a0(&local_e8,puVar8,lVar7,0,param_3);
      puVar4 = local_e0;
      ppuVar3 = local_e8;
      if (((char)local_d8 != '\0') && (local_e0 != (undefined1 *)0x0)) {
        if ((undefined1 *)0x7fffffffffffffee < local_e0) {
                    /* WARNING: Subroutine does not return */
          FUN_04b089c0();
        }
        if ((undefined1 *)((long)&MACH_HEADER.sizeofcmds + 2) < local_e0) {
          uVar9 = (ulong)local_e0 & 0x7ffffffffffffff0;
          ppuVar6 = (undefined **)FUN_00008790(uVar9 + 0x10);
          *param_1 = (undefined *)ppuVar6;
          param_1[1] = puVar4;
          param_1[2] = (undefined *)(uVar9 + 0x8000000000000010);
        }
        else {
          *(char *)((long)param_1 + 0x17) = (char)local_e0;
          ppuVar6 = param_1;
        }
        if (((undefined **)((long)ppuVar6 + (long)puVar4) < ppuVar6) ||
           ((ppuVar6 <= ppuVar3 && (ppuVar3 < (undefined **)((long)ppuVar6 + (long)puVar4))))) {
                    /* WARNING: Does not return */
          pcVar5 = (code *)SoftwareBreakpoint(1,0x4345624);
          (*pcVar5)();
        }
        _memmove(ppuVar6,ppuVar3,(size_t)puVar4);
        *(undefined1 *)((long)ppuVar6 + (long)puVar4) = 0;
        goto LAB_04345570;
      }
    }
  }
  *param_1 = (undefined *)0x0;
  param_1[1] = (undefined *)0x0;
  param_1[2] = (undefined *)0x0;
LAB_04345570:
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


