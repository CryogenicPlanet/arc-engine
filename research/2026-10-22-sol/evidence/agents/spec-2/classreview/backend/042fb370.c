// ==== 0x42fb370  FUN_042fb370  (108 bytes)

undefined8 FUN_042fb370(long param_1)

{
  code *pcVar1;
  long *plVar2;
  undefined8 uVar3;
  
  if (((*(long *)(param_1 + 0xf0) != 0) && (*(char *)(*(long *)(param_1 + 0xf0) + 4) == '\0')) &&
     (*(long *)(param_1 + 0xf8) != 0)) {
    if ((*(long *)(param_1 + 0xf0) != 0) && (*(char *)(*(long *)(param_1 + 0xf0) + 4) == '\0')) {
      plVar2 = (long *)(**(code **)(**(long **)(param_1 + 0xf8) + 0x38))();
      uVar3 = (**(code **)(*plVar2 + 0x168))();
      uVar3 = FUN_003bc2f4(uVar3,"browser.allow_javascript_apple_events",0x25);
      return uVar3;
    }
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x42fb3dc);
    (*pcVar1)();
  }
  return 0;
}


