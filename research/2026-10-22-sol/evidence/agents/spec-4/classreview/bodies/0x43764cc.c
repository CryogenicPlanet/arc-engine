// ==== 0x43764cc  FUN_043764cc  (176 bytes)

void FUN_043764cc(undefined8 *param_1,long param_2)

{
  code *pcVar1;
  undefined1 uVar2;
  long *plVar3;
  long lVar4;
  long *plVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  if (*(long *)(param_2 + 0x130) == 0) {
    *(undefined1 *)param_1 = 0;
    uVar2 = 0;
  }
  else {
    plVar3 = *(long **)(*(long *)(param_2 + 0x130) + 0x8d8);
    if (plVar3 == (long *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x4376578);
      (*pcVar1)();
    }
    if (*(undefined1 **)(*plVar3 + 0xd0) == &LAB_04152228) {
      do {
        plVar5 = plVar3;
        plVar3 = (long *)plVar5[0x2f];
      } while ((long *)plVar5[0x2f] != (long *)0x0);
    }
    else {
      do {
        plVar5 = plVar3;
        plVar3 = (long *)plVar5[0x2f];
      } while ((long *)plVar5[0x2f] != (long *)0x0);
    }
    if ((*(byte *)(plVar5 + 0x219) & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4376580);
      (*pcVar1)();
    }
    lVar4 = plVar5[0x1de];
    if (*(char *)(lVar4 + 0x157) < '\0') {
      FUN_00060a60(param_1,*(undefined8 *)(lVar4 + 0x140),*(undefined8 *)(lVar4 + 0x148));
    }
    else {
      uVar7 = *(undefined8 *)(lVar4 + 0x148);
      uVar6 = *(undefined8 *)(lVar4 + 0x140);
      param_1[2] = *(undefined8 *)(lVar4 + 0x150);
      param_1[1] = uVar7;
      *param_1 = uVar6;
    }
    uVar2 = 1;
  }
  *(undefined1 *)(param_1 + 3) = uVar2;
  return;
}


