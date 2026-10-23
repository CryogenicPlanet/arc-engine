// ==== 0x437b014  FUN_0437b014  (776 bytes)

void FUN_0437b014(undefined8 *param_1,long param_2,long *param_3)

{
  code *pcVar1;
  int iVar2;
  undefined8 uVar3;
  long *plVar4;
  long lVar5;
  ulong uVar6;
  char *pcVar7;
  ulong extraout_x1;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined4 uVar10;
  long *plVar11;
  long lVar12;
  undefined1 local_58 [16];
  undefined1 local_48;
  
  if (param_3 == (long *)0x0) {
    pcVar7 = "browser is null";
    uVar3 = 0xd;
    uVar8 = 0xf;
    uVar9 = 0x991;
  }
  else if (*(long *)(param_2 + 0x130) == 0) {
    pcVar7 = "managed webcontents is null";
    uVar3 = 0x25;
    uVar8 = 0x1b;
    uVar9 = 0x995;
  }
  else {
    plVar4 = (long *)(**(code **)(*param_3 + 0x78))(param_3);
    lVar5 = FUN_018efdf0(*(undefined8 *)(*(long *)(DAT_0e481618 + 0x2a0) + 8),
                         *(undefined8 *)(param_2 + 0x130));
    if ((((lVar5 == 0) || (lVar5 = FUN_0031dab4(), lVar5 == 0)) || (*(long *)(lVar5 + 0x28) == 0))
       || (((*(char *)(*(long *)(lVar5 + 0x28) + 4) != '\0' || (*(long *)(lVar5 + 0x30) == 0)) ||
           ((*(long *)(lVar5 + 0x28) == 0 ||
            ((*(char *)(*(long *)(lVar5 + 0x28) + 4) != '\0' ||
             (plVar11 = *(long **)(lVar5 + 0x30),
             plVar11 == (long *)0x0 || plVar11 == (long *)segment_command_00000020.segname)))))))) {
      pcVar7 = "webcontents not attached to any browser";
      uVar3 = 0x25;
      uVar8 = 0x27;
      uVar9 = 0x99e;
    }
    else if (plVar11 == param_3) {
      pcVar7 = "browser equals current browser";
      uVar3 = 0xd;
      uVar8 = 0x1e;
      uVar9 = 0x9a3;
    }
    else {
      lVar12 = plVar11[2];
      lVar5 = (**(code **)(*param_3 + 0x38))(param_3);
      if (lVar12 == lVar5) {
        plVar11 = (long *)(**(code **)(*plVar11 + 0x78))(plVar11);
        uVar6 = FUN_00027de0(plVar11,*(undefined8 *)(param_2 + 0x130));
        if (((plVar11[0x12] == 0) || (iVar2 = FUN_00403d24(*plVar11), (extraout_x1 & 1) == 0)) ||
           (iVar2 < 0)) {
          uVar3 = (*(code *)PTR_FUN_0e2f1ea8)();
          if (((int)uVar6 < 0) || (*(int *)(*plVar11 + 0x18) <= (int)uVar6)) {
                    /* WARNING: Does not return */
            pcVar1 = (code *)SoftwareBreakpoint(0,0x437b318);
            (*pcVar1)();
          }
          uVar8 = FUN_003abe30(*plVar11,uVar6 & 0xffffffff);
          FUN_0335ad80(plVar11,uVar8,6,uVar3);
        }
        FUN_01e7c404(plVar11,param_2 + 0xe0);
        lVar5 = FUN_00882ba0(*(undefined8 *)(param_2 + 0x130));
        if (lVar5 != 0) {
          FUN_01bdfd00(lVar5 + 0x48,param_2 + 0x100);
        }
        lVar5 = FUN_000805e0(*(long *)(param_2 + 0x130) + 8,&DAT_0cf208a0);
        if (lVar5 != 0) {
          FUN_04d1bbc0(lVar5 + 0xa0,param_2 + 200);
        }
        if (*(long *)(param_2 + 0x2b8) != 0) {
          FUN_04357fd0();
        }
        lVar5 = FUN_07e1094c(plVar11,uVar6);
        lVar12 = FUN_000805e0(*(long *)(param_2 + 0x130) + 8,&DAT_0e4a2f38);
        if (lVar12 == 0) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(0,0x437b324);
          (*pcVar1)();
        }
        FUN_02b4d6c8(*(undefined8 *)(lVar5 + 0x40),0);
        FUN_04353534(param_2 + 0x28,0);
        uVar10 = 9;
        if (*(long *)(*plVar4 + 0x18) != 0) {
          uVar10 = 0;
        }
        local_58[0] = 0;
        local_48 = 0;
        FUN_0422a9e4(plVar4,*(long *)(*plVar4 + 0x18),lVar5,uVar10,local_58,0,0);
        if (*(char *)(param_2 + 0x5c0) != '\0') {
          FUN_01e7c404(plVar11,param_2 + 0xe0);
          FUN_0210cd90(plVar4 + 4,param_2 + 0xe0);
          FUN_0210cc84(param_2 + 0xe0,plVar4);
        }
        FUN_04373cc4(param_2);
        uVar3 = 1;
        goto LAB_0437b080;
      }
      pcVar7 = "browser\'s profile does not equal current browse profile";
      uVar3 = 0xd;
      uVar8 = 0x37;
      uVar9 = 0x9a8;
    }
  }
  uVar3 = FUN_04b37104(uVar3,pcVar7,uVar8,uVar9,
                       "../../arc/browser/web_contents/web_contents_impl.cc");
LAB_0437b080:
  *param_1 = uVar3;
  return;
}


