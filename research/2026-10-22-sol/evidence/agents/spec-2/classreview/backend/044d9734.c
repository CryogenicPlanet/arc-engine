// ==== 0x44d9734  FUN_044d9734  (764 bytes)

/* WARNING: Removing unreachable block (ram,0x044d99f8) */

void FUN_044d9734(long *param_1,undefined8 param_2,uint param_3)

{
  code *pcVar1;
  long *plVar2;
  int *piVar3;
  int *piVar4;
  long lVar5;
  int *piVar6;
  ulong uVar7;
  ulong uVar8;
  int iVar9;
  undefined8 local_a0;
  undefined8 uStack_98;
  long *local_90;
  ulong local_88;
  ulong local_80;
  undefined8 local_70;
  undefined8 uStack_68;
  long *local_60;
  ulong local_58;
  ulong local_50;
  undefined1 local_40 [24];
  byte local_28;
  
  piVar4 = (int *)*param_1;
  lVar5 = param_1[1] * 0x28;
  iVar9 = (int)param_2;
  piVar3 = piVar4;
  if (lVar5 != 0) {
    uVar7 = (lVar5 >> 3) * -0x3333333333333333;
    piVar6 = piVar4;
    do {
      uVar8 = uVar7 >> 1;
      piVar3 = piVar6 + uVar8 * 10 + 10;
      uVar7 = uVar7 + ~(uVar7 >> 1);
      if (iVar9 <= piVar6[uVar8 * 10]) {
        piVar3 = piVar6;
        uVar7 = uVar8;
      }
      piVar6 = piVar3;
    } while (uVar7 != 0);
  }
  piVar6 = piVar4 + param_1[1] * 10;
  if ((piVar3 == piVar6) || (iVar9 < *piVar3)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x44d9a1c);
    (*pcVar1)();
  }
  if ((char)piVar3[8] != '\x01') {
    if ((char)piVar3[8] == -1) {
                    /* WARNING: Subroutine does not return */
      FUN_04b335b8("std::get: variant is valueless");
    }
                    /* WARNING: Subroutine does not return */
    FUN_04b335b8("std::get: wrong alternative for variant");
  }
  if (param_3 == *(byte *)(piVar3 + 2)) {
    return;
  }
  if (lVar5 != 0) {
    uVar7 = (lVar5 >> 3) * -0x3333333333333333;
    piVar3 = piVar4;
    do {
      uVar8 = uVar7 >> 1;
      piVar4 = piVar3 + uVar8 * 10 + 10;
      uVar7 = uVar7 + ~(uVar7 >> 1);
      if (iVar9 <= piVar3[uVar8 * 10]) {
        piVar4 = piVar3;
        uVar7 = uVar8;
      }
      piVar3 = piVar4;
    } while (uVar7 != 0);
  }
  if ((piVar4 == piVar6) || (iVar9 < *piVar4)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x44d99c4);
    (*pcVar1)();
  }
  local_40[0] = (undefined1)param_3;
  local_28 = 1;
  FUN_01195640(piVar4 + 2,local_40);
  if (((local_28 != 0xff) && ((local_28 & 0xfd) != 0)) && (local_28 != 4)) {
    (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_28])(&local_70,local_40);
  }
  local_28 = 0xff;
  local_50 = 0;
  uStack_68 = 0;
  local_70 = 0;
  local_58 = 0;
  local_60 = (long *)0x0;
  if (param_1[8] != 0) {
    FUN_04307afc(&local_70,param_1 + 3);
  }
  local_80 = 0;
  uStack_98 = 0;
  local_a0 = 0;
  local_88 = 0;
  local_90 = (long *)0x0;
  do {
    if (local_60 == (long *)0x0) {
LAB_044d9930:
      if (local_90 == (long *)0x0) goto LAB_044d9884;
      uVar7 = local_90[1];
      if (local_80 <= (ulong)local_90[1]) {
        uVar7 = local_80;
      }
      if (local_88 == uVar7) goto LAB_044d9884;
    }
    else {
      uVar7 = local_60[1];
      if (local_50 <= (ulong)local_60[1]) {
        uVar7 = local_50;
      }
      if (local_58 == uVar7) goto LAB_044d9930;
    }
    if (local_58 == local_88 && local_60 == local_90) {
LAB_044d9884:
      FUN_042db300(&local_a0);
      FUN_042db300(&local_70);
      return;
    }
    if ((ulong)local_60[1] <= local_58) {
LAB_044d99c8:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x44d99cc);
      (*pcVar1)();
    }
    plVar2 = (long *)(*local_60 + local_58 * 0x10);
    lVar5 = *plVar2;
    if ((lVar5 == 0) || (*(char *)(lVar5 + 4) != '\0')) {
      plVar2 = (long *)0x0;
    }
    else {
      plVar2 = (long *)plVar2[1];
    }
    (**(code **)(*plVar2 + 0x10))(plVar2,param_2);
    if (local_60 != (long *)0x0) {
      uVar7 = local_60[1];
      if (local_50 <= (ulong)local_60[1]) {
        uVar7 = local_50;
      }
      do {
        local_58 = local_58 + 1;
        if (uVar7 <= local_58) break;
        if ((ulong)local_60[1] <= local_58) goto LAB_044d99c8;
        plVar2 = (long *)(*local_60 + local_58 * 0x10);
        if ((plVar2[1] != 0) && ((*plVar2 == 0 || (*(char *)(*plVar2 + 4) != '\0')))) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(0,0x44d9a10);
          (*pcVar1)();
        }
      } while (((*plVar2 == 0) || (*(char *)(*plVar2 + 4) != '\0')) || (plVar2[1] == 0));
    }
  } while( true );
}


