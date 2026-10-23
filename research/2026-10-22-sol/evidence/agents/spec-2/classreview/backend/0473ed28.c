// ==== 0x473ed28  FUN_0473ed28  (576 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0473ed28(undefined8 param_1,long *param_2,undefined8 param_3)

{
  byte bVar1;
  int iVar2;
  undefined8 uVar3;
  long lVar4;
  char *pcVar5;
  undefined8 *local_a0;
  undefined8 uStack_98;
  long local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  long local_78;
  byte local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  
  local_68 = 0;
  uStack_60 = 0;
  local_58 = 0;
  lVar4 = param_2[1] * 0x30;
  if (lVar4 != 0) {
    pcVar5 = (char *)(*param_2 + 0x17);
    do {
      if (*pcVar5 < '\0') {
        FUN_00054420(&local_a0,*(undefined8 *)(pcVar5 + -0x17),*(undefined8 *)(pcVar5 + -0xf));
      }
      else {
        uStack_98 = *(undefined8 *)(pcVar5 + -0xf);
        local_a0 = *(undefined8 **)(pcVar5 + -0x17);
        local_90 = *(long *)(pcVar5 + -7);
      }
      if (pcVar5[0x18] < '\0') {
        FUN_00054420(&local_88,*(undefined8 *)(pcVar5 + 1),*(undefined8 *)(pcVar5 + 9));
      }
      else {
        uStack_80 = *(undefined8 *)(pcVar5 + 9);
        local_88 = *(undefined8 *)(pcVar5 + 1);
        local_78 = *(long *)(pcVar5 + 0x11);
      }
      local_70 = 1;
      FUN_07986938(&local_68,&local_a0);
      if (((local_70 & 1) != 0) && (local_78 < 0)) {
        FUN_00005810(local_88,local_78 << 1);
      }
      if (local_90 < 0) {
        FUN_00005810(local_a0,local_90 << 1);
      }
      pcVar5 = pcVar5 + 0x30;
      lVar4 = lVar4 + -0x30;
    } while (lVar4 != 0);
  }
  if (((DAT_0e47f1a8 & 1) == 0) && (iVar2 = FUN_0c44b610(&DAT_0e47f1a8), iVar2 != 0)) {
    DAT_0e47f1a0 = UNUserNotificationCenter::currentNotificationCenter
                             ((ID)&_OBJC_CLASS___UNUserNotificationCenter,
                              "currentNotificationCenter");
    uRam000000000e47f188 = 0;
    _DAT_0e47f180 = 0;
    uRam000000000e47f198 = 0;
    _DAT_0e47f190 = 0;
    uRam000000000e47f178 = 0;
    _DAT_0e47f170 = 0;
    DAT_0e47f1a8 = 1;
    iVar2 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar1 = DAT_0e47f1a9;
    if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_0c4495ac("%s failed to acquire mutex");
    }
    DAT_0e47f1a9 = 1;
    iVar2 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar1 >> 2 & 1) != 0) &&
       (iVar2 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar2 != 0)) {
                    /* WARNING: Subroutine does not return */
      FUN_0c4495ac("%s failed to broadcast");
    }
  }
  uVar3 = FUN_07983e68(&DAT_0e47f170,param_1,&local_68,param_3);
  local_a0 = &local_68;
  FUN_0798018c(&local_a0);
  return uVar3;
}


