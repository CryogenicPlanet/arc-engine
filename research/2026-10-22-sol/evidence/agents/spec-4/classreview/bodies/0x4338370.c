// ==== 0x4338370  FUN_04338370  (592 bytes)

undefined8 FUN_04338370(long param_1,long param_2)

{
  byte bVar1;
  int iVar2;
  undefined8 uVar3;
  long lVar4;
  ulong uVar5;
  long *plVar6;
  long *plVar7;
  long lVar8;
  int *local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  int *local_70;
  undefined8 uStack_68;
  int *local_60;
  undefined8 local_58;
  undefined8 uStack_50;
  
  lVar8 = *(long *)(param_1 + 8);
  if (*(int *)(lVar8 + 0x1c0) == 0) {
    bVar1 = *(long *)(lVar8 + 0x1d0) == 0;
  }
  else {
    bVar1 = *(byte *)(lVar8 + 0x1b8) ^ 1;
  }
  uVar3 = 0;
  if ((param_2 != 0) && ((bVar1 & 1) == 0)) {
    local_78 = *(undefined8 *)(lVar8 + 0x1c4);
    iVar2 = (int)local_78;
    local_88 = (int *)0x0;
    uStack_80 = 0x200000006;
    lVar4 = FUN_022f4af4(lVar8);
    local_60 = (int *)0x0;
    if (*(long *)(lVar4 + 8) == 0) {
      uVar5 = 0;
    }
    else {
      uStack_68 = *(undefined8 *)(lVar4 + 0x10);
      local_70 = *(int **)(lVar4 + 8);
      if ((&local_60 != (int **)(lVar4 + 0x18)) &&
         (local_60 = *(int **)(lVar4 + 0x18), local_60 != (int *)0x0)) {
        *local_60 = *local_60 + 1;
      }
      uStack_50 = *(undefined8 *)(lVar4 + 0x28);
      local_58 = *(undefined8 *)(lVar4 + 0x20);
      uVar5 = FUN_005f0180(&local_70,&local_88,param_2,(long)(iVar2 << 2),0,0);
      if (local_60 != (int *)0x0) {
        LOAcquire();
        iVar2 = *local_60;
        *local_60 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          FUN_00005810(local_60,0x90);
        }
      }
    }
    if (local_88 != (int *)0x0) {
      LOAcquire();
      iVar2 = *local_88;
      *local_88 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        FUN_00005810(local_88,0x90);
      }
    }
    if ((uVar5 & 1) == 0) {
      local_60 = *(int **)(lVar8 + 0x1c4);
      local_70 = (int *)0x0;
      uStack_68 = 0x200000006;
      plVar6 = (long *)FUN_0089d34c(&local_70,param_2,(long)((int)local_60 << 2),0);
      if (local_70 != (int *)0x0) {
        LOAcquire();
        iVar2 = *local_70;
        *local_70 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          FUN_00005810(local_70,0x90);
        }
      }
      if (plVar6 == (long *)0x0) {
        return 0;
      }
      plVar7 = (long *)FUN_022f4af4(lVar8);
      if (*plVar7 == 0) {
        plVar7 = (long *)0x0;
      }
      else {
        plVar7 = (long *)FUN_01d79300(plVar7,0,0);
      }
      local_70 = (int *)0x0;
      uStack_68 = 0;
      local_60 = (int *)0x0;
      FUN_0230fca8(0,0,plVar6,plVar7,&local_70,0);
      if (plVar7 != (long *)0x0) {
        LOAcquire();
        iVar2 = (int)plVar7[1];
        *(int *)(plVar7 + 1) = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          (**(code **)(*plVar7 + 0x10))(plVar7);
        }
      }
      (**(code **)(*plVar6 + 0x48))(plVar6,0);
      (**(code **)(*plVar6 + 8))(plVar6);
    }
    uVar3 = 1;
  }
  return uVar3;
}


