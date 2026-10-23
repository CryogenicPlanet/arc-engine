// ==== 0x43794f0  FUN_043794f0  (268 bytes)

void FUN_043794f0(undefined8 param_1,int param_2)

{
  bool bVar1;
  long lVar2;
  code *pcVar3;
  int iVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  undefined1 *puVar7;
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
    if (param_2 == 0) {
      puVar7 = &DAT_100000001;
    }
    else {
      if (param_2 != 1) {
                    /* WARNING: Does not return */
        pcVar3 = (code *)SoftwareBreakpoint(0x550b,0x43795fc);
        (*pcVar3)();
      }
      puVar7 = (undefined1 *)0x100000000;
    }
    FUN_0442a3a0(local_28,puVar7);
  }
  puVar5 = local_28;
  local_28 = (undefined8 *)0x0;
  if (puVar5 != (undefined8 *)0x0) {
    FUN_00005810(puVar5,0x10);
  }
  FUN_00111f30(&local_50);
  return;
}


