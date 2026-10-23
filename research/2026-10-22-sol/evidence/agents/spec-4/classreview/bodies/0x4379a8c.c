// ==== 0x4379a8c  FUN_04379a8c  (244 bytes)

void FUN_04379a8c(undefined8 param_1,undefined8 param_2)

{
  bool bVar1;
  long lVar2;
  int iVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
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
      lVar2 = lStack_48;
      if (iVar3 != 0) {
        puVar5 = (undefined8 *)FUN_00008790(0x10);
        puVar4 = local_28;
        *puVar5 = &PTR__cxx_destruct_0da88710;
        puVar5[1] = lVar2;
        bVar1 = local_28 != (undefined8 *)0x0;
        local_28 = puVar5;
        if (bVar1) {
          FUN_00005810(puVar4,0x10);
        }
      }
    }
    FUN_0442a510(local_28,param_2);
  }
  puVar4 = local_28;
  local_28 = (undefined8 *)0x0;
  if (puVar4 != (undefined8 *)0x0) {
    FUN_00005810(puVar4,0x10);
  }
  FUN_00111f30(&local_50);
  return;
}


