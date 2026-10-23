// ==== 0x4442340  ArcWebContents::willRunUnloadEvents  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContents::willRunUnloadEvents(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    bVar2 = FUN_04378aa0(*(undefined8 *)(param_1 + 0x118));
    return bVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4442354);
  (*pcVar1)();
}


