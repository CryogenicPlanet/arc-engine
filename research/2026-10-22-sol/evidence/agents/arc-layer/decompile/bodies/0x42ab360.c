// ==== 0x42ab360  FUN_042ab360  (704 bytes)

void FUN_042ab360(long param_1)

{
  code *pcVar1;
  int iVar2;
  int *piVar3;
  long lVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  char *local_1a0;
  undefined8 local_198;
  undefined1 *puStack_190;
  undefined8 local_188;
  char local_180 [8];
  undefined7 local_178;
  char cStack_171;
  undefined1 auStack_170 [128];
  char *local_f0;
  undefined8 local_e8;
  ulong uStack_e0;
  undefined8 local_d8;
  char local_d0 [8];
  undefined7 local_c8;
  char cStack_c1;
  undefined1 auStack_c0 [128];
  
  puVar6 = *(undefined8 **)(param_1 + 0x30);
  *(undefined8 *)(param_1 + 0x30) = 0;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[3] = 0;
    if (puVar6[2] != 0) {
      *(undefined1 *)(puVar6[2] + 4) = 1;
      piVar3 = (int *)puVar6[2];
      if (piVar3 != (int *)0x0) {
        LOAcquire();
        iVar2 = *piVar3;
        *piVar3 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          FUN_00005810(piVar3,8);
        }
      }
    }
    piVar3 = (int *)puVar6[1];
    if (piVar3 != (int *)0x0) {
      LOAcquire();
      iVar2 = *piVar3;
      *piVar3 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        (**(code **)(piVar3 + 4))();
      }
    }
    piVar3 = (int *)*puVar6;
    if (piVar3 != (int *)0x0) {
      LOAcquire();
      iVar2 = *piVar3;
      *piVar3 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        (**(code **)(piVar3 + 4))();
      }
    }
    FUN_00005810(puVar6,0x20);
  }
  FUN_04739228();
  if (*(long *)(param_1 + 0x18) == 0) {
LAB_042ab3e4:
    FUN_02d2f2bc(*(undefined8 *)(param_1 + 0x10));
    lVar4 = *(long *)(param_1 + 0x10);
    *(undefined8 *)(param_1 + 0x10) = 0;
    if (lVar4 != 0) {
      uVar5 = FUN_02d2f978();
      FUN_00005810(uVar5,0x100);
    }
    return;
  }
  uVar5 = FUN_02bed438();
  cStack_c1 = '\b';
  if (local_d0 <= "shutdown" || "shutdown" < &local_d8) {
    local_d8 = 0x6e776f6474756873;
    local_d0[0] = '\0';
    local_f0 = (char *)FUN_00008790(0x30);
    uStack_e0 = 0x8000000000000030;
    local_e8 = 0x21;
    if ((local_f0 <= local_f0 + 0x21) &&
       (local_f0 + 0x21 <= "Shutting down browser main runner" ||
        "Shutting down browser main runner" < local_f0)) {
      builtin_strncpy(local_f0,"Shutting down browser main runner",0x22);
      FUN_08b903b4(auStack_c0,&local_d8,&local_f0);
      FUN_02bed570(uVar5,auStack_c0);
      FUN_08b90450(auStack_c0);
      if ((long)uStack_e0 < 0) {
        FUN_00005810(local_f0,uStack_e0 & 0x7fffffffffffffff);
      }
      if (cStack_c1 < '\0') {
        FUN_00005810(local_d8,CONCAT17(cStack_c1,local_c8) & 0x7fffffffffffffff);
      }
      FUN_0318b9bc(*(undefined8 *)(param_1 + 0x18));
      lVar4 = *(long *)(param_1 + 0x18);
      *(undefined8 *)(param_1 + 0x18) = 0;
      if (lVar4 != 0) {
        FUN_0318b544();
      }
      uVar5 = FUN_02bed438();
      cStack_171 = '\b';
      if (local_180 <= "shutdown" || "shutdown" < &local_188) {
        local_188 = 0x6e776f6474756873;
        local_180[0] = '\0';
        local_1a0 = (char *)FUN_00008790(0x20);
        puStack_190 = &DAT_8000000000000020;
        local_198 = 0x1d;
        if ((local_1a0 <= local_1a0 + 0x1d) &&
           (local_1a0 + 0x1d <= "Browser main runner shut down" ||
            "Browser main runner shut down" < local_1a0)) {
          builtin_strncpy(local_1a0,"Browser main runner shut down",0x1e);
          FUN_08b903b4(auStack_170,&local_188,&local_1a0);
          FUN_02bed570(uVar5,auStack_170);
          FUN_08b90450(auStack_170);
          if ((long)puStack_190 < 0) {
            FUN_00005810(local_1a0,(ulong)puStack_190 & 0x7fffffffffffffff);
          }
          if (cStack_171 < '\0') {
            FUN_00005810(local_188,CONCAT17(cStack_171,local_178) & 0x7fffffffffffffff);
          }
          goto LAB_042ab3e4;
        }
      }
    }
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x42ab620);
  (*pcVar1)();
}


