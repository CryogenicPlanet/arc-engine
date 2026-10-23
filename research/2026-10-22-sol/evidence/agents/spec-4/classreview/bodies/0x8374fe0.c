// ==== 0x8374fe0  ArcWebContents::save  (84 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::save(ID param_1,SEL param_2)

{
  code *pcVar1;
  ulong uVar2;
  
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x8375034);
    (*pcVar1)();
  }
  if (*(long *)(*(long *)(param_1 + 0x118) + 0x130) != 0) {
    FUN_06796e50();
    return;
  }
  uVar2 = FUN_04b37104(0x25,"managed webcontents is null",0x1b,0x720,
                       "../../arc/browser/web_contents/web_contents_impl.cc");
  if ((uVar2 & 1) != 0) {
    return;
  }
  FUN_04b35898();
  return;
}


