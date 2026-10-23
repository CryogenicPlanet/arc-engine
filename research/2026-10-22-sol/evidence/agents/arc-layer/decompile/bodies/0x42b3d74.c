// ==== 0x42b3d74  FUN_042b3d74  (440 bytes)

void FUN_042b3d74(undefined8 *param_1,long param_2,undefined4 param_3,undefined8 param_4,
                 undefined8 *param_5,undefined8 *param_6)

{
  ulong uVar1;
  byte bVar2;
  code *pcVar3;
  int iVar4;
  undefined4 *puVar5;
  char *pcVar6;
  undefined8 uVar7;
  undefined8 *puVar8;
  int *piVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 local_68 [2];
  undefined7 local_58;
  char cStack_51;
  
  bVar2 = *(byte *)((long)param_5 + 0x17);
  uVar1 = param_5[1];
  if (-1 < (char)bVar2) {
    uVar1 = (ulong)bVar2;
  }
  if (uVar1 == 0) {
    pcVar6 = "profile name cannot be empty";
    uVar10 = 0x1c;
    uVar7 = 0x213;
  }
  else {
    if (*(char *)(param_6[3] + 0x10) == '\0') {
      puVar8 = (undefined8 *)*param_5;
      if (-1 < (char)bVar2) {
        puVar8 = param_5;
      }
      FUN_007cf8e0(local_68,puVar8);
      piVar9 = *(int **)(param_2 + 0x468);
      if ((piVar9 != (int *)0x0) && (iVar4 = *piVar9, *piVar9 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
        pcVar3 = (code *)SoftwareBreakpoint(0,0x42b3f2c);
        (*pcVar3)();
      }
      uVar10 = *(undefined8 *)(param_2 + 0x470);
      puVar5 = (undefined4 *)FUN_00008790(0x80);
      *puVar5 = 1;
      *(undefined1 **)(puVar5 + 2) = &LAB_042bf4cc;
      *(code **)(puVar5 + 4) = FUN_042bf5ac;
      *(undefined1 **)(puVar5 + 6) = &LAB_04169bc0;
      *(code **)(puVar5 + 8) = FUN_042b3f34;
      *(undefined8 *)(puVar5 + 10) = 0;
      *(int **)(puVar5 + 0xc) = piVar9;
      *(undefined8 *)(puVar5 + 0xe) = uVar10;
      puVar5[0x10] = param_3;
      if (*(char *)((long)param_5 + 0x17) < '\0') {
        FUN_00060a60(puVar5 + 0x12,*param_5,param_5[1]);
      }
      else {
        uVar10 = *param_5;
        *(undefined8 *)(puVar5 + 0x14) = param_5[1];
        *(undefined8 *)(puVar5 + 0x12) = uVar10;
        *(undefined8 *)(puVar5 + 0x16) = param_5[2];
      }
      puVar8 = (undefined8 *)param_6[3];
      uVar10 = *param_6;
      uVar11 = param_6[3];
      uVar7 = param_6[2];
      *(undefined8 *)(puVar5 + 0x1a) = param_6[1];
      *(undefined8 *)(puVar5 + 0x18) = uVar10;
      *(undefined8 *)(puVar5 + 0x1e) = uVar11;
      *(undefined8 *)(puVar5 + 0x1c) = uVar7;
      if ((code *)*puVar8 != (code *)0x0) {
        uVar10 = (*(code *)*puVar8)(*param_6);
        *(undefined8 *)(puVar5 + 0x18) = uVar10;
      }
      FUN_07bdc470(local_68,0,param_4,puVar5,0);
      if (cStack_51 < '\0') {
        FUN_00005810(local_68[0],CONCAT17(cStack_51,local_58) << 1);
      }
      uVar10 = 1;
      goto LAB_042b3eb4;
    }
    pcVar6 = "callback is null";
    uVar10 = 0x10;
    uVar7 = 0x218;
  }
  uVar10 = FUN_04b37104(0xd,pcVar6,uVar10,uVar7,"../../arc/browser/browser_application_impl.cc");
LAB_042b3eb4:
  *param_1 = uVar10;
  return;
}


