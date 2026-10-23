// ==== 0x4441f14  ArcWebContents::revertTranslation  (60 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::revertTranslation(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    lVar2 = FUN_000805e0(*(long *)(*(long *)(param_1 + 0x118) + 0x130) + 8,&DAT_0c9d7100);
    FUN_03b1c238(*(undefined8 *)(lVar2 + 8));
    FUN_09f2536c();
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4441f50);
  (*pcVar1)();
}


