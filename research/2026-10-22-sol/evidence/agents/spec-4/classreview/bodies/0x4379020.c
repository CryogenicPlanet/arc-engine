// ==== 0x4379020  FUN_04379020  (260 bytes)

void FUN_04379020(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  bool bVar1;
  long lVar2;
  int iVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  undefined8 local_60;
  long lStack_58;
  long local_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 *local_38;
  
  uStack_48 = 0;
  local_50 = 0;
  local_38 = (undefined8 *)0x0;
  uStack_40 = 0;
  lStack_58 = 0;
  local_60 = 0;
  FUN_043755a4(&local_60);
  if (local_50 != 0 || lStack_58 != 0) {
    if (local_38 == (undefined8 *)0x0) {
      puVar4 = (undefined8 *)FUN_00008790(8);
      *puVar4 = &PTR__cxx_destruct_0da88798;
      iVar3 = FUN_009180c0(&local_60,0,0,0,puVar4,"arc.mojom.RenderFrameAgent",&LAB_04429bcc,
                           &LAB_04051dc0);
      lVar2 = lStack_58;
      if (iVar3 != 0) {
        puVar5 = (undefined8 *)FUN_00008790(0x10);
        puVar4 = local_38;
        *puVar5 = &PTR__cxx_destruct_0da88710;
        puVar5[1] = lVar2;
        bVar1 = local_38 != (undefined8 *)0x0;
        local_38 = puVar5;
        if (bVar1) {
          FUN_00005810(puVar4,0x10);
        }
      }
    }
    FUN_04429e2c(local_38,param_2,param_3);
  }
  puVar4 = local_38;
  local_38 = (undefined8 *)0x0;
  if (puVar4 != (undefined8 *)0x0) {
    FUN_00005810(puVar4,0x10);
  }
  FUN_00111f30(&local_60);
  return;
}


