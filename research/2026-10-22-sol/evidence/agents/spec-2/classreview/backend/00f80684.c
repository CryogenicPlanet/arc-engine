// ==== 0xf80684  FUN_00f80684  (328 bytes)

undefined8 * FUN_00f80684(ulong param_1)

{
  ulong uVar1;
  uint uVar2;
  undefined8 *puVar3;
  long lVar4;
  long lVar5;
  pthread_mutexattr_t local_48;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if (DAT_0e3bd5b0 < (undefined8 *)((long)&MACH_HEADER.magic + 2)) {
    if (DAT_0e3bd5b0 == (undefined8 *)0x0) {
      DAT_0e3bd5b0 = (undefined8 *)0x1;
      puVar3 = (undefined8 *)FUN_00008790(0x88);
      puVar3[2] = 0;
      puVar3[3] = 0;
      puVar3[4] = 0;
      *puVar3 = &PTR_LAB_0da8c9a0;
      puVar3[1] = puVar3 + 2;
      *(undefined1 *)(puVar3 + 5) = 0;
      local_48.__sig = 0;
      local_48.__opaque[0] = '\0';
      local_48.__opaque[1] = '\0';
      local_48.__opaque[2] = '\0';
      local_48.__opaque[3] = '\0';
      local_48.__opaque[4] = '\0';
      local_48.__opaque[5] = '\0';
      local_48.__opaque[6] = '\0';
      local_48.__opaque[7] = '\0';
      _pthread_mutexattr_init(&local_48);
      _pthread_mutexattr_setprotocol(&local_48,1);
      _pthread_mutex_init((pthread_mutex_t *)(puVar3 + 6),&local_48);
      uVar2 = _pthread_mutexattr_destroy(&local_48);
      param_1 = (ulong)uVar2;
      puVar3[0xe] = 0;
      puVar3[0xf] = 0;
      *(undefined1 *)(puVar3 + 0x10) = 0;
      DAT_0e3bd5b0 = puVar3;
    }
    else if (DAT_0e3bd5b0 == (undefined8 *)((long)&MACH_HEADER.magic + 1)) {
      lVar4 = (*(code *)PTR_FUN_0e2f1ea8)();
      do {
        lVar5 = (*(code *)PTR_FUN_0e2f1ea8)();
        uVar1 = lVar5 - lVar4 >> 0x3f ^ 0x8000000000000000;
        if (!SBORROW8(lVar5,lVar4)) {
          uVar1 = lVar5 - lVar4;
        }
        if ((long)uVar1 < 1000) {
          param_1 = FUN_083b146c();
        }
        else {
          param_1 = FUN_00005dd8(1000);
        }
      } while (DAT_0e3bd5b0 == (undefined8 *)((long)&MACH_HEADER.magic + 1));
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return DAT_0e3bd5b0;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(param_1);
}


