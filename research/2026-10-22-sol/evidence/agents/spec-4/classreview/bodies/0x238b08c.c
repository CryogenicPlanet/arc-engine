// ==== 0x238b08c  FUN_0238b08c  (576 bytes)

undefined8 * FUN_0238b08c(void)

{
  byte bVar1;
  code *pcVar2;
  int iVar3;
  ulong uVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  
  if (((DAT_0e4967d0 & 1) == 0) && (iVar3 = thunk_FUN_0c44b610(&DAT_0e4967d0), iVar3 != 0)) {
    uVar4 = FUN_00fbcec8();
    DAT_0e496798 = &PTR_FUN_0dace2c0;
    if ((uVar4 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8();
    }
    DAT_0e4967a8 = "ExtensionRegistrar";
    DAT_0e4967b0 = 0;
    DAT_0e4967a0 = uVar4;
    FUN_00080d44(uVar4,&DAT_0e496798);
    DAT_0e4967b8 = 0;
    DAT_0e4967c0 = 0;
    DAT_0e4967c8 = 0;
    DAT_0e496798 = &PTR_LAB_0d903990;
    if (((DAT_0e496810 & 1) == 0) && (iVar3 = thunk_FUN_0c44b610(&DAT_0e496810), iVar3 != 0)) {
      uVar6 = FUN_00fbcec8();
      puVar5 = (undefined8 *)FUN_024dd634(&DAT_0e4967d8,"ExtensionRegistry",uVar6);
      *puVar5 = &PTR_LAB_0d903a68;
      thunk_FUN_0c44b760(&DAT_0e496810);
    }
    if (DAT_0e4967f0 == DAT_0e4967b0) {
      FUN_005ff508(DAT_0e4967a0 + 8,&DAT_0e4967d8,&DAT_0e496798);
    }
    puVar5 = (undefined8 *)FUN_01145938();
    if ((undefined ***)puVar5 == &DAT_0e496798) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x238b288);
      (*pcVar2)();
    }
    if (*(int *)(puVar5 + 3) == DAT_0e4967b0) {
      FUN_005ff508(DAT_0e4967a0 + 8,puVar5,&DAT_0e496798);
    }
    puVar5 = (undefined8 *)FUN_01f75988();
    if ((undefined ***)puVar5 == &DAT_0e496798) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x238b294);
      (*pcVar2)();
    }
    if (*(int *)(puVar5 + 3) == DAT_0e4967b0) {
      FUN_005ff508(DAT_0e4967a0 + 8,puVar5,&DAT_0e496798);
    }
    DAT_0e4967d0 = 1;
    iVar3 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar1 = DAT_0e4967d1;
    if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to acquire mutex");
    }
    DAT_0e4967d1 = 1;
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
  return &DAT_0e496798;
}


