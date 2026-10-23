// ==== 0x42eb354  FUN_042eb354  (304 bytes)

void FUN_042eb354(long param_1,undefined8 param_2,undefined8 *param_3)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  long local_60;
  undefined8 local_58;
  long lStack_50;
  long local_48;
  undefined8 local_40;
  long lStack_38;
  
  if (((*(long *)(param_1 + 0x68) != 0) && (lVar1 = FUN_014baf1c(), lVar1 != 0)) &&
     (lVar1 = FUN_0194a118(lVar1 + 8,param_2), lVar1 != 0)) {
    uVar4 = *(undefined8 *)(param_1 + 0x68);
    uVar2 = FUN_00f22bd0();
    lVar1 = FUN_0009278c(uVar2,uVar4,1);
    if (lVar1 != 0) {
      local_60 = 0;
      local_58 = 0;
      lStack_50 = 0;
      lVar3 = (long)*(char *)((long)param_3 + 0x17);
      puVar5 = param_3;
      if (lVar3 < 0) {
        puVar5 = (undefined8 *)*param_3;
        lVar3 = param_3[1];
      }
      FUN_00acf100(&local_60,puVar5,lVar3);
      lVar3 = FUN_00008790(0x100);
      lStack_38 = lStack_50;
      local_40 = local_58;
      local_48 = local_60;
      local_60 = 0;
      local_58 = 0;
      lStack_50 = 0;
      FUN_014ba538(lVar3,0x3f,"commands.onCommand",0x12,&local_48,*(undefined8 *)(param_1 + 0x68),0)
      ;
      if (local_48 != 0) {
        FUN_00996624(&local_48);
        FUN_00005810(local_48,lStack_38 << 5);
      }
      *(undefined4 *)(lVar3 + 0xd0) = 1;
      FUN_0278a1f8(lVar1,param_2,lVar3);
      if (local_60 != 0) {
        FUN_00996624(&local_60);
        FUN_00005810(local_60,lStack_50 << 5);
      }
    }
  }
  return;
}


