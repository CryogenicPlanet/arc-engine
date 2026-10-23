// ==== 0x42b7ea0  FUN_042b7ea0  (960 bytes)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_042b7ea0(undefined8 param_1,undefined8 *param_2)

{
  byte bVar1;
  byte bVar2;
  undefined8 *puVar3;
  int iVar4;
  pthread_mutexattr_t local_68;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  if (((DAT_0e3baaf0 & 1) == 0) && (iVar4 = thunk_FUN_0c44b610(&DAT_0e3baaf0), iVar4 != 0)) {
    uRam000000000e3baad8 = 0;
    _DAT_0e3baad0 = 0;
    uRam000000000e3baae8 = 0;
    _DAT_0e3baae0 = 0;
    uRam000000000e3baab8 = 0;
    _DAT_0e3baab0 = 0;
    uRam000000000e3baac8 = 0;
    _DAT_0e3baac0 = 0;
    local_68.__sig = 0;
    local_68.__opaque[0] = '\0';
    local_68.__opaque[1] = '\0';
    local_68.__opaque[2] = '\0';
    local_68.__opaque[3] = '\0';
    local_68.__opaque[4] = '\0';
    local_68.__opaque[5] = '\0';
    local_68.__opaque[6] = '\0';
    local_68.__opaque[7] = '\0';
    _pthread_mutexattr_init(&local_68);
    _pthread_mutexattr_setprotocol(&local_68,1);
    _pthread_mutex_init((pthread_mutex_t *)&DAT_0e3baab0,&local_68);
    _pthread_mutexattr_destroy(&local_68);
    DAT_0e3baaf0 = 1;
    iVar4 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar1 = DAT_0e3baaf1;
    if (iVar4 != 0) goto LAB_042b81e8;
    DAT_0e3baaf1 = 1;
    iVar4 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar1 >> 2 & 1) != 0) &&
       (iVar4 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar4 != 0))
    goto LAB_042b81d0;
  }
  iVar4 = _pthread_mutex_trylock((pthread_mutex_t *)&DAT_0e3baab0);
  if (iVar4 != 0) {
    FUN_002b39c8(&DAT_0e3baab0,&PTR_s_BaseLock_0df6cbd8);
  }
  if (((DAT_0e3bab18 & 1) == 0) && (iVar4 = thunk_FUN_0c44b610(&DAT_0e3bab18), iVar4 != 0)) {
    DAT_0e3bab08 = FUN_040490c0;
    DAT_0e3bab10 = &DAT_0c4b3500;
    DAT_0e3bab18 = 1;
    iVar4 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar1 = DAT_0e3bab19;
    if (iVar4 != 0) goto LAB_042b81e8;
    DAT_0e3bab19 = 1;
    iVar4 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar1 >> 2 & 1) != 0) &&
       (iVar4 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar4 != 0))
    goto LAB_042b81d0;
  }
  bVar1 = *(byte *)(DAT_0e3bab10 + 2);
  if (((DAT_0e3bab18 & 1) == 0) && (iVar4 = thunk_FUN_0c44b610(&DAT_0e3bab18), iVar4 != 0)) {
    DAT_0e3bab08 = FUN_040490c0;
    DAT_0e3bab10 = &DAT_0c4b3500;
    DAT_0e3bab18 = 1;
    iVar4 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar2 = DAT_0e3bab19;
    if (iVar4 != 0) goto LAB_042b81e8;
    DAT_0e3bab19 = 1;
    iVar4 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar2 >> 2 & 1) != 0) &&
       (iVar4 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar4 != 0))
    goto LAB_042b81d0;
  }
  puVar3 = DAT_0e3bab10;
  DAT_0e3bab08 = (code *)0x0;
  DAT_0e3bab10 = &DAT_0c4b3500;
  if ((code *)puVar3[1] != (code *)0x0) {
    (*(code *)puVar3[1])(DAT_0e3baaf8);
  }
  DAT_0e3bab00 = param_2[1];
  DAT_0e3baaf8 = *param_2;
  DAT_0e3bab10 = (undefined8 *)param_2[3];
  DAT_0e3bab08 = (code *)param_2[2];
  param_2[2] = 0;
  param_2[3] = &DAT_0c4b3500;
  if (((DAT_0e3bab18 & 1) == 0) && (iVar4 = thunk_FUN_0c44b610(&DAT_0e3bab18), iVar4 != 0)) {
    DAT_0e3bab08 = FUN_040490c0;
    DAT_0e3bab10 = &DAT_0c4b3500;
    DAT_0e3bab18 = 1;
    iVar4 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar2 = DAT_0e3bab19;
    if (iVar4 != 0) {
LAB_042b81e8:
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to acquire mutex");
    }
    DAT_0e3bab19 = 1;
    iVar4 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar2 >> 2 & 1) != 0) &&
       (iVar4 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar4 != 0)) {
LAB_042b81d0:
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to broadcast");
    }
  }
  bVar2 = *(byte *)(DAT_0e3bab10 + 2);
  iVar4 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3baab0);
  if ((bVar1 & (bVar2 ^ 1) & 1) == 0) {
    if (((bVar1 | bVar2 ^ 1) & 1) == 0) {
      DAT_0e4a2fe0 = (code *)0x0;
    }
  }
  else {
    DAT_0e4a2fe0 = FUN_042b8284;
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_58) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(iVar4);
  }
  return;
}


