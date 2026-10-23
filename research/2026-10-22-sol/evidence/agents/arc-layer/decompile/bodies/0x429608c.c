// ==== 0x429608c  FUN_0429608c  (376 bytes)

long FUN_0429608c(long param_1,ulong param_2,undefined8 param_3,undefined8 *param_4)

{
  int iVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  pthread_mutexattr_t local_48;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar2 = (undefined8 *)FUN_023ec3c0(param_1,param_3);
  *puVar2 = &PTR_FUN_0da7a7f8;
  if ((param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_2);
  }
  *(undefined8 *)(param_1 + 0xb8) = 0;
  *(undefined8 *)(param_1 + 0xb0) = 0;
  *(undefined8 *)(param_1 + 200) = 0;
  *(undefined8 *)(param_1 + 0xc0) = 0;
  *(ulong *)(param_1 + 0xa0) = param_2;
  *(undefined ***)(param_1 + 0xa8) = &PTR_FUN_0dce4600;
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
  _pthread_mutex_init((pthread_mutex_t *)(param_1 + 0xd0),&local_48);
  _pthread_mutexattr_destroy(&local_48);
  *(undefined8 *)(param_1 + 0x110) = 0;
  *(undefined ***)(param_1 + 0xa8) = &PTR_LAB_0da827d8;
  if (*(char *)((long)param_4 + 0x17) < '\0') {
    FUN_00060a60(param_1 + 0x118,*param_4,param_4[1]);
  }
  else {
    uVar4 = param_4[1];
    uVar3 = *param_4;
    *(undefined8 *)(param_1 + 0x128) = param_4[2];
    *(undefined8 *)(param_1 + 0x120) = uVar4;
    *(undefined8 *)(param_1 + 0x118) = uVar3;
  }
  local_48.__sig = FUN_00008790(0x30);
  *(int *)local_48.__sig = 1;
  *(code **)(local_48.__sig + 8) = FUN_020769c0;
  *(code **)(local_48.__sig + 0x10) = FUN_039a3680;
  *(code **)(local_48.__sig + 0x18) = FUN_00b77080;
  *(undefined **)(local_48.__sig + 0x20) = &DAT_04296204;
  FUN_04558e68(&local_48);
  if ((int *)local_48.__sig != (int *)0x0) {
    LOAcquire();
    iVar1 = *(int *)local_48.__sig;
    *(int *)local_48.__sig = iVar1 + -1;
    LORelease();
    if (iVar1 == 1) {
      (**(code **)(local_48.__sig + 0x10))();
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return param_1;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


