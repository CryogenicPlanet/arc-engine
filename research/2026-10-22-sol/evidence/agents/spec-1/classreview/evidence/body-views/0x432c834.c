// ==== 0x432c834  FUN_0432c834  (284 bytes)

bool FUN_0432c834(long param_1)

{
  ulong uVar1;
  long *plVar2;
  byte bVar3;
  bool bVar4;
  long *plVar5;
  int iVar6;
  int iVar7;
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  
  local_38 = 0;
  uStack_30 = 0;
  local_28 = 0;
  FUN_0be71158(&local_38,*(undefined8 *)(param_1 + 0x80));
  plVar5 = (long *)FUN_02ae8880(&local_38,"state",5);
  if (plVar5 == (long *)0x0) {
LAB_0432c894:
    bVar4 = false;
  }
  else {
    bVar3 = *(byte *)((long)plVar5 + 0x17);
    uVar1 = plVar5[1];
    if (-1 < (char)bVar3) {
      uVar1 = (ulong)bVar3;
    }
    if (uVar1 == 7) {
      plVar2 = (long *)*plVar5;
      if (-1 < (char)bVar3) {
        plVar2 = plVar5;
      }
      iVar6 = *(int *)((long)plVar2 + 3);
      bVar4 = (int)*plVar2 == 0x47474f4c;
      iVar7 = 0x474e4947;
    }
    else {
      if (uVar1 != 0xc) goto LAB_0432c894;
      plVar2 = (long *)*plVar5;
      if (-1 < (char)bVar3) {
        plVar2 = plVar5;
      }
      if (*plVar2 == 0x494c414954494e49 && (int)plVar2[1] == 0x474e495a) {
        bVar4 = true;
        goto LAB_0432c898;
      }
      iVar6 = (int)plVar2[1];
      bVar4 = *plVar2 == 0x474e495452415453;
      iVar7 = 0x474f4c5f;
    }
    bVar4 = bVar4 && iVar6 == iVar7;
  }
LAB_0432c898:
  FUN_005a58dc(&local_38);
  return bVar4;
}


