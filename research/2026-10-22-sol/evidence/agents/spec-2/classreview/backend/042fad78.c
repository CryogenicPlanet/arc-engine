// ==== 0x42fad78  FUN_042fad78  (120 bytes)

undefined1 FUN_042fad78(long param_1)

{
  code *pcVar1;
  uint uVar2;
  
  if (((*(long *)(param_1 + 0xf0) == 0) || (*(char *)(*(long *)(param_1 + 0xf0) + 4) != '\0')) ||
     (*(long *)(param_1 + 0xf8) == 0)) {
    return 0;
  }
  if ((*(long *)(param_1 + 0xf0) == 0) || (*(char *)(*(long *)(param_1 + 0xf0) + 4) != '\0')) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x42fadec);
    (*pcVar1)();
  }
  uVar2 = (**(code **)(**(long **)(param_1 + 0xf8) + 0x60))();
  if ((uVar2 < 6) && ((0x33U >> (ulong)(uVar2 & 0x1f) & 1) != 0)) {
    return (&DAT_0c9d683c)[uVar2];
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0,0x42fadf8);
  (*pcVar1)();
}


