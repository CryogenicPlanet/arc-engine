// ==== 0x4443e04  ArcWebContents::setDisallowDownloads:  (76 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContents::setDisallowDownloads_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  long lVar2;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    lVar2 = *(long *)(*(long *)(param_1 + 0x118) + 0x130);
    if ((lVar2 != 0) && (lVar2 = FUN_000805e0(lVar2 + 8,&DAT_0c6ca550), lVar2 != 0)) {
      *(char *)(lVar2 + 0x2a) = (char)param_3;
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4443e50);
  (*pcVar1)();
}


