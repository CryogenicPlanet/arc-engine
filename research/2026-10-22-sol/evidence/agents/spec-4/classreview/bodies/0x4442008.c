// ==== 0x4442008  ArcWebContents::stopSharingThisTab  (36 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::stopSharingThisTab(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x444202c);
    (*pcVar1)();
  }
  lVar2 = *(long *)(*(long *)(param_1 + 0x118) + 0x600);
  if (lVar2 != 0) {
    FUN_04229eac(lVar2 + -8);
    return;
  }
  return;
}


