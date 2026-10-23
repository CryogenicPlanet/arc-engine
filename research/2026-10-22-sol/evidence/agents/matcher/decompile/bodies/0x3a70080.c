// ==== 0x3a70080  FUN_03a70080  (344 bytes)

void FUN_03a70080(undefined8 param_1,int param_2,undefined1 param_3,undefined8 param_4)

{
  int iVar1;
  undefined8 *puVar2;
  undefined4 *puVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  long lVar6;
  undefined1 auStack_70 [8];
  undefined4 local_68;
  int local_64;
  undefined1 auStack_60 [32];
  
  local_68 = 0;
  local_64 = param_2;
  if (param_2 == 0) {
    uVar4 = FUN_08376b28(&local_64,&LAB_063855e0);
    uVar5 = FUN_08376b28(&local_68,&LAB_063855e0);
    uVar4 = FUN_08376c54("kind != GPU_PROCESS_KIND_INFO_COLLECTION",uVar4,uVar5);
    FUN_0837a430(auStack_60,"../../content/browser/gpu/gpu_process_host.cc",0x2c0);
    lVar6 = FUN_08375eb0(uVar4,0x9f,auStack_60);
    if (lVar6 != 0) {
      FUN_026df440(auStack_70,lVar6);
      FUN_0837612c(auStack_70);
    }
  }
  puVar2 = (undefined8 *)FUN_0049fafc(&DAT_0c76648c);
  puVar3 = (undefined4 *)FUN_00008790(0x38);
  *puVar3 = 1;
  *(undefined1 **)(puVar3 + 2) = &LAB_02c4a950;
  *(code **)(puVar3 + 4) = FUN_03a70440;
  *(code **)(puVar3 + 6) = FUN_00b77080;
  *(code **)(puVar3 + 8) = FUN_02c4a970;
  puVar3[10] = local_64;
  *(undefined1 *)(puVar3 + 0xb) = param_3;
  *(undefined8 *)(puVar3 + 0xc) = param_4;
  (**(code **)*puVar2)(puVar2,param_1,puVar3,0);
  if (puVar2 != (undefined8 *)0x0) {
    LOAcquire();
    iVar1 = *(int *)(puVar2 + 1);
    *(int *)(puVar2 + 1) = iVar1 + -1;
    LORelease();
    if (iVar1 == 1) {
      FUN_030607e0(puVar2);
    }
  }
  return;
}


