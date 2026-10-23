// ==== 0x434e3c0  FUN_0434e3c0  (484 bytes)

void FUN_0434e3c0(undefined8 *param_1)

{
  ulong uVar1;
  byte bVar2;
  undefined8 *puVar3;
  bool bVar4;
  int iVar5;
  void *pvVar6;
  void *pvVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  long lVar10;
  size_t sVar11;
  ulong uVar12;
  
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  if (((DAT_0e3bae48 & 1) == 0) && (iVar5 = thunk_FUN_0c44b610(&DAT_0e3bae48), iVar5 != 0)) {
    FUN_0434d5d4();
    DAT_0e3bae48 = 1;
    iVar5 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar2 = DAT_0e3bae49;
    if (iVar5 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to acquire mutex");
    }
    DAT_0e3bae49 = 1;
    iVar5 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar5 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar2 >> 2 & 1) != 0) &&
       (iVar5 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar5 != 0)) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to broadcast");
    }
  }
  uVar1 = DAT_0e3bae40;
  uVar12 = param_1[2];
  if (uVar12 < DAT_0e3bae40) {
    if (0xaaaaaaaaaaaaaaa < DAT_0e3bae40) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    lVar10 = param_1[1];
    pvVar6 = (void *)FUN_00008790(DAT_0e3bae40 * 0x18);
    pvVar7 = (void *)*param_1;
    sVar11 = lVar10 * 0x18;
    if (sVar11 != 0) {
      _memcpy(pvVar6,pvVar7,sVar11);
    }
    *param_1 = pvVar6;
    param_1[1] = ((long)sVar11 >> 3) * -0x5555555555555555;
    param_1[2] = uVar1;
    if (pvVar7 != (void *)0x0) {
      FUN_00005810(pvVar7,uVar12 * 0x18);
    }
  }
  puVar8 = DAT_0e3bae30;
  if (DAT_0e3bae30 != &DAT_0e3bae38) {
    do {
      FUN_00ad9ee8(param_1,puVar8 + 4);
      puVar3 = (undefined8 *)puVar8[1];
      if ((undefined8 *)puVar8[1] == (undefined8 *)0x0) {
        do {
          puVar9 = (undefined8 *)puVar8[2];
          bVar4 = puVar8 != (undefined8 *)*puVar9;
          puVar8 = puVar9;
        } while (bVar4);
      }
      else {
        do {
          puVar9 = puVar3;
          puVar3 = (undefined8 *)*puVar9;
        } while ((undefined8 *)*puVar9 != (undefined8 *)0x0);
      }
      puVar8 = puVar9;
    } while (puVar9 != &DAT_0e3bae38);
  }
  return;
}


