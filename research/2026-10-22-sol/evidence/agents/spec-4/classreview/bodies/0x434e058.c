// ==== 0x434e058  FUN_0434e058  (864 bytes)

void FUN_0434e058(long param_1,long *param_2)

{
  undefined8 *puVar1;
  ulong uVar2;
  size_t sVar3;
  undefined8 ****ppppuVar4;
  ulong uVar5;
  char cVar6;
  byte bVar7;
  code *pcVar8;
  int iVar9;
  long lVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  long lVar13;
  long lVar14;
  undefined8 *puVar15;
  undefined8 ***local_1a0;
  ulong uStack_198;
  undefined8 local_190;
  undefined8 ***local_180;
  long lStack_178;
  undefined8 local_170;
  
  lVar10 = FUN_000805e0(*(long *)(param_1 + 8) + 8,&DAT_0c9bfecc);
  if (lVar10 == 0 || lVar10 == 0x40) {
    uVar12 = 0;
  }
  else {
    uVar12 = *(undefined8 *)(lVar10 + 0x20);
  }
  lVar10 = FUN_000805e0(*(long *)(param_1 + 8) + 8,&DAT_0c9bfecc);
  if (lVar10 == 0 || lVar10 == 0x40) {
    lVar10 = 0;
  }
  else {
    lVar10 = *(long *)(lVar10 + 0x20);
  }
  lVar13 = (long)*(char *)(lVar10 + 0x67);
  if (lVar13 < 0) {
    lVar14 = *(long *)(lVar10 + 0x50);
    lVar13 = *(long *)(lVar10 + 0x58);
  }
  else {
    lVar14 = lVar10 + 0x50;
  }
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(&local_1a0,*param_2,param_2[1]);
  }
  else {
    uStack_198 = param_2[1];
    local_1a0 = (undefined8 ***)*param_2;
    local_190 = param_2[2];
  }
  if (((DAT_0e3bae48 & 1) == 0) && (iVar9 = thunk_FUN_0c44b610(&DAT_0e3bae48), iVar9 != 0)) {
    FUN_0434d5d4();
    DAT_0e3bae48 = 1;
    iVar9 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar7 = DAT_0e3bae49;
    if (iVar9 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to acquire mutex");
    }
    DAT_0e3bae49 = 1;
    iVar9 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar9 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar7 >> 2 & 1) != 0) &&
       (iVar9 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar9 != 0)) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to broadcast");
    }
  }
  if (DAT_0e3bae38 != (undefined8 *)0x0) {
    ppppuVar4 = (undefined8 ****)local_1a0;
    if (-1 < (long)local_190._7_1_) {
      ppppuVar4 = &local_1a0;
    }
    puVar15 = DAT_0e3bae38;
    uVar5 = uStack_198;
    if (-1 < (long)local_190) {
      uVar5 = (long)local_190._7_1_;
    }
    do {
      while( true ) {
        cVar6 = *(char *)((long)puVar15 + 0x37);
        puVar1 = (undefined8 *)puVar15[4];
        if (-1 < (long)cVar6) {
          puVar1 = puVar15 + 4;
        }
        uVar2 = puVar15[5];
        if (-1 < cVar6) {
          uVar2 = (long)cVar6;
        }
        sVar3 = uVar2;
        if (uVar5 <= uVar2) {
          sVar3 = uVar5;
        }
        iVar9 = _memcmp(ppppuVar4,puVar1,sVar3);
        if (iVar9 != 0) break;
        if (uVar5 == uVar2) {
          if ((undefined8 **)puVar15 != &DAT_0e3bae38) {
            if (*(char *)((long)puVar15 + 0x4f) < '\0') {
              FUN_00060a60(&local_180,puVar15[7],puVar15[8]);
            }
            else {
              lStack_178 = puVar15[8];
              local_180 = (undefined8 ***)puVar15[7];
              local_170 = puVar15[9];
            }
            goto LAB_0434e210;
          }
          goto LAB_0434e168;
        }
        if (uVar5 < uVar2) goto LAB_0434e2ac;
LAB_0434e160:
        puVar1 = puVar15 + 1;
        puVar15 = (undefined8 *)*puVar1;
        if ((undefined8 *)*puVar1 == (undefined8 *)0x0) goto LAB_0434e168;
      }
      if (-1 < iVar9) goto LAB_0434e160;
LAB_0434e2ac:
      puVar15 = (undefined8 *)*puVar15;
    } while (puVar15 != (undefined8 *)0x0);
  }
LAB_0434e168:
  if (DAT_0e4a3078 < 3) {
    FUN_00f195d8(&local_180,"../../arc/browser/translate_helper.cc",0x25,2);
    uVar11 = FUN_00040060(&local_170,"Missing language name [",0x17);
    ppppuVar4 = (undefined8 ****)local_1a0;
    if (-1 < (long)local_190._7_1_) {
      ppppuVar4 = &local_1a0;
    }
    uVar5 = uStack_198;
    if (-1 < (long)local_190) {
      uVar5 = (long)local_190._7_1_;
    }
    uVar11 = FUN_00040060(uVar11,ppppuVar4,uVar5);
    FUN_00040060(uVar11,"]",1);
    FUN_00f1950c(&local_180);
  }
  local_170 = CONCAT17(2,(undefined7)local_170);
  if ("en" < (char *)((ulong)&local_180 | 2) && &local_180 < (undefined8 ****)0xd025554) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(1,0x434e378);
    (*pcVar8)();
  }
  local_180 = (undefined8 ***)CONCAT53(local_180._3_5_,0x6e65);
LAB_0434e210:
  ppppuVar4 = (undefined8 ****)local_180;
  if (-1 < (long)local_170._7_1_) {
    ppppuVar4 = &local_180;
  }
  lVar10 = lStack_178;
  if (-1 < (long)local_170) {
    lVar10 = (long)local_170._7_1_;
  }
  FUN_09f2442c(uVar12,lVar14,lVar13,ppppuVar4,lVar10,1,5);
  if ((long)local_170 < 0) {
    FUN_00005810(local_180,local_170 & 0x7fffffffffffffff);
  }
  if ((long)local_190 < 0) {
    FUN_00005810(local_1a0,local_190 & 0x7fffffffffffffff);
  }
  return;
}


