// ==== 0x6afe0e0  FUN_06afe0e0  (580 bytes)

undefined8
FUN_06afe0e0(long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
            undefined8 param_5)

{
  uint uVar1;
  long *plVar2;
  code *pcVar3;
  int iVar4;
  ulong uVar5;
  int *piVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 local_48;
  
  local_48 = param_5;
  uVar5 = FUN_01559e00(0);
  if ((uVar5 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0,0x6afe314);
    (*pcVar3)();
  }
  piVar6 = (int *)FUN_022a58ec(*(undefined8 *)(param_1 + 0x90),param_2,0x1f);
  if (piVar6 == (int *)0x0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0,0x6afe320);
    (*pcVar3)();
  }
  iVar4 = *piVar6;
  *piVar6 = iVar4 + 1;
  if (iVar4 == 0x7fffffff) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0,0x6afe32c);
    (*pcVar3)();
  }
  uVar1 = (int)param_3 - 9;
  if (5 < uVar1) {
    uVar8 = *(undefined8 *)(*(long *)(*(long *)(param_1 + 0x80) + 0x10) + 0x48);
    iVar4 = FUN_06b11550(uVar8,piVar6,param_4);
    if ((iVar4 == 0) || (iVar4 = FUN_06b11668(uVar8,piVar6,param_4), iVar4 != 0)) {
      FUN_06aff724(*(undefined8 *)(param_1 + 0x90),piVar6);
      uVar8 = 0;
      goto LAB_06afe194;
    }
  }
  uVar8 = *(undefined8 *)(param_1 + 0x40);
  iVar4 = *piVar6;
  *piVar6 = iVar4 + 1;
  if (iVar4 == 0x7fffffff) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0,0x6afe338);
    (*pcVar3)();
  }
  FUN_078117c0(uVar8,piVar6);
  if (DAT_0e3b0c58 == (long *)0x0) {
    DAT_0e3b0c58 = (long *)FUN_00956e40("Extensions.UninstallType",1,100,0x65,1);
  }
  plVar2 = DAT_0e3b0c58;
  uVar8 = FUN_0457672c(piVar6);
  (**(code **)(*plVar2 + 0x30))(plVar2,uVar8);
  FUN_06afcecc(param_1,*(undefined8 *)(piVar6 + 0x8a),3);
  iVar4 = FUN_00080ffc(*(long *)(param_1 + 0x90) + 0x50,*(undefined8 *)(piVar6 + 0x8a));
  if (iVar4 != 0) {
    FUN_06aff874(*(undefined8 *)(param_1 + 0x90),*(undefined8 *)(piVar6 + 0x8a));
  }
  iVar4 = FUN_00080ffc(*(long *)(param_1 + 0x90) + 0x68,*(undefined8 *)(piVar6 + 0x8a));
  if (iVar4 != 0) {
    FUN_06aff884(*(undefined8 *)(param_1 + 0x90),*(undefined8 *)(piVar6 + 0x8a));
  }
  uVar8 = local_48;
  uVar7 = *(undefined8 *)(param_1 + 0x40);
  iVar4 = *piVar6;
  *piVar6 = iVar4 + 1;
  if (iVar4 == 0x7fffffff) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0,0x6afe344);
    (*pcVar3)();
  }
  local_48 = 0;
  FUN_07811818(uVar7,piVar6,uVar8);
  FUN_0417c230(param_1,*(undefined8 *)(piVar6 + 0x8a));
  FUN_06aff600(*(undefined8 *)(param_1 + 0x90),piVar6,param_3);
  FUN_029fc7c0(*(long *)(param_1 + 0xa0) + 0x10,*(undefined8 *)(piVar6 + 0x8a));
  FUN_06af92cc(*(undefined8 *)(param_1 + 0x88),*(long *)(piVar6 + 0x8a),
               *(undefined4 *)(*(long *)(piVar6 + 0x8a) + 0x50),uVar1 < 6);
  uVar8 = 1;
LAB_06afe194:
  LOAcquire();
  iVar4 = *piVar6;
  *piVar6 = iVar4 + -1;
  LORelease();
  if (iVar4 == 1) {
    uVar7 = thunk_FUN_02d613c8(piVar6);
    FUN_00005810(uVar7,0x268);
  }
  FUN_00d5ff9c(&local_48);
  return uVar8;
}


