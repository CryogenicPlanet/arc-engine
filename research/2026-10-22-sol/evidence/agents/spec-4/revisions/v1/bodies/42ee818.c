// ==== 0x42ee818  FUN_042ee818  (524 bytes)

void FUN_042ee818(double param_1,double param_2,long param_3,undefined8 *param_4)

{
  long lVar1;
  code *pcVar2;
  int iVar3;
  undefined8 uVar4;
  undefined4 *puVar5;
  undefined8 *puVar6;
  long lVar7;
  long lVar8;
  int *piVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  
  lVar7 = *(long *)(param_3 + 0x40);
  if (lVar7 == 0) {
                    /* WARNING: Could not recover jumptable at 0x042ee95c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)param_4[2])(param_4,2);
    return;
  }
  uVar4 = FUN_04315374();
  uVar4 = FUN_0009278c(uVar4,lVar7,1);
  lVar7 = 0x295e9648864000;
  if ((param_1 != 0.0) && (lVar7 = 0, !NAN(param_1))) {
    param_1 = param_1 * 1000000.0;
    lVar7 = 0;
    if (-9.223372036854776e+18 <= param_1) {
      lVar7 = 0x7fffffffffffffff;
    }
    lVar8 = (long)param_1;
    if (9.223372036854775e+18 < param_1) {
      lVar8 = lVar7;
    }
    lVar7 = lVar8;
    if (1 < lVar8 + 0x8000000000000001U) {
      lVar7 = 0x7fffffffffffffff;
      if (!SCARRY8(lVar8,0x295e9648864000)) {
        lVar7 = lVar8 + 0x295e9648864000;
      }
    }
  }
  lVar8 = 0x295e9648864000;
  if ((param_2 != 0.0) && (lVar8 = 0, !NAN(param_2))) {
    param_2 = param_2 * 1000000.0;
    lVar8 = 0;
    if (-9.223372036854776e+18 <= param_2) {
      lVar8 = 0x7fffffffffffffff;
    }
    lVar1 = (long)param_2;
    if (9.223372036854775e+18 < param_2) {
      lVar1 = lVar8;
    }
    lVar8 = lVar1;
    if (1 < lVar1 + 0x8000000000000001U) {
      lVar8 = 0x7fffffffffffffff;
      if (!SCARRY8(lVar1,0x295e9648864000)) {
        lVar8 = lVar1 + 0x295e9648864000;
      }
    }
  }
  piVar9 = *(int **)(param_3 + 0xa8);
  if ((piVar9 != (int *)0x0) && (iVar3 = *piVar9, *piVar9 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(0,0x42eea24);
    (*pcVar2)();
  }
  uVar10 = *(undefined8 *)(param_3 + 0xb0);
  puVar5 = (undefined4 *)FUN_00008790(0x60);
  *puVar5 = 1;
  *(undefined **)(puVar5 + 2) = &DAT_042da57c;
  *(code **)(puVar5 + 4) = FUN_042da658;
  *(undefined1 **)(puVar5 + 6) = &LAB_04169bc0;
  *(undefined1 **)(puVar5 + 8) = &LAB_042ed7f4;
  *(undefined8 *)(puVar5 + 10) = 0;
  *(int **)(puVar5 + 0xc) = piVar9;
  *(undefined8 *)(puVar5 + 0xe) = uVar10;
  puVar6 = (undefined8 *)param_4[3];
  uVar10 = *param_4;
  uVar12 = param_4[3];
  uVar11 = param_4[2];
  *(undefined8 *)(puVar5 + 0x12) = param_4[1];
  *(undefined8 *)(puVar5 + 0x10) = uVar10;
  *(undefined8 *)(puVar5 + 0x16) = uVar12;
  *(undefined8 *)(puVar5 + 0x14) = uVar11;
  if ((code *)*puVar6 != (code *)0x0) {
    uVar10 = (*(code *)*puVar6)(*param_4);
    *(undefined8 *)(puVar5 + 0x10) = uVar10;
  }
  FUN_043185d4(uVar4,lVar7,lVar8,puVar5);
  return;
}


