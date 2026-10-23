// ==== 0x4442e7c  ArcWebContents::print  (60 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::print(ID param_1,SEL param_2)

{
  code *pcVar1;
  ulong local_18;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_043778a0(&local_18,*(undefined8 *)(param_1 + 0x118));
    if ((local_18 & 1) == 0) {
      FUN_04b35898();
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4442eb8);
  (*pcVar1)();
}


