// ==== 0x4379218  FUN_04379218  (404 bytes)

void FUN_04379218(undefined8 param_1,undefined8 *param_2)

{
  bool bVar1;
  code *pcVar2;
  int iVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  long lVar6;
  long lVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  undefined8 *local_70;
  long local_68;
  long lStack_60;
  char local_58;
  undefined8 local_50;
  long lStack_48;
  long local_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 *local_28;
  
  uStack_38 = 0;
  local_40 = 0;
  local_28 = (undefined8 *)0x0;
  uStack_30 = 0;
  lStack_48 = 0;
  local_50 = 0;
  FUN_043755a4(&local_50);
  if (local_40 != 0 || lStack_48 != 0) {
    if (local_28 == (undefined8 *)0x0) {
      puVar4 = (undefined8 *)FUN_00008790(8);
      *puVar4 = &PTR__cxx_destruct_0da88798;
      iVar3 = FUN_009180c0(&local_50,0,0,0,puVar4,"arc.mojom.RenderFrameAgent",&LAB_04429bcc,
                           &LAB_04051dc0);
      lVar6 = lStack_48;
      if (iVar3 != 0) {
        puVar5 = (undefined8 *)FUN_00008790(0x10);
        puVar4 = local_28;
        *puVar5 = &PTR__cxx_destruct_0da88710;
        puVar5[1] = lVar6;
        bVar1 = local_28 != (undefined8 *)0x0;
        local_28 = puVar5;
        if (bVar1) {
          FUN_00005810(puVar4,0x10);
        }
      }
    }
    puVar4 = local_28;
    uVar16 = param_2[1];
    uVar14 = *param_2;
    uVar11 = param_2[3];
    uVar8 = param_2[2];
    uVar17 = param_2[5];
    uVar15 = param_2[4];
    uVar12 = param_2[7];
    uVar9 = param_2[6];
    uVar13 = param_2[9];
    uVar10 = param_2[8];
    local_70 = (undefined8 *)FUN_00008790(0x50);
    local_70[1] = uVar16;
    *local_70 = uVar14;
    local_70[3] = uVar11;
    local_70[2] = uVar8;
    local_70[5] = uVar17;
    local_70[4] = uVar15;
    local_70[7] = uVar12;
    local_70[6] = uVar9;
    local_70[9] = uVar13;
    local_70[8] = uVar10;
    lStack_60 = 0x14;
    local_68 = 0x14;
    local_58 = '\x01';
    FUN_0442a24c(puVar4,&local_70);
    if ((local_58 != '\0') && (local_70 != (undefined8 *)0x0)) {
      if (local_68 * 4 != 0) {
        lVar6 = local_68 * -4;
        lVar7 = local_68 * 4 + (long)local_70;
        do {
          lVar7 = lVar7 + -4;
          if (lVar7 == 0) {
                    /* WARNING: Does not return */
            pcVar2 = (code *)SoftwareBreakpoint(1,0x43793ac);
            (*pcVar2)();
          }
          lVar6 = lVar6 + 4;
        } while (lVar6 != 0);
      }
      local_68 = 0;
      FUN_00005810(local_70,lStack_60 << 2);
    }
  }
  puVar4 = local_28;
  local_28 = (undefined8 *)0x0;
  if (puVar4 != (undefined8 *)0x0) {
    FUN_00005810(puVar4,0x10);
  }
  FUN_00111f30(&local_50);
  return;
}


