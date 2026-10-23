// ==== 0x435db94  FUN_0435db94  (496 bytes)

void FUN_0435db94(long param_1,undefined8 param_2,ulong param_3)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  long *plVar4;
  undefined8 *puVar5;
  undefined4 *puVar6;
  ulong uVar7;
  long lVar8;
  int *piVar9;
  undefined8 uVar10;
  char local_70 [16];
  undefined6 uStack_60;
  char cStack_5a;
  char cStack_59;
  byte local_58;
  undefined1 uStack_41;
  
  lVar3 = 0;
  if (*(long *)(*(long *)(param_1 + 8) + 0x8d8) != 0) {
    lVar3 = FUN_010f9b60();
  }
  lVar8 = *(long *)(*(long *)(lVar3 + 0x170) + 0x348);
  if ((lVar8 == 0) || (*(char *)(lVar8 + 4) != '\0')) {
    plVar4 = (long *)0x0;
  }
  else {
    plVar4 = *(long **)(*(long *)(lVar3 + 0x170) + 0x350);
  }
  (**(code **)(*plVar4 + 0x1e8))(plVar4,param_3 & 0xffffffff | 0x100000000);
  puVar5 = (undefined8 *)FUN_00008790(0x18);
  puVar5[1] = 0;
  puVar5[2] = 0;
  *puVar5 = 0;
  local_70[0] = 1;
  local_58 = 1;
  FUN_0053a240(puVar5,"fromSurface",0xb,local_70);
  if ((ulong)local_58 != 0xff) {
    (*(code *)(&PTR__cxx_destruct_0df6d258)[local_58])(&uStack_41,local_70);
  }
  local_70[0] = 1;
  local_58 = 1;
  FUN_0053a240(puVar5,"captureBeyondViewport",0x15,local_70);
  if ((ulong)local_58 != 0xff) {
    (*(code *)(&PTR__cxx_destruct_0df6d258)[local_58])(&uStack_41,local_70);
  }
  cStack_59 = '\x16';
  if ("Page.captureScreenshot" < &cStack_5a && local_70 < "age.captureScreenshot") {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x435dd7c);
    (*pcVar1)();
  }
  builtin_strncpy(local_70,"Page.captureScre",0x10);
  uStack_60 = 0x746f68736e65;
  cStack_5a = '\0';
  piVar9 = *(int **)(param_1 + 0xb0);
  if ((piVar9 != (int *)0x0) && (iVar2 = *piVar9, *piVar9 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x435dd8c);
    (*pcVar1)();
  }
  uVar10 = *(undefined8 *)(param_1 + 0xb8);
  puVar6 = (undefined4 *)FUN_00008790(0x48);
  *puVar6 = 1;
  *(undefined1 **)(puVar6 + 2) = &LAB_0435eedc;
  *(code **)(puVar6 + 4) = FUN_0407bd40;
  *(undefined1 **)(puVar6 + 6) = &LAB_040c0f80;
  *(undefined1 **)(puVar6 + 8) = &LAB_0435e5c4;
  *(undefined8 *)(puVar6 + 10) = 0;
  *(int **)(puVar6 + 0xc) = piVar9;
  *(undefined8 *)(puVar6 + 0xe) = uVar10;
  *(undefined8 *)(puVar6 + 0x10) = param_2;
  uVar7 = FUN_0435dd94(param_1,local_70,puVar5,puVar6);
  if (cStack_59 < '\0') {
    FUN_00005810(local_70._0_8_,
                 CONCAT17(cStack_59,CONCAT16(cStack_5a,uStack_60)) & 0x7fffffffffffffff);
  }
  if (uVar7 >> 0x20 != 0) {
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0,0x435dd80);
  (*pcVar1)();
}


