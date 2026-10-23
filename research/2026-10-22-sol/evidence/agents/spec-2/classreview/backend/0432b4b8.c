// ==== 0x432b4b8  FUN_0432b4b8  (512 bytes)

undefined8 * FUN_0432b4b8(undefined8 *param_1,char *param_2,undefined8 *param_3)

{
  int *piVar1;
  code *pcVar2;
  int iVar3;
  ulong uVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined8 *puVar7;
  undefined8 uVar8;
  long lVar9;
  char *pcVar10;
  undefined8 *puVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  char *local_78;
  undefined8 uStack_70;
  char *local_68;
  size_t local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  *param_1 = 0;
  uVar4 = FUN_01559e00(0);
  if ((uVar4 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(0,0x432b6a8);
    (*pcVar2)();
  }
  puVar5 = (undefined8 *)FUN_00008790(0x40);
  uVar8 = *param_3;
  uVar6 = param_3[1];
  puVar11 = (undefined8 *)param_3[3];
  uVar13 = param_3[3];
  uVar12 = param_3[2];
  if (puVar11[1] != 0) {
    param_3[2] = 0;
    param_3[3] = &DAT_0c4b3500;
    if (puVar11[1] != 0) {
      puVar11 = &DAT_0c4b3500;
    }
  }
  *puVar5 = &PTR_FUN_0da7e4c0;
  puVar5[1] = uVar8;
  puVar5[2] = uVar6;
  puVar5[4] = uVar13;
  puVar5[3] = uVar12;
  local_78 = "bcny-";
  uStack_70 = 5;
  if (param_2[0x17] < '\0') {
    param_2 = *(char **)param_2;
  }
  local_68 = param_2;
  if (param_2 != (char *)0x0) {
    pcVar10 = (char *)(puVar5 + 5);
    local_60 = _strlen(param_2);
    FUN_00217a74(puVar5 + 5,&local_78,2);
    uVar6 = FUN_00f80684();
    if (*(char *)((long)puVar5 + 0x3f) < '\0') {
      pcVar10 = *(char **)pcVar10;
    }
    if (pcVar10 != (char *)0x0) {
      _strlen(pcVar10);
      puVar7 = (undefined8 *)(*(code *)PTR___tlv_bootstrap_0e379858)();
      if (((long *)*puVar7 == (long *)0x0) || (lVar9 = *(long *)*puVar7, lVar9 == 0)) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x432b6b8);
        (*pcVar2)();
      }
      piVar1 = (int *)(lVar9 + 8);
      iVar3 = *piVar1;
      *piVar1 = iVar3 + 1;
      if (iVar3 == 0x7fffffff) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x432b6c4);
        (*pcVar2)();
      }
      local_78 = (char *)CONCAT71(local_78._1_7_,1);
      FUN_016d1354(uVar6,puVar5,pcVar10);
      if ((code *)puVar11[1] != (code *)0x0) {
        (*(code *)puVar11[1])(uVar8);
      }
      puVar11 = (undefined8 *)*param_1;
      *param_1 = puVar5;
      if (puVar11 != (undefined8 *)0x0) {
        *puVar11 = &PTR_FUN_0da7e4c0;
        uVar8 = FUN_00f80684();
        FUN_01e3d3f8(uVar8,puVar11,0);
        if (*(char *)((long)puVar11 + 0x3f) < '\0') {
          FUN_00005810(puVar11[5],puVar11[7] & 0x7fffffffffffffff);
        }
        if (*(code **)(puVar11[4] + 8) != (code *)0x0) {
          (**(code **)(puVar11[4] + 8))(puVar11[1]);
        }
        FUN_00005810(puVar11,0x40);
      }
      if (*(long *)PTR____stack_chk_guard_0d824020 != local_58) {
                    /* WARNING: Subroutine does not return */
        ___stack_chk_fail();
      }
      return param_1;
    }
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x432b6b0);
  (*pcVar2)();
}


