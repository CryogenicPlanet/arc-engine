// ==== 0x43b5ebc  FUN_043b5ebc  (248 bytes)

void FUN_043b5ebc(long param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  size_t sVar5;
  char cVar6;
  int iVar7;
  undefined8 uVar8;
  long *plVar9;
  undefined8 local_48;
  
  plVar9 = *(long **)(param_1 + 0xb0);
  if (plVar9 != (long *)0x0) {
    cVar6 = *(char *)((long)param_2 + 0x17);
    puVar1 = (void *)*param_2;
    if (-1 < (long)cVar6) {
      puVar1 = param_2;
    }
    uVar2 = param_2[1];
    if (-1 < cVar6) {
      uVar2 = (long)cVar6;
    }
    do {
      while( true ) {
        cVar6 = *(char *)((long)plVar9 + 0x37);
        plVar3 = (long *)plVar9[4];
        if (-1 < (long)cVar6) {
          plVar3 = plVar9 + 4;
        }
        uVar4 = plVar9[5];
        if (-1 < cVar6) {
          uVar4 = (long)cVar6;
        }
        sVar5 = uVar4;
        if (uVar2 <= uVar4) {
          sVar5 = uVar2;
        }
        iVar7 = _memcmp(puVar1,plVar3,sVar5);
        if (iVar7 == 0) break;
        if (-1 < iVar7) goto LAB_043b5f04;
LAB_043b5f44:
        plVar9 = (long *)*plVar9;
        if (plVar9 == (long *)0x0) {
          return;
        }
      }
      if (uVar2 == uVar4) {
        if (plVar9 == (long *)(param_1 + 0xb0)) {
          return;
        }
        if (plVar9[7] == 0) {
          return;
        }
        uVar8 = FUN_07d1d444(plVar9[7],0);
        local_48 = *(undefined8 *)(*(long *)(param_1 + 0x60) + 600);
        _os_retain();
        FUN_047343e0(uVar8,&local_48);
        _os_release(local_48);
        return;
      }
      if (uVar2 < uVar4) goto LAB_043b5f44;
LAB_043b5f04:
      plVar9 = (long *)plVar9[1];
    } while (plVar9 != (long *)0x0);
  }
  return;
}


