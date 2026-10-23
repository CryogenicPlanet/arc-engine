// ==== 0x43793ac  FUN_043793ac  (324 bytes)

void FUN_043793ac(void)

{
  bool bVar1;
  long lVar2;
  code *pcVar3;
  int iVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  long lVar7;
  long lVar8;
  undefined1 local_70;
  undefined7 uStack_6f;
  long local_68;
  long local_60;
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
      puVar5 = (undefined8 *)FUN_00008790(8);
      *puVar5 = &PTR__cxx_destruct_0da88798;
      iVar4 = FUN_009180c0(&local_50,0,0,0,puVar5,"arc.mojom.RenderFrameAgent",&LAB_04429bcc,
                           &LAB_04051dc0);
      lVar2 = lStack_48;
      if (iVar4 != 0) {
        puVar6 = (undefined8 *)FUN_00008790(0x10);
        puVar5 = local_28;
        *puVar6 = &PTR__cxx_destruct_0da88710;
        puVar6[1] = lVar2;
        bVar1 = local_28 != (undefined8 *)0x0;
        local_28 = puVar6;
        if (bVar1) {
          FUN_00005810(puVar5,0x10);
        }
      }
    }
    local_70 = 0;
    local_58 = '\0';
    FUN_0442a24c(local_28,&local_70);
    if ((local_58 != '\0') && (lVar2 = CONCAT71(uStack_6f,local_70), lVar2 != 0)) {
      if (local_68 * 4 != 0) {
        lVar7 = local_68 * -4;
        lVar8 = local_68 * 4 + lVar2;
        do {
          lVar8 = lVar8 + -4;
          if (lVar8 == 0) {
                    /* WARNING: Does not return */
            pcVar3 = (code *)SoftwareBreakpoint(1,0x43794f0);
            (*pcVar3)();
          }
          lVar7 = lVar7 + 4;
        } while (lVar7 != 0);
      }
      local_68 = 0;
      FUN_00005810(lVar2,local_60 << 2);
    }
  }
  puVar5 = local_28;
  local_28 = (undefined8 *)0x0;
  if (puVar5 != (undefined8 *)0x0) {
    FUN_00005810(puVar5,0x10);
  }
  FUN_00111f30(&local_50);
  return;
}


