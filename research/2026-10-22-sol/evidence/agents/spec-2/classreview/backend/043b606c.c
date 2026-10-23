// ==== 0x43b606c  FUN_043b606c  (288 bytes)

void FUN_043b606c(long param_1,undefined8 *param_2,ulong param_3)

{
  undefined8 *puVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  size_t sVar5;
  char cVar6;
  int iVar7;
  long lVar8;
  long *plVar9;
  
  lVar8 = *(long *)(param_1 + 0x68);
  if ((lVar8 != 0) && (param_3 < *(ulong *)(lVar8 + 200))) {
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
          if (-1 < iVar7) goto LAB_043b60f0;
LAB_043b6130:
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
          iVar7 = FUN_07cd9ea0(lVar8,param_2);
          if (iVar7 == 0) {
            return;
          }
          FUN_07cd9f40(*(undefined8 *)(param_1 + 0x68),param_2,param_3);
          return;
        }
        if (uVar2 < uVar4) goto LAB_043b6130;
LAB_043b60f0:
        plVar9 = (long *)plVar9[1];
      } while (plVar9 != (long *)0x0);
    }
  }
  return;
}


