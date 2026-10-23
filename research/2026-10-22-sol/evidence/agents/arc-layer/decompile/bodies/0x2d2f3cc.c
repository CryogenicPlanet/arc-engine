// ==== 0x2d2f3cc  FUN_02d2f3cc  (1420 bytes)

/* WARNING: Removing unreachable block (ram,0x02d2f6e0) */

undefined8 FUN_02d2f3cc(ulong *param_1,undefined8 param_2)

{
  byte bVar1;
  code *pcVar2;
  int iVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  long extraout_x0;
  ulong uVar6;
  undefined8 uVar7;
  undefined1 *puVar8;
  long lVar9;
  ulong uVar10;
  ulong local_d0;
  undefined4 local_c8;
  ulong local_c0;
  int *piStack_b8;
  int *local_b0;
  ulong local_a8;
  ulong uStack_a0;
  ulong local_98;
  undefined4 local_90;
  ulong local_88;
  int *piStack_80;
  int *local_78;
  ulong local_70;
  ulong uStack_68;
  sigset_t local_5c;
  __sigaction_u local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  
  FUN_03b4e0ec();
  if (DAT_0e3b8850 == '\0') {
    DAT_0e3b8850 = '\x01';
    LOAcquire();
    DAT_0e553138 = FUN_083b64b4;
    LORelease();
    DAT_0e4a9110 = 1;
    FUN_027b0cc0();
    if (PTR__cxx_destruct_0e2b8240 == PTR__cxx_destruct_0e2b8248) {
      PTR__cxx_destruct_0e2b8240 = FUN_0837a450;
    }
    if (DAT_0e4a2fd8 == (undefined8 *)0x0) {
      uVar6 = param_1[1];
      uVar10 = param_1[2];
      puVar4 = (undefined8 *)FUN_00008790(0x38);
      puVar5 = (undefined8 *)FUN_00008790(0x18);
      *puVar4 = puVar5;
      puVar4[2] = 1;
      if (puVar5 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x2d2f8c8);
        (*pcVar2)();
      }
      *puVar5 = 0;
      puVar5[1] = 0;
      puVar5[2] = 0;
      puVar4[1] = 1;
      puVar4[4] = 0;
      puVar4[3] = puVar4 + 4;
      puVar4[5] = 0;
      puVar4[6] = 1;
      DAT_0e4a2fd8 = puVar4;
      FUN_01b2da94(puVar4,(int)uVar6,uVar10);
    }
    FUN_02744728(0);
    _setlocale(0,"");
    _setlocale(4,"C");
    _signal(0xd);
    if (extraout_x0 == -1) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x2d2f8dc);
      (*pcVar2)();
    }
    uVar6 = FUN_0027ec30(DAT_0e4a2fd8,"disable-in-process-stack-traces");
    if ((uVar6 & 1) == 0) {
      local_5c = 0;
      iVar3 = _sigprocmask(3,&local_5c,(sigset_t *)0x0);
      if (iVar3 != 0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x2d2f8e8);
        (*pcVar2)();
      }
      local_58.__sa_handler = (_func_333 *)0x0;
      uStack_50 = 0;
      iVar3 = _sigaction(1,(sigaction *)&local_58,(sigaction *)0x0);
      if ((((((iVar3 != 0) ||
             (iVar3 = _sigaction(2,(sigaction *)&local_58,(sigaction *)0x0), iVar3 != 0)) ||
            (iVar3 = _sigaction(3,(sigaction *)&local_58,(sigaction *)0x0), iVar3 != 0)) ||
           ((iVar3 = _sigaction(4,(sigaction *)&local_58,(sigaction *)0x0), iVar3 != 0 ||
            (iVar3 = _sigaction(6,(sigaction *)&local_58,(sigaction *)0x0), iVar3 != 0)))) ||
          ((iVar3 = _sigaction(8,(sigaction *)&local_58,(sigaction *)0x0), iVar3 != 0 ||
           ((iVar3 = _sigaction(0xb,(sigaction *)&local_58,(sigaction *)0x0), iVar3 != 0 ||
            (iVar3 = _sigaction(0xe,(sigaction *)&local_58,(sigaction *)0x0), iVar3 != 0)))))) ||
         ((iVar3 = _sigaction(0xf,(sigaction *)&local_58,(sigaction *)0x0), iVar3 != 0 ||
          (((iVar3 = _sigaction(0x14,(sigaction *)&local_58,(sigaction *)0x0), iVar3 != 0 ||
            (iVar3 = _sigaction(10,(sigaction *)&local_58,(sigaction *)0x0), iVar3 != 0)) ||
           (iVar3 = _sigaction(5,(sigaction *)&local_58,(sigaction *)0x0), iVar3 != 0)))))) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x2d2f8bc);
        (*pcVar2)();
      }
    }
    FUN_02c38d0c();
    FUN_026802fc(&LAB_03b7e054,3000,0xbbc);
    local_d0 = *param_1;
    *param_1 = 0;
    local_c8 = (undefined4)param_1[1];
    piStack_b8 = (int *)param_1[3];
    local_c0 = param_1[2];
    param_1[2] = 0;
    param_1[3] = 0;
    local_b0 = (int *)param_1[4];
    param_1[4] = 0;
    uStack_a0 = param_1[6];
    local_a8 = param_1[5];
    uVar7 = FUN_02698c44(param_2,&local_d0);
    if (local_b0 != (int *)0x0) {
      LOAcquire();
      iVar3 = *local_b0;
      *local_b0 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        (**(code **)(local_b0 + 4))();
      }
    }
    if (piStack_b8 != (int *)0x0) {
      LOAcquire();
      iVar3 = *piStack_b8;
      *piStack_b8 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        (**(code **)(piStack_b8 + 4))();
      }
    }
    if ((local_c0 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    local_c0 = 0;
    if ((local_d0 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    local_d0 = 0;
    if (-1 < (int)uVar7) {
      return uVar7;
    }
    if (((DAT_0e3b8890 & 1) == 0) && (iVar3 = thunk_FUN_0c44b610(&DAT_0e3b8890), iVar3 != 0)) {
      DAT_0e3b8858 = &PTR_FUN_0da063e8;
      DAT_0e3b8860 = 0;
      DAT_0e3b8888 = '\0';
      FUN_01b2c2ec(&PTR_PTR_0df6ee28);
      if (DAT_0e2b82a4 != 0) {
        puVar8 = &DAT_0e3b8860;
        if (DAT_0e3b8888 != '\0') {
          puVar8 = (undefined1 *)FUN_01545464(&DAT_0e3b8860);
          puVar8[0x28] = 0;
        }
        lVar9 = FUN_00fd84bc(puVar8,"TracingEnabled",0xe,1,0);
        *(undefined1 *)(lVar9 + 0x28) = 1;
      }
      DAT_0e3b8890 = 1;
      iVar3 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
      bVar1 = DAT_0e3b8891;
      if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
        thunk_FUN_0c4495ac("%s failed to acquire mutex");
      }
      DAT_0e3b8891 = 1;
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
  }
  else {
    local_98 = *param_1;
    *param_1 = 0;
    local_90 = (undefined4)param_1[1];
    piStack_80 = (int *)param_1[3];
    local_88 = param_1[2];
    param_1[2] = 0;
    param_1[3] = 0;
    local_78 = (int *)param_1[4];
    param_1[4] = 0;
    uStack_68 = param_1[6];
    local_70 = param_1[5];
    FUN_07cc1d64(param_2,&local_98);
    if (local_78 != (int *)0x0) {
      LOAcquire();
      iVar3 = *local_78;
      *local_78 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        (**(code **)(local_78 + 4))();
      }
    }
    if (piStack_80 != (int *)0x0) {
      LOAcquire();
      iVar3 = *piStack_80;
      *piStack_80 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        (**(code **)(piStack_80 + 4))();
      }
    }
    if ((local_88 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    local_88 = 0;
    if ((local_98 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    local_98 = 0;
  }
  local_58.__sa_handler = (_func_333 *)0x0;
  uStack_50 = 0;
  local_48 = 0;
  FUN_00cad330(&local_58,DAT_0e4a2fd8,&DAT_0c6d7369,4);
  uVar7 = FUN_0243ef74(param_2);
  return uVar7;
}


