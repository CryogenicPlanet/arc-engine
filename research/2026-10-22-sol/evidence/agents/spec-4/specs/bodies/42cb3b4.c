// ==== 0x42cb3b4  FUN_042cb3b4  (248 bytes)

void FUN_042cb3b4(long param_1,undefined8 *param_2)

{
  long *plVar1;
  undefined4 *puVar2;
  undefined8 uVar3;
  long lVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  long local_28;
  
  if (*(long *)(param_1 + 0x88) != 0) {
    FUN_0119426c();
    plVar1 = (long *)FUN_02c31954();
    if (plVar1 != (long *)0x0) {
      FUN_00008790(0x158);
      local_28 = FUN_0554a6fc();
      puVar2 = (undefined4 *)FUN_00008790(0x48);
      uVar3 = *param_2;
      uVar6 = param_2[3];
      uVar5 = param_2[2];
      *(undefined8 *)(puVar2 + 0xc) = param_2[1];
      *(undefined8 *)(puVar2 + 10) = uVar3;
      *puVar2 = 1;
      *(undefined1 **)(puVar2 + 2) = &LAB_042da4f4;
      *(code **)(puVar2 + 4) = FUN_042a34c0;
      *(code **)(puVar2 + 6) = FUN_00b77080;
      *(undefined1 **)(puVar2 + 8) = &LAB_042cb4ac;
      lVar4 = param_2[3];
      *(undefined8 *)(puVar2 + 0x10) = uVar6;
      *(undefined8 *)(puVar2 + 0xe) = uVar5;
      if (*(long *)(lVar4 + 8) != 0) {
        param_2[2] = 0;
        param_2[3] = &DAT_0c4b3500;
      }
      (**(code **)(*plVar1 + 0x38))(plVar1,&local_28,puVar2);
      lVar4 = local_28;
      local_28 = 0;
      if (lVar4 != 0) {
        uVar3 = FUN_0554a748();
        FUN_00005810(uVar3,0x158);
      }
      return;
    }
  }
                    /* WARNING: Could not recover jumptable at 0x042cb498. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)param_2[2])(param_2,0);
  return;
}


