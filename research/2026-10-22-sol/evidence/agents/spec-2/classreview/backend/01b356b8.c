// ==== 0x1b356b8  FUN_01b356b8  (444 bytes)

void FUN_01b356b8(undefined8 *param_1,undefined8 param_2)

{
  size_t sVar1;
  ulong uVar2;
  ulong uVar3;
  code *pcVar4;
  long lVar5;
  long lVar6;
  undefined8 *puVar7;
  undefined8 *puVar8;
  ulong local_50 [2];
  
  lVar5 = _CFStringGetLength();
  if (lVar5 == 0) {
LAB_01b35864:
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    return;
  }
  local_50[0] = 0;
  lVar6 = _CFStringGetBytes(param_2,0,lVar5,0x8000100,0,0,0,0,local_50);
  uVar3 = local_50[0];
  if ((lVar6 == 0) || ((long)local_50[0] < 1)) goto LAB_01b35864;
  sVar1 = local_50[0] + 1;
  if ((long)sVar1 < 0) {
                    /* WARNING: Subroutine does not return */
    FUN_040416c0();
  }
  puVar7 = (undefined8 *)FUN_00008790(sVar1);
  if ((ulong)-(long)puVar7 <= uVar3) goto LAB_01b357fc;
  _bzero(puVar7,sVar1);
  lVar5 = _CFStringGetBytes(param_2,0,lVar5,0x8000100,0,0,puVar7,uVar3,0);
  if (lVar5 == 0) {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    if (puVar7 == (undefined8 *)0x0) {
      return;
    }
  }
  else {
    if ((sVar1 <= uVar3) || (*(undefined1 *)((long)puVar7 + uVar3) = 0, puVar7 == (undefined8 *)0x0)
       ) goto LAB_01b357fc;
    if (0x7fffffffffffffee < uVar3) {
                    /* WARNING: Subroutine does not return */
      FUN_04b089c0();
    }
    if (uVar3 < 0x17) {
      *(char *)((long)param_1 + 0x17) = (char)uVar3;
      puVar8 = param_1;
    }
    else {
      uVar2 = (uVar3 & 0x7ffffffffffffff0) + 0x10;
      puVar8 = (undefined8 *)FUN_00008790(uVar2);
      *param_1 = puVar8;
      param_1[1] = uVar3;
      param_1[2] = uVar2 | 0x8000000000000000;
    }
    if (((undefined8 *)((long)puVar8 + uVar3) < puVar8) ||
       ((puVar8 <= puVar7 && (puVar7 < (undefined8 *)((long)puVar8 + uVar3))))) goto LAB_01b357fc;
    _memmove(puVar8,puVar7,uVar3);
    *(undefined1 *)((long)puVar8 + uVar3) = 0;
  }
  if (sVar1 <= (ulong)-(long)puVar7) {
    FUN_00005810(puVar7,sVar1);
    return;
  }
LAB_01b357fc:
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x1b35800);
  (*pcVar4)();
}


