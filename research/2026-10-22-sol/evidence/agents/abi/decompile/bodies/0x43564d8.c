// ==== 0x43564d8  FUN_043564d8  (636 bytes)

void FUN_043564d8(undefined8 *param_1,long param_2,int param_3,undefined8 *param_4)

{
  undefined8 *puVar1;
  ulong *puVar2;
  long lVar3;
  char cVar4;
  ulong uVar5;
  code *pcVar6;
  undefined8 uVar7;
  long lVar8;
  char *pcVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  ulong uVar12;
  long lVar13;
  ulong *puVar14;
  ulong *puVar15;
  ulong uVar16;
  ulong uVar17;
  ulong uVar18;
  long *plVar19;
  ulong uVar20;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  ulong local_38;
  
  uVar12 = *(ulong *)(param_2 + 0x108);
  if ((uVar12 != 0) && (*(long *)(param_2 + 0x118) != 0)) {
    uVar16 = (ulong)param_3;
    uVar17 = uVar12 - 1;
    if ((uVar12 & uVar17) == 0) {
      uVar18 = uVar17 & uVar16;
    }
    else {
      uVar18 = uVar16;
      if (uVar12 <= uVar16) {
        uVar18 = 0;
        if (uVar12 != 0) {
          uVar18 = uVar16 / uVar12;
        }
        uVar18 = uVar16 - uVar18 * uVar12;
      }
    }
    plVar19 = *(long **)(*(long *)(param_2 + 0x100) + uVar18 * 8);
    if (plVar19 != (long *)0x0) {
      do {
        while( true ) {
          plVar19 = (long *)*plVar19;
          if (plVar19 == (long *)0x0) goto LAB_0435653c;
          uVar20 = plVar19[1];
          if (uVar20 != uVar16) break;
          if (*(int *)(plVar19 + 2) == param_3) {
            uVar12 = plVar19[3];
            if ((*(long *)(uVar12 + 0x28) == 0) || (*(char *)(uVar12 + 0x30) != '\0')) {
              pcVar9 = "DevTools connection closed";
              uVar7 = 0x39;
              uVar10 = 0x1a;
              uVar11 = 0x37;
              goto LAB_04356558;
            }
            cVar4 = *(char *)((long)param_4 + 0x17);
            if (((long)cVar4 < 0) && (0x800000 < (ulong)param_4[1])) {
              pcVar9 = "DevTools message exceeds 8 MiB";
              uVar7 = 0x21;
              uVar10 = 0x1e;
              uVar11 = 0x3a;
              goto LAB_04356558;
            }
            uStack_48 = 0;
            local_50 = 0;
            local_38 = 0;
            uStack_40 = 0;
            uVar16 = param_4[1];
            puVar1 = (undefined8 *)*param_4;
            if (-1 < cVar4) {
              uVar16 = (long)cVar4;
              puVar1 = param_4;
            }
            FUN_0234775c(&local_50,puVar1,uVar16,0,200);
            if (((char)local_38 != '\0') &&
               (uVar16 = FUN_01443580(&local_50,&DAT_0c9d7137,2), (uVar16 >> 0x20 & 1) != 0)) {
              if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
                pcVar6 = (code *)SoftwareBreakpoint(1,0x435674c);
                (*pcVar6)();
              }
              lVar8 = FUN_02ae8880(&local_50,"method",6);
              if (lVar8 != 0) {
                cVar4 = *(char *)((long)param_4 + 0x17);
                puVar1 = (undefined8 *)*param_4;
                if (-1 < (long)cVar4) {
                  puVar1 = param_4;
                }
                lVar8 = param_4[1];
                if (-1 < cVar4) {
                  lVar8 = (long)cVar4;
                }
                puVar2 = *(ulong **)(*(long *)(uVar12 + 0x28) + 0x40);
                lVar3 = *(long *)(*(long *)(uVar12 + 0x28) + 0x48);
                lVar13 = lVar3 << 4;
                puVar15 = puVar2;
                if (lVar13 != 0) {
                  uVar16 = lVar13 >> 4;
                  puVar14 = puVar2;
                  do {
                    uVar17 = uVar16 >> 1;
                    puVar15 = puVar14 + uVar17 * 2 + 2;
                    uVar16 = uVar16 + ~(uVar16 >> 1);
                    if (uVar12 <= puVar14[uVar17 * 2]) {
                      puVar15 = puVar14;
                      uVar16 = uVar17;
                    }
                    puVar14 = puVar15;
                  } while (uVar16 != 0);
                }
                if (((puVar15 != puVar2 + lVar3 * 2) && (*puVar15 <= uVar12)) && (puVar15[1] != 0))
                {
                  FUN_01fe9cf0(puVar15[1],puVar1,lVar8);
                }
                *param_1 = 1;
LAB_043566cc:
                if ((char)local_38 == '\0') {
                  return;
                }
                FUN_005a58dc(&local_50);
                return;
              }
            }
            uVar7 = FUN_04b37104(0xd,"Invalid CDP command envelope",0x1c,0x3e,
                                 "../../arc/browser/web_contents/devtools_impl.cc");
            *param_1 = uVar7;
            goto LAB_043566cc;
          }
        }
        if ((uVar12 & uVar17) == 0) {
          uVar20 = uVar20 & uVar17;
        }
        else if (uVar12 <= uVar20) {
          uVar5 = 0;
          if (uVar12 != 0) {
            uVar5 = uVar20 / uVar12;
          }
          uVar20 = uVar20 - uVar5 * uVar12;
        }
      } while (uVar20 == uVar18);
    }
  }
LAB_0435653c:
  pcVar9 = "Unknown DevTools connection";
  uVar7 = 0x15;
  uVar10 = 0x1b;
  uVar11 = 0x94;
LAB_04356558:
  uVar7 = FUN_04b37104(uVar7,pcVar9,uVar10,uVar11,"../../arc/browser/web_contents/devtools_impl.cc")
  ;
  *param_1 = uVar7;
  return;
}


