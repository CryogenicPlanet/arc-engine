// ==== 0x5533b24  FUN_05533b24  (108 bytes)

undefined8 * FUN_05533b24(undefined8 *param_1,undefined8 *param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  
  *(undefined1 *)param_1 = 0;
  *(undefined1 *)(param_1 + 3) = 0;
  if (*(char *)(param_2 + 3) != '\0') {
    if (param_1 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x5533b90);
      (*pcVar1)();
    }
    if (*(char *)((long)param_2 + 0x17) < '\0') {
      FUN_00060a60(param_1,*param_2,param_2[1]);
    }
    else {
      uVar3 = param_2[1];
      uVar2 = *param_2;
      param_1[2] = param_2[2];
      param_1[1] = uVar3;
      *param_1 = uVar2;
    }
    *(undefined1 *)(param_1 + 3) = 1;
  }
  return param_1;
}


