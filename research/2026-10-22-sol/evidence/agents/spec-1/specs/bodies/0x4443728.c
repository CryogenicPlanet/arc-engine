// ==== 0x4443728  ArcWebContents::showBlockedPopups  (160 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::showBlockedPopups(ID param_1,SEL param_2)

{
  code *pcVar1;
  ulong uVar2;
  char *pcVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  long lVar6;
  
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x44437c8);
    (*pcVar1)();
  }
  lVar6 = *(long *)(*(long *)(param_1 + 0x118) + 0x130);
  if (lVar6 == 0) {
    pcVar3 = "Attempted to show blocked popups for nullptr WebContents.";
    uVar4 = 0x39;
    uVar5 = 0x966;
  }
  else {
    lVar6 = FUN_000805e0(lVar6 + 8,&DAT_0c7dfab8);
    if (lVar6 != 0 && lVar6 != 0x20) {
      FUN_07277188();
      return;
    }
    pcVar3 = "Attempted to show blocked popups for page without popup blocker.";
    uVar4 = 0x40;
    uVar5 = 0x96d;
  }
  uVar2 = FUN_04b37104(0x25,pcVar3,uVar4,uVar5,"../../arc/browser/web_contents/web_contents_impl.cc"
                      );
  if ((uVar2 & 1) != 0) {
    return;
  }
  FUN_04b35898();
  return;
}


