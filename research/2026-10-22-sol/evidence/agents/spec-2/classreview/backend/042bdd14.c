// ==== 0x42bdd14  FUN_042bdd14  (240 bytes)

void FUN_042bdd14(undefined8 param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  undefined4 *puVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  uVar1 = *param_2;
  uVar2 = param_2[1];
  puVar5 = (undefined8 *)param_2[3];
  uVar7 = param_2[3];
  uVar6 = param_2[2];
  if (puVar5[1] != 0) {
    param_2[2] = 0;
    param_2[3] = &DAT_0c4b3500;
  }
  puVar3 = (undefined8 *)FUN_00008790(0x20);
  *puVar3 = uVar1;
  puVar3[1] = uVar2;
  puVar3[3] = uVar7;
  puVar3[2] = uVar6;
  if (puVar5[1] != 0) {
    puVar5 = &DAT_0c4b3500;
  }
  puVar4 = (undefined4 *)FUN_00008790(0x48);
  *puVar4 = 1;
  *(undefined1 **)(puVar4 + 2) = &LAB_042ae004;
  *(code **)(puVar4 + 4) = FUN_041a82e0;
  *(code **)(puVar4 + 6) = FUN_00b77080;
  *(undefined8 **)(puVar4 + 10) = puVar3;
  *(undefined1 **)(puVar4 + 0xe) = &LAB_042c2c24;
  *(undefined ***)(puVar4 + 0x10) = &PTR_FUN_0df6a788;
  FUN_02e7bb1c(0,puVar4);
  if ((code *)puVar5[1] != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x042bdde8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)puVar5[1])(uVar1);
    return;
  }
  return;
}


