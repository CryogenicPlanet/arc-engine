// ==== 0x4443db8  ArcWebContents::disallowDownloads  (76 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContents::disallowDownloads(ID param_1,SEL param_2)

{
  code *pcVar1;
  uint uVar2;
  long lVar3;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    lVar3 = *(long *)(*(long *)(param_1 + 0x118) + 0x130);
    uVar2 = 0;
    if (lVar3 != 0) {
      lVar3 = FUN_000805e0(lVar3 + 8,&DAT_0c6ca550);
      if (lVar3 == 0) {
        uVar2 = 0;
      }
      else {
        uVar2 = (uint)*(byte *)(lVar3 + 0x2a);
      }
    }
    return uVar2 & 1;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4443e04);
  (*pcVar1)();
}


