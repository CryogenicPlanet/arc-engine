// ==== 0x4441e44  ArcWebContents::setSafeAreaInsets:left:bottom:right:  (20 bytes)

/* Function Stack Size: 0x20 bytes */

void ArcWebContents::setSafeAreaInsets_left_bottom_right_
               (ID param_1,SEL param_2,float param_3,float param_4,float param_5,float param_6)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_04377750(*(undefined8 *)(param_1 + 0x118));
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4441e58);
  (*pcVar1)();
}


