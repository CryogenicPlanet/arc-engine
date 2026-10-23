// ==== 0x4042a20  FUN_04042a20  (52 bytes)

void FUN_04042a20(long param_1)

{
  code *pcVar1;
  
  if (param_1 != -0x18) {
    _objc_destroyWeak(param_1 + 0x28);
    _objc_destroyWeak(param_1 + 0x20);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4042a54);
  (*pcVar1)();
}


