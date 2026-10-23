// ==== 0x42fbc80  FUN_042fbc80  (240 bytes)

void FUN_042fbc80(long param_1)

{
  code *pcVar1;
  int iVar2;
  long *plVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  int *piVar6;
  
  if (((*(long *)(param_1 + 0xf0) == 0) || (*(char *)(*(long *)(param_1 + 0xf0) + 4) != '\0')) ||
     (*(long *)(param_1 + 0xf8) == 0)) {
    return;
  }
  if ((*(long *)(param_1 + 0xf0) == 0) || (*(char *)(*(long *)(param_1 + 0xf0) + 4) != '\0')) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x42fbd6c);
    (*pcVar1)();
  }
  (**(code **)(**(long **)(param_1 + 0xf8) + 0x38))();
  FUN_0119426c();
  plVar3 = (long *)FUN_02c31954();
  piVar6 = *(int **)(param_1 + 0x298);
  if ((piVar6 != (int *)0x0) && (iVar2 = *piVar6, *piVar6 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x42fbd78);
    (*pcVar1)();
  }
  uVar5 = *(undefined8 *)(param_1 + 0x2a0);
  puVar4 = (undefined4 *)FUN_00008790(0x40);
  *puVar4 = 1;
  *(undefined1 **)(puVar4 + 2) = &LAB_040c5ad0;
  *(code **)(puVar4 + 4) = FUN_040a6300;
  *(undefined1 **)(puVar4 + 6) = &LAB_04169bc0;
  *(code **)(puVar4 + 8) = PiPNSView::viewWillStartLiveResize;
  *(undefined8 *)(puVar4 + 10) = 0;
  *(int **)(puVar4 + 0xc) = piVar6;
  *(undefined8 *)(puVar4 + 0xe) = uVar5;
                    /* WARNING: Could not recover jumptable at 0x042fbd64. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*plVar3 + 0x68))(plVar3,puVar4);
  return;
}


