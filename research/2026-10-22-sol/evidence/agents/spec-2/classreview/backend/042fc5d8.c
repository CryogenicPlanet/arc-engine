// ==== 0x42fc5d8  FUN_042fc5d8  (396 bytes)

void FUN_042fc5d8(long param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  size_t sVar5;
  char cVar6;
  code *pcVar7;
  int iVar8;
  long lVar9;
  long *plVar10;
  ulong uVar11;
  
  if (((*(long *)(param_1 + 0xf0) != 0) && (*(char *)(*(long *)(param_1 + 0xf0) + 4) == '\0')) &&
     (*(long *)(param_1 + 0xf8) != 0)) {
    if ((*(long *)(param_1 + 0xf0) == 0) || (*(char *)(*(long *)(param_1 + 0xf0) + 4) != '\0')) {
                    /* WARNING: Does not return */
      pcVar7 = (code *)SoftwareBreakpoint(0,0x42fc764);
      (*pcVar7)();
    }
    lVar9 = (**(code **)(**(long **)(param_1 + 0xf8) + 0x18))();
    for (plVar10 = *(long **)(lVar9 + 8); plVar10 != (long *)0x0; plVar10 = (long *)*plVar10) {
      while ((undefined **)plVar10[4] < (undefined **)0xdf542a1) {
        if ((undefined **)plVar10[4] == &PTR_s_ExtensionsContainer_kDataKeyImp_0df542a0) {
          if (plVar10 == (long *)(lVar9 + 8)) goto LAB_042fc674;
          uVar11 = plVar10[5];
          if ((uVar11 & 0xfffffffc00000000) == DAT_0e2f2208) {
            FUN_0000ffa8(uVar11);
          }
          goto LAB_042fc6a0;
        }
        plVar10 = (long *)plVar10[1];
        if (plVar10 == (long *)0x0) goto LAB_042fc674;
      }
    }
LAB_042fc674:
    uVar11 = 0;
LAB_042fc6a0:
    if ((uVar11 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4(uVar11);
    }
    if (uVar11 != 0) {
      plVar10 = *(long **)(uVar11 + 0xb0);
      if (plVar10 != (long *)0x0) {
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
            cVar6 = *(char *)((long)plVar10 + 0x37);
            plVar3 = (long *)plVar10[4];
            if (-1 < (long)cVar6) {
              plVar3 = plVar10 + 4;
            }
            uVar4 = plVar10[5];
            if (-1 < cVar6) {
              uVar4 = (long)cVar6;
            }
            sVar5 = uVar4;
            if (uVar2 <= uVar4) {
              sVar5 = uVar2;
            }
            iVar8 = _memcmp(puVar1,plVar3,sVar5);
            if (iVar8 == 0) break;
            if (-1 < iVar8) goto LAB_042fc6e4;
LAB_042fc724:
            plVar10 = (long *)*plVar10;
            if (plVar10 == (long *)0x0) {
              return;
            }
          }
          if (uVar2 == uVar4) {
            if (plVar10 == (long *)(uVar11 + 0xb0)) {
              return;
            }
            if (plVar10[7] == 0) {
              return;
            }
            FUN_07d1d550(plVar10[7],2);
            return;
          }
          if (uVar2 < uVar4) goto LAB_042fc724;
LAB_042fc6e4:
          plVar10 = (long *)plVar10[1];
        } while (plVar10 != (long *)0x0);
      }
    }
  }
  return;
}


