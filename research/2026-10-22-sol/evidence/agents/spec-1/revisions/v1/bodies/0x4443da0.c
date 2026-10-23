// ==== 0x4443da0  ArcWebContents::javascriptPopupsDisplayedCount  (24 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcWebContents::javascriptPopupsDisplayedCount(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    return *(int *)(*(long *)(param_1 + 0x118) + 0x26c);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4443db8);
  (*pcVar1)();
}


