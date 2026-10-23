// ==== 0x43210c8  FUN_043210c8  (324 bytes)

undefined8 * FUN_043210c8(void)

{
  byte bVar1;
  code *pcVar2;
  int iVar3;
  int *piVar4;
  
  if (((DAT_0e4a2c88 & 1) == 0) && (iVar3 = thunk_FUN_0c44b610(&DAT_0e4a2c88), iVar3 != 0)) {
    DAT_0e4a2b30 = &PTR_LAB_0da7e3c8;
    DAT_0e4a2b38 = &PTR_LAB_0da7e418;
    DAT_0e4a2b48 = 0;
    DAT_0e4a2b50 = 0;
    DAT_0e4a2b40 = 0;
    DAT_0e4a2b58 = 0;
    FUN_07040904(&DAT_0e4a2b60);
    FUN_07040904(&DAT_0e4a2be8);
    DAT_0e4a2c70 = 0;
    piVar4 = (int *)FUN_00008790(8);
    *piVar4 = 0;
    *(undefined1 *)(piVar4 + 1) = 0;
    if ((piVar4 != (int *)0x0) && (iVar3 = *piVar4, *piVar4 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x43211e0);
      (*pcVar2)();
    }
    DAT_0e4a2c80 = &DAT_0e4a2b30;
    DAT_0e4a2c88 = 1;
    DAT_0e4a2c78 = piVar4;
    iVar3 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar1 = DAT_0e4a2c89;
    if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to acquire mutex");
    }
    DAT_0e4a2c89 = 1;
    iVar3 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar1 >> 2 & 1) != 0) &&
       (iVar3 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar3 != 0)) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to broadcast");
    }
  }
  return &DAT_0e4a2b30;
}


