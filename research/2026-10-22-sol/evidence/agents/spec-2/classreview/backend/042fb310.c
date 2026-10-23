// ==== 0x42fb310  FUN_042fb310  (88 bytes)

undefined4 FUN_042fb310(long param_1)

{
  code *pcVar1;
  undefined4 *puVar2;
  
  if (((*(long *)(param_1 + 0xf0) != 0) && (*(char *)(*(long *)(param_1 + 0xf0) + 4) == '\0')) &&
     (*(long *)(param_1 + 0xf8) != 0)) {
    if ((*(long *)(param_1 + 0xf0) != 0) && (*(char *)(*(long *)(param_1 + 0xf0) + 4) == '\0')) {
      puVar2 = (undefined4 *)(**(code **)(**(long **)(param_1 + 0xf8) + 0x48))();
      return *puVar2;
    }
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x42fb368);
    (*pcVar1)();
  }
  return 0xffffffff;
}


