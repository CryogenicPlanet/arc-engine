// ==== 0x8790  FUN_00008790  (660 bytes)

/* WARNING: Removing unreachable block (ram,0x00008a1c) */

void FUN_00008790(undefined8 param_1)

{
  undefined8 uVar1;
  undefined *puVar2;
  uint uVar3;
  long lVar4;
  undefined *puVar5;
  void *pvVar6;
  code *pcVar7;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined2 local_110;
  undefined1 local_10e;
  undefined4 local_10d;
  undefined1 local_109;
  undefined8 local_108;
  undefined8 uStack_100;
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined1 local_58;
  undefined2 local_57;
  undefined1 local_55;
  undefined1 local_54;
  undefined2 local_53;
  undefined1 local_51;
  undefined4 local_50;
  undefined3 uStack_4c;
  long local_48;
  
  puVar2 = PTR_PTR_0e2fe388;
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  if (*(code **)PTR_PTR_0e2fe388 != FUN_03b4a5ac) {
    lVar4 = (**(code **)PTR_PTR_0e2fe388)(param_1,0,0);
    goto joined_r0x00008840;
  }
  pcVar7 = (code *)*DAT_0e2f1bd8;
  if (pcVar7 == FUN_00011d10) {
    pcVar7 = *(code **)PTR_PTR_0e2fe380;
    puVar5 = DAT_0e4a5380;
    if (pcVar7 == FUN_03b621f4) {
      if (DAT_0e4a5380 == (undefined *)0x0) {
        do {
        } while (DAT_0e4a9080 != '\0');
        DAT_0e4a9080 = 1;
        local_109 = 0;
        local_10d = 0;
        local_57 = 0;
        local_55 = 0;
        local_53 = 0;
        local_51 = 0;
        uStack_118 = 0xffffffffffffffff;
        local_120 = 0;
        local_110 = 1;
        local_10e = 0;
        local_108 = 0;
        uStack_100 = 0;
        local_f8 = 0;
        uStack_f0 = 0xf0000;
        uStack_e0 = 0;
        local_e8 = 0;
        uStack_d0 = 0;
        local_d8 = 0;
        uStack_c0 = 0;
        local_c8 = 0;
        local_b8 = 0xf0000;
        uStack_98 = 0;
        local_a0 = 0;
        uStack_88 = 0;
        uStack_90 = 0;
        uStack_a8 = 0;
        local_b0 = 0;
        local_80 = 0xf0000;
        uStack_70 = 0;
        local_78 = 0;
        uStack_60 = 0;
        local_68 = 0;
        local_58 = 0;
        local_54 = 0;
        local_50 = 0;
        uStack_4c = 0;
        FUN_013ad53c(&DAT_0e4a5400,&local_120);
        DAT_0e4a5380 = &DAT_0e4a5400;
        DAT_0e4a9080 = '\0';
        puVar5 = DAT_0e4a5380;
      }
    }
    else {
      if (pcVar7 != (code *)&LAB_03b6261c) goto LAB_00008898;
      if (DAT_0e4a5380 == (undefined *)0x0) {
        puVar5 = (undefined *)FUN_0445ac8c();
      }
    }
    lVar4 = FUN_00011f04(puVar5,param_1,0);
  }
  else {
LAB_00008898:
    lVar4 = (*pcVar7)(param_1,0,0);
  }
  uVar3 = DAT_0e4a4060;
  uVar1 = DAT_0e2f1b18;
  if (((DAT_0e4a4060 & 1) != 0) && ((DAT_0e4a4060 >> 2 & 1) == 0)) {
    pvVar6 = _pthread_getspecific(DAT_0e4a2fc8);
    _pthread_setspecific(DAT_0e4a2fc8,(void *)((long)&MACH_HEADER.magic + 1));
    if (pvVar6 == (void *)0x0) {
      thunk_FUN_0838f230(uVar1,uVar3,lVar4,param_1,2,0);
      _pthread_setspecific(DAT_0e4a2fc8,(void *)0x0);
    }
  }
joined_r0x00008840:
  while ((lVar4 == 0 && (pcVar7 = (code *)thunk_FUN_0c449698(), pcVar7 != (code *)0x0))) {
    (*pcVar7)();
    lVar4 = (**(code **)puVar2)(param_1,0,0);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail();
  }
  return;
}


