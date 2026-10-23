// ==== 0x42fa9fc  FUN_042fa9fc  (128 bytes)

void FUN_042fa9fc(long param_1,undefined8 *param_2)

{
  code *pcVar1;
  long *plVar2;
  long lVar3;
  undefined8 local_28;
  
  if ((*(long *)(param_1 + 0xf0) != 0) && (*(char *)(*(long *)(param_1 + 0xf0) + 4) == '\0')) {
    plVar2 = (long *)(**(code **)(**(long **)(param_1 + 0xf8) + 0x28))();
    if ((plVar2 != (long *)0x0) && (lVar3 = (**(code **)(*plVar2 + 0x28))(), lVar3 != 0)) {
      local_28 = *param_2;
      _os_retain();
      FUN_043c8b18(lVar3,&local_28);
      _os_release(local_28);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0,0x42faa7c);
  (*pcVar1)();
}


