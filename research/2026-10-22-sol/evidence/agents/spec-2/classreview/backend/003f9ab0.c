// ==== 0x3f9ab0  FUN_003f9ab0  (148 bytes)

void FUN_003f9ab0(undefined8 param_1,undefined8 *param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  
  while( true ) {
    if (param_2 == (undefined8 *)0x0) {
      return;
    }
    FUN_003f9ab0(param_1,*param_2);
    if (param_2 == (undefined8 *)0xffffffffffffffe0) break;
    puVar2 = (undefined8 *)param_2[1];
    if (*(char *)((long)param_2 + 0x4f) < '\0') {
      FUN_00005810(param_2[7],param_2[9] & 0x7fffffffffffffff);
    }
    if (*(char *)((long)param_2 + 0x37) < '\0') {
      FUN_00005810(param_2[4],param_2[6] & 0x7fffffffffffffff);
    }
    FUN_00005810(param_2,0x50);
    param_2 = puVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x3f9b44);
  (*pcVar1)();
}


