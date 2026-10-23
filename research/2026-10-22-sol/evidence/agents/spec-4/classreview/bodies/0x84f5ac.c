// ==== 0x84f5ac  FUN_0084f5ac  (380 bytes)

undefined8 * FUN_0084f5ac(void)

{
  byte bVar1;
  int iVar2;
  undefined8 uVar3;
  
  if (((DAT_0e49a540 & 1) == 0) && (iVar2 = thunk_FUN_0c44b610(&DAT_0e49a540), iVar2 != 0)) {
    uVar3 = FUN_00fbcec8();
    FUN_00a6f270(&DAT_0e49a508,"ExtensionSystem",uVar3,0);
    DAT_0e49a528 = 0;
    DAT_0e49a530 = 0;
    DAT_0e49a538 = 0;
    DAT_0e49a508 = &PTR_LAB_0d9985f0;
    if (((DAT_0e49a500 & 1) == 0) && (iVar2 = thunk_FUN_0c44b610(&DAT_0e49a500), iVar2 != 0)) {
      FUN_017a680c(&DAT_0e49a4b0);
      thunk_FUN_0c44b760(&DAT_0e49a500);
    }
    if (DAT_0e49a4c8 == DAT_0e49a520) {
      FUN_005ff508(DAT_0e49a510 + 8,&DAT_0e49a4b0,&DAT_0e49a508);
    }
    DAT_0e49a540 = 1;
    iVar2 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar1 = DAT_0e49a541;
    if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to acquire mutex");
    }
    DAT_0e49a541 = 1;
    iVar2 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar1 >> 2 & 1) != 0) &&
       (iVar2 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar2 != 0)) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to broadcast");
    }
  }
  return &DAT_0e49a508;
}


