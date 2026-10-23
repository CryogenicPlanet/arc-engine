// ==== 0x4442a8c  ArcWebContents::exitFullscreen  (44 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::exitFullscreen(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4442ab8);
    (*pcVar1)();
  }
  lVar2 = *(long *)(param_1 + 0x118);
  *(undefined1 *)(lVar2 + 0x158) = 1;
  lVar2 = *(long *)(lVar2 + 0x130);
  if (lVar2 == 0) {
    return;
  }
  FUN_06798c84(lVar2,1);
  return;
}


