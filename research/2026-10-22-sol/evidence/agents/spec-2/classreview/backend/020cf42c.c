// ==== 0x20cf42c  FUN_020cf42c  (428 bytes)

undefined1 FUN_020cf42c(int param_1)

{
  byte bVar1;
  code *pcVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  ulong uVar6;
  ulong uVar7;
  
  if (((DAT_0e4acbd0 & 1) == 0) && (iVar3 = thunk_FUN_0c44b610(&DAT_0e4acbd0), iVar3 != 0)) {
    FUN_044d8594(&DAT_0e4acb80);
    DAT_0e4acbd0 = 1;
    iVar3 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar1 = DAT_0e4acbd1;
    if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to acquire mutex");
    }
    DAT_0e4acbd1 = 1;
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
  piVar5 = DAT_0e4acb80;
  if (DAT_0e4acb88 * 0x28 != 0) {
    uVar6 = (DAT_0e4acb88 * 0x28 >> 3) * -0x3333333333333333;
    piVar4 = DAT_0e4acb80;
    do {
      uVar7 = uVar6 >> 1;
      piVar5 = piVar4 + uVar7 * 10 + 10;
      uVar6 = uVar6 + ~(uVar6 >> 1);
      if (param_1 <= piVar4[uVar7 * 10]) {
        piVar5 = piVar4;
        uVar6 = uVar7;
      }
      piVar4 = piVar5;
    } while (uVar6 != 0);
  }
  if ((piVar5 == DAT_0e4acb80 + DAT_0e4acb88 * 10) || (param_1 < *piVar5)) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(0,0x20cf588);
    (*pcVar2)();
  }
  if ((char)piVar5[8] == '\x01') {
    return (char)piVar5[2];
  }
  if ((char)piVar5[8] == -1) {
                    /* WARNING: Subroutine does not return */
    FUN_04b335b8("std::get: variant is valueless");
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b335b8("std::get: wrong alternative for variant");
}


