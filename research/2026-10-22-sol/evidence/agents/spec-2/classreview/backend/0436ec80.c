// ==== 0x436ec80  FUN_0436ec80  (324 bytes)

void FUN_0436ec80(undefined8 *param_1)

{
  undefined8 *puVar1;
  byte bVar2;
  int iVar3;
  
  if (((DAT_0e3bae78 & 1) == 0) && (iVar3 = thunk_FUN_0c44b610(&DAT_0e3bae78), iVar3 != 0)) {
    DAT_0e3bae68 = FUN_040490c0;
    DAT_0e3bae70 = &DAT_0c4b3500;
    DAT_0e3bae78 = 1;
    iVar3 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar2 = DAT_0e3bae79;
    if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to acquire mutex");
    }
    DAT_0e3bae79 = 1;
    iVar3 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar2 >> 2 & 1) != 0) &&
       (iVar3 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar3 != 0)) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to broadcast");
    }
  }
  puVar1 = DAT_0e3bae70;
  DAT_0e3bae68 = (code *)0x0;
  DAT_0e3bae70 = &DAT_0c4b3500;
  if ((code *)puVar1[1] != (code *)0x0) {
    (*(code *)puVar1[1])(DAT_0e3bae58);
  }
  uRam000000000e3bae60 = param_1[1];
  DAT_0e3bae58 = *param_1;
  DAT_0e3bae70 = (undefined8 *)param_1[3];
  DAT_0e3bae68 = (code *)param_1[2];
  param_1[2] = 0;
  param_1[3] = &DAT_0c4b3500;
  return;
}


