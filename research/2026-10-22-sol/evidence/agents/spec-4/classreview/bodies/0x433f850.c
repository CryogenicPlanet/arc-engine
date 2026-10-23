// ==== 0x433f850  FUN_0433f850  (28 bytes)

undefined1 FUN_0433f850(uint param_1)

{
  code *pcVar1;
  
  if (param_1 < 0x1c) {
    return (&DAT_0c9d7090)[param_1];
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x433f86c);
  (*pcVar1)();
}


