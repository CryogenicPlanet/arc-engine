// ==== 0x4443ae4  ArcWebContents::activateTab:  (92 bytes)

/* Function Stack Size: 0x18 bytes */

bool ArcWebContents::activateTab_(ID param_1,SEL param_2,ID *param_3)

{
  code *pcVar1;
  bool bVar2;
  ulong local_28;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_0437a510(&local_28,*(undefined8 *)(param_1 + 0x118));
    bVar2 = FUN_04402548(&local_28,param_3);
    if ((local_28 & 1) == 0) {
      FUN_04b35898();
    }
    return bVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4443b40);
  (*pcVar1)();
}


