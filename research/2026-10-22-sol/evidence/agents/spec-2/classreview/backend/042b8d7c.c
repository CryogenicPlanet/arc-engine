// ==== 0x42b8d7c  FUN_042b8d7c  (228 bytes)

void FUN_042b8d7c(undefined8 param_1,undefined8 *param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined1 auStack_50 [32];
  
  FUN_00029e40(auStack_50,"AvailableImportSources","../../arc/browser/browser_application_impl.cc",
               0x442);
  puVar1 = (undefined4 *)FUN_00008790(0x30);
  *puVar1 = 1;
  *(code **)(puVar1 + 2) = FUN_020fa180;
  *(code **)(puVar1 + 4) = FUN_03955000;
  *(code **)(puVar1 + 6) = FUN_00b77080;
  *(code **)(puVar1 + 8) = FUN_04320d04;
  puVar2 = (undefined4 *)FUN_00008790(0x48);
  *puVar2 = 1;
  *(undefined1 **)(puVar2 + 2) = &LAB_042bfbf0;
  *(code **)(puVar2 + 4) = FUN_042a34c0;
  *(code **)(puVar2 + 6) = FUN_00b77080;
  uVar4 = *param_2;
  uVar6 = param_2[3];
  uVar5 = param_2[2];
  *(undefined8 *)(puVar2 + 0xc) = param_2[1];
  *(undefined8 *)(puVar2 + 10) = uVar4;
  lVar3 = param_2[3];
  *(undefined8 *)(puVar2 + 0x10) = uVar6;
  *(undefined8 *)(puVar2 + 0xe) = uVar5;
  if (*(long *)(lVar3 + 8) != 0) {
    param_2[2] = 0;
    param_2[3] = &DAT_0c4b3500;
  }
  FUN_042b8e60(auStack_50,&DAT_0c9d66b8,puVar1,puVar2);
  return;
}


