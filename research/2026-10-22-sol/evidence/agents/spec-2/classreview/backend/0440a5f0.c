// ==== 0x440a5f0  FUN_0440a5f0  (176 bytes)

void FUN_0440a5f0(long *param_1,undefined8 *param_2,undefined8 *param_3)

{
  code *pcVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  
  puVar2 = (undefined8 *)(*param_1 + param_1[1] * 0x30);
  if (puVar2 != (undefined8 *)0x0) {
    if (*(char *)((long)param_2 + 0x17) < '\0') {
      FUN_00054420(puVar2,*param_2,param_2[1]);
    }
    else {
      uVar4 = param_2[1];
      uVar3 = *param_2;
      puVar2[2] = param_2[2];
      puVar2[1] = uVar4;
      *puVar2 = uVar3;
    }
    if (*(char *)((long)param_3 + 0x17) < '\0') {
      FUN_00054420(puVar2 + 3,*param_3,param_3[1]);
    }
    else {
      uVar4 = param_3[1];
      uVar3 = *param_3;
      puVar2[5] = param_3[2];
      puVar2[4] = uVar4;
      puVar2[3] = uVar3;
    }
    param_1[1] = ((long)puVar2 + (0x30 - *param_1) >> 4) * -0x5555555555555555;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440a6a0);
  (*pcVar1)();
}


