// ==== 0x43770c0  FUN_043770c0  (316 bytes)

void FUN_043770c0(long param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 param_5,undefined8 *param_6)

{
  undefined8 uVar1;
  long lVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  long lVar5;
  code *pcVar6;
  undefined8 *puVar7;
  undefined8 uVar8;
  undefined8 local_70 [2];
  undefined7 local_60;
  char cStack_59;
  undefined8 local_58;
  
  if (*(long *)(param_1 + 0x130) == 0) {
    return;
  }
  lVar2 = FUN_000805e0(*(long *)(param_1 + 0x130) + 8,&DAT_0cf208a0);
  if (lVar2 == 0) {
    return;
  }
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    if (param_2[1] == 0) goto LAB_043771bc;
  }
  else if (*(char *)((long)param_2 + 0x17) == '\0') {
LAB_043771bc:
    FUN_0c1fde60(lVar2,1);
    local_58 = 0xffffffff00000000;
    (*(code *)param_6[2])(param_6,&local_58);
    return;
  }
  uVar3 = *param_6;
  uVar1 = param_6[1];
  uVar8 = param_6[3];
  uVar4 = param_6[2];
  if (*(code **)param_6[3] != (code *)0x0) {
    uVar3 = (**(code **)param_6[3])();
  }
  lVar5 = *(long *)(param_1 + 0x150);
  *(undefined8 *)(param_1 + 0x150) = uVar8;
  *(undefined8 *)(param_1 + 0x148) = uVar4;
  uVar4 = *(undefined8 *)(param_1 + 0x138);
  *(undefined8 *)(param_1 + 0x138) = uVar3;
  *(undefined8 *)(param_1 + 0x140) = uVar1;
  pcVar6 = *(code **)(lVar5 + 8);
  if (pcVar6 != (code *)0x0) {
    (*pcVar6)(uVar4);
  }
  lVar5 = (long)*(char *)((long)param_2 + 0x17);
  puVar7 = param_2;
  if (lVar5 < 0) {
    puVar7 = (undefined8 *)*param_2;
    lVar5 = param_2[1];
  }
  FUN_007cf8e0(local_70,puVar7,lVar5);
  FUN_0c1fdb5c(lVar2,local_70,param_3,param_4,param_5,0);
  if (cStack_59 < '\0') {
    FUN_00005810(local_70[0],CONCAT17(cStack_59,local_60) << 1);
  }
  return;
}


