// ==== 0xf71cc  FUN_000f71cc  (600 bytes)

undefined8 FUN_000f71cc(undefined8 param_1,ulong param_2,undefined8 param_3)

{
  undefined8 ******ppppppuVar1;
  byte bVar2;
  code *pcVar3;
  int iVar4;
  pthread_t p_Var5;
  ulong uVar6;
  int *piVar7;
  int extraout_w8;
  int iVar8;
  long *plVar9;
  pthread_t p_Var10;
  long lVar11;
  long local_68;
  undefined8 *****local_60;
  long local_58;
  undefined7 local_50;
  char cStack_49;
  
  if (1 < (uint)param_2) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0xf73f0);
    (*pcVar3)();
  }
  if (((DAT_0e495370 & 1) == 0) && (iVar4 = thunk_FUN_0c44b610(&DAT_0e495370), iVar4 != 0)) {
    DAT_0e495358 = 0;
    DAT_0e495360 = 0;
    DAT_0e495368 = 0;
    DAT_0e495370 = 1;
    iVar4 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar2 = DAT_0e495371;
    if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to acquire mutex");
    }
    DAT_0e495371 = 1;
    iVar4 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar2 >> 2 & 1) != 0) &&
       (iVar4 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar4 != 0)) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to broadcast");
    }
  }
  plVar9 = (long *)(&DAT_0e495358)[param_2 & 0xffffffff];
  if (plVar9 != (long *)0x0) {
    if (*(code **)(*plVar9 + 0x48) == FUN_00073188) {
      p_Var10 = *(pthread_t *)(plVar9[3] + 8);
      lVar11 = *(long *)(plVar9[3] + 0x10);
      p_Var5 = _pthread_self();
      if (lVar11 != 0 && p_Var10 == p_Var5) {
                    /* WARNING: Does not return */
        pcVar3 = (code *)SoftwareBreakpoint(0,0xf73f4);
        (*pcVar3)();
      }
      if (p_Var10 == p_Var5) {
        return param_1;
      }
      if (*(long *)(plVar9[3] + 8) != 0) {
        piVar7 = (int *)(*(code *)PTR___tlv_bootstrap_0e379768)(*(undefined4 *)(plVar9[3] + 0x18));
        iVar4 = DAT_0e4a4064;
        iVar8 = *piVar7;
        if (*piVar7 == -1) {
          iVar8 = DAT_0e4a4064 + 1;
          *piVar7 = DAT_0e4a4064;
          DAT_0e4a4064 = iVar8;
          iVar8 = iVar4;
        }
        if (extraout_w8 != -1 && extraout_w8 == iVar8) {
          return param_1;
        }
      }
    }
    else {
      uVar6 = (**(code **)(*plVar9 + 0x48))(plVar9);
      if ((uVar6 & 1) != 0) {
        return param_1;
      }
    }
  }
  thunk_FUN_0837a430(&local_60,"../../content/browser/browser_thread_impl.cc",0x11c);
  thunk_FUN_08375df4(&local_68,"checker.CalledOnValidBrowserThread(thread_identifier)",param_3,
                     &local_60);
  FUN_0638ff4c(&local_60,param_2);
  ppppppuVar1 = (undefined8 ******)local_60;
  if (-1 < (long)cStack_49) {
    ppppppuVar1 = &local_60;
  }
  if (-1 < cStack_49) {
    local_58 = (long)cStack_49;
  }
  FUN_00040060(local_68 + 0x10,ppppppuVar1,local_58);
  if (cStack_49 < '\0') {
    FUN_00005810(local_60,CONCAT17(cStack_49,local_50) & 0x7fffffffffffffff);
  }
  thunk_FUN_0837612c(&local_68);
  return param_1;
}


