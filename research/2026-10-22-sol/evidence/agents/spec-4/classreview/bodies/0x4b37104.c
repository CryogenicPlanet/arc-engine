// ==== 0x4b37104  FUN_04b37104  (196 bytes)

undefined4 *
FUN_04b37104(undefined4 *param_1,void *param_2,size_t param_3,undefined8 param_4,char *param_5)

{
  code *pcVar1;
  undefined4 *puVar2;
  void *pvVar3;
  
  puVar2 = param_1;
  if (param_3 != 0 && param_1 != (undefined4 *)((long)&MACH_HEADER.magic + 1)) {
    puVar2 = (undefined4 *)FUN_00008790(0x40);
    *puVar2 = 1;
    puVar2[1] = (int)((ulong)param_1 >> 2);
    pvVar3 = (void *)FUN_031cc1f4(puVar2 + 2,param_3);
    if (((void *)((long)pvVar3 + param_3) < pvVar3) ||
       (param_2 < (void *)((long)pvVar3 + param_3) && pvVar3 <= param_2)) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4b371c8);
      (*pcVar1)();
    }
    _memmove(pvVar3,param_2,param_3);
    *(undefined1 *)((long)pvVar3 + param_3) = 0;
    *(undefined8 *)(puVar2 + 8) = 0;
    *(undefined8 *)(puVar2 + 0xe) = 0;
    if (*param_5 != '\0') {
      FUN_04b35cd0(puVar2,param_4,param_5);
    }
  }
  return puVar2;
}


