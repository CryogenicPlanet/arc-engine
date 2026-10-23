// ==== 0x435da28  FUN_0435da28  (356 bytes)

void FUN_0435da28(undefined8 *param_1,long param_2,int *param_3)

{
  code *pcVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int *piVar5;
  undefined8 uVar6;
  undefined1 auStack_60 [32];
  
  if (*(long *)(param_2 + 8) == 0) {
    uVar6 = FUN_04b37104(0x25,"managed webcontents is null.",0x1c,0x51,
                         "../../arc/browser/web_contents/devtools_protocol_wrapper.cc");
    *param_1 = uVar6;
    if (param_3 != (int *)0x0) {
      LOAcquire();
      iVar2 = *param_3;
      *param_3 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
                    /* WARNING: Could not recover jumptable at 0x0435db84. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (**(code **)(param_3 + 4))(param_3);
        return;
      }
    }
  }
  else {
    piVar5 = *(int **)(param_2 + 0xb0);
    if (piVar5 != (int *)0x0) {
      iVar2 = *piVar5;
      *piVar5 = iVar2 + 1;
      if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x435db8c);
        (*pcVar1)();
      }
    }
    uVar6 = *(undefined8 *)(param_2 + 0xb8);
    puVar3 = (undefined4 *)FUN_00008790(0x48);
    *puVar3 = 1;
    *(undefined1 **)(puVar3 + 2) = &LAB_04075dc0;
    *(code **)(puVar3 + 4) = FUN_0407bd40;
    *(undefined1 **)(puVar3 + 6) = &LAB_040c0f80;
    *(code **)(puVar3 + 8) = FUN_0435db94;
    *(undefined8 *)(puVar3 + 10) = 0;
    *(int **)(puVar3 + 0xc) = piVar5;
    *(undefined8 *)(puVar3 + 0xe) = uVar6;
    *(int **)(puVar3 + 0x10) = param_3;
    FUN_00029e40(auStack_60,"GetMaxTextureSize","../../content/public/browser/gpu_utils.cc",0x9d);
    puVar4 = (undefined4 *)FUN_00008790(0x30);
    *puVar4 = 1;
    *(code **)(puVar4 + 2) = FUN_040bf0c8;
    *(code **)(puVar4 + 4) = FUN_0407a980;
    *(code **)(puVar4 + 6) = FUN_00b77080;
    *(undefined4 **)(puVar4 + 10) = puVar3;
    FUN_03a70080(auStack_60,1,0,puVar4);
    *param_1 = 1;
  }
  return;
}


