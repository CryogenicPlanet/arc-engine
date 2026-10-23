// ==== 0x4337e00  FUN_04337e00  (716 bytes)

void FUN_04337e00(float param_1,void *param_2,size_t param_3,undefined8 *param_4,uint param_5,
                 uint param_6,long param_7)

{
  undefined8 *puVar1;
  uint uVar2;
  uint uVar3;
  long lVar4;
  int iVar5;
  undefined8 *puVar6;
  void *pvVar7;
  undefined8 *puVar8;
  long *plVar9;
  undefined4 *puVar10;
  long lVar11;
  code *pcVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  undefined1 auStack_a0 [32];
  undefined **local_80 [2];
  long *local_70;
  char local_68;
  
  if (((param_2 == (void *)0x0) || (param_3 == 0)) || (param_1 <= 0.0)) {
    local_80[0] = (undefined **)((ulong)local_80[0] & 0xffffffffffffff00);
    local_68 = '\0';
    (*(code *)param_4[2])(param_4,local_80);
    if (local_68 == '\0') {
      return;
    }
    local_80[0] = &PTR_FUN_0da7eeb0;
    if (local_70 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar4 = local_70[1];
    local_70[1] = lVar4 + -1;
    LORelease();
    if (lVar4 != 0) {
      return;
    }
    (**(code **)(*local_70 + 0x10))(local_70);
    if (local_70[2] != 0) {
      LOAcquire();
      lVar4 = local_70[2];
      local_70[2] = lVar4 + -1;
      LORelease();
      if (lVar4 != 0) {
        return;
      }
    }
    pcVar12 = *(code **)(*local_70 + 0x20);
    if (((pcVar12 == FUN_00f01bc0) || (pcVar12 == FUN_00f01bc0)) || (pcVar12 == FUN_013efb40)) {
      FUN_00005810(local_70,0x20);
      return;
    }
LAB_04337fd8:
    (*pcVar12)(local_70);
    return;
  }
  iVar5 = FUN_00122014(param_2,param_3,1);
  if (iVar5 != 0) {
                    /* WARNING: Does not return */
    pcVar12 = (code *)SoftwareBreakpoint(0,0x43380cc);
    (*pcVar12)();
  }
  uVar2 = 0x7fffffff;
  if (param_6 != 0 || param_5 != 0) {
    uVar2 = param_6 & ((int)param_6 >> 0x1f ^ 0xffffffffU);
  }
  uVar3 = 0x7fffffff;
  if (param_6 != 0 || param_5 != 0) {
    uVar3 = param_5 & ((int)param_5 >> 0x1f ^ 0xffffffffU);
  }
  lVar4 = 0x8000000;
  if (param_7 != 0) {
    lVar4 = param_7;
  }
  puVar6 = (undefined8 *)FUN_00008790(0x30);
  puVar6[1] = 0;
  puVar6[2] = 0;
  *puVar6 = &PTR_viewWillStartLiveResize_0dd26860;
  if (puVar6 + 3 != (undefined8 *)0x0) {
    if ((long)param_3 < 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    pvVar7 = (void *)FUN_00008790(param_3);
    puVar6[3] = pvVar7;
    puVar6[5] = param_3;
    _memcpy(pvVar7,param_2,param_3);
    puVar6[4] = param_3;
    puVar8 = (undefined8 *)FUN_00008790(0x38);
    puVar8[1] = 0;
    puVar8[2] = 0;
    *puVar8 = &PTR_viewWillStartLiveResize_0da7ef28;
    puVar1 = puVar8 + 3;
    if (puVar1 != (undefined8 *)0x0) {
      uVar13 = *param_4;
      uVar15 = param_4[3];
      uVar14 = param_4[2];
      puVar8[4] = param_4[1];
      *puVar1 = uVar13;
      lVar11 = param_4[3];
      puVar8[6] = uVar15;
      puVar8[5] = uVar14;
      if (*(long *)(lVar11 + 8) != 0) {
        param_4[2] = 0;
        param_4[3] = &DAT_0c4b3500;
      }
      plVar9 = (long *)FUN_0049fafc(&DAT_0c9d705c);
      FUN_00029e40(auStack_a0,"Decode","../../arc/browser/public/image.cc",0x18b);
      puVar10 = (undefined4 *)FUN_00008790(0x60);
      *puVar10 = 1;
      *(code **)(puVar10 + 2) = FUN_04339084;
      *(code **)(puVar10 + 4) = FUN_0433a950;
      *(code **)(puVar10 + 6) = FUN_00b77080;
      *(undefined8 **)(puVar10 + 10) = puVar6 + 3;
      *(undefined8 **)(puVar10 + 0xc) = puVar6;
      *(undefined8 **)(puVar10 + 0xe) = puVar1;
      *(undefined8 **)(puVar10 + 0x10) = puVar8;
      puVar10[0x12] = param_1;
      *(ulong *)(puVar10 + 0x13) = CONCAT44(uVar2,uVar3);
      *(long *)(puVar10 + 0x16) = lVar4;
      (**(code **)*plVar9)(plVar9,auStack_a0,puVar10,0);
      if (plVar9 == (long *)0x0) {
        return;
      }
      LOAcquire();
      iVar5 = (int)plVar9[1];
      *(int *)(plVar9 + 1) = iVar5 + -1;
      LORelease();
      if (iVar5 != 1) {
        return;
      }
      pcVar12 = *(code **)(*plVar9 + 0x18);
      local_70 = plVar9;
      if (pcVar12 == (code *)&LAB_044511c4) {
        pcVar12 = *(code **)(*plVar9 + 0x10);
      }
      goto LAB_04337fd8;
    }
  }
                    /* WARNING: Does not return */
  pcVar12 = (code *)SoftwareBreakpoint(1,0x43380c4);
  (*pcVar12)();
}


