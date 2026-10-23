// ==== 0x4332684  FUN_04332684  (28 bytes)

undefined1 FUN_04332684(uint param_1)

{
  code *pcVar1;
  
  if (param_1 < 0x25) {
    return (&DAT_0c9d7034)[param_1];
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43326a0);
  (*pcVar1)();
}


