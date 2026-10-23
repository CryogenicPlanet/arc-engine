// ==== 0x4403ca4  ArcHungRendererDialog::crashProcess  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcHungRendererDialog::crashProcess(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_0435f150(*(undefined8 *)(param_1 + 8));
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4403cb8);
  (*pcVar1)();
}


