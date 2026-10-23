// ==== 0x882ba0  FUN_00882ba0  (284 bytes)

undefined8 FUN_00882ba0(long param_1)

{
  int *piVar1;
  int iVar2;
  code *pcVar3;
  long lVar4;
  undefined8 uVar5;
  long *plVar6;
  long lVar7;
  int *piVar8;
  int *piVar9;
  ulong uVar10;
  ulong uVar11;
  undefined8 local_28;
  
  plVar6 = *(long **)(param_1 + 0x8d8);
  lVar7 = plVar6[0x25];
  if (*(undefined1 **)(*plVar6 + 0x78) == &LAB_04150798) {
    if ((lVar7 == 0) || (*(char *)(lVar7 + 4) != '\0')) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0,0x882cbc);
      (*pcVar3)();
    }
    lVar7 = *(long *)(plVar6[0x26] + 0x40);
    if (lVar7 == 0) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0,0x882cc8);
      (*pcVar3)();
    }
  }
  else {
    if ((lVar7 == 0) || (*(char *)(lVar7 + 4) != '\0')) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0,0x882cd4);
      (*pcVar3)();
    }
    lVar7 = *(long *)(plVar6[0x26] + 0x40);
    if (lVar7 == 0) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0,0x882ca0);
      (*pcVar3)();
    }
  }
  local_28 = CONCAT44((int)plVar6[0xeb],*(undefined4 *)(lVar7 + 0x198));
  lVar7 = FUN_000805e0(param_1 + 8,&DAT_0ccc00f4);
  uVar5 = 0;
  if (lVar7 != 0) {
    lVar4 = FUN_00883910(&local_28);
    iVar2 = *(int *)(*(long *)(lVar4 + 0x140) + 0x10);
    piVar1 = *(int **)(lVar7 + 0x10);
    lVar4 = *(long *)(lVar7 + 0x18) << 4;
    piVar9 = piVar1;
    if (lVar4 != 0) {
      uVar10 = lVar4 >> 4;
      piVar8 = piVar1;
      do {
        uVar11 = uVar10 >> 1;
        piVar9 = piVar8 + uVar11 * 4 + 4;
        uVar10 = uVar10 + ~(uVar10 >> 1);
        if (iVar2 <= piVar8[uVar11 * 4]) {
          piVar9 = piVar8;
          uVar10 = uVar11;
        }
        piVar8 = piVar9;
      } while (uVar10 != 0);
    }
    if ((piVar9 == piVar1 + *(long *)(lVar7 + 0x18) * 4) || (iVar2 < *piVar9)) {
      uVar5 = 0;
    }
    else {
      uVar5 = *(undefined8 *)(piVar9 + 2);
    }
  }
  return uVar5;
}


