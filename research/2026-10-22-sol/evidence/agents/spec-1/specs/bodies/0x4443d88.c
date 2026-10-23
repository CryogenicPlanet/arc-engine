// ==== 0x4443d88  ArcWebContents::setIsBlockingJavascriptPopups:  (24 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContents::setIsBlockingJavascriptPopups_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    *(char *)(*(long *)(param_1 + 0x118) + 0x269) = (char)param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4443da0);
  (*pcVar1)();
}


