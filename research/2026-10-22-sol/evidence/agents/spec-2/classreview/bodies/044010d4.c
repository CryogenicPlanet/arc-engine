// ==== 0x44010d4  ArcBrowserCreateParams::canFullscreen  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBrowserCreateParams::canFullscreen(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 8) + 0xaa);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44010ec);
  (*pcVar1)();
}


