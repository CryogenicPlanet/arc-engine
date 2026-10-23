// ==== 0x4403cb8  ArcHungRendererDialog::wait  (20 bytes)

/* Function Stack Size: 0x10 bytes */

pid_t ArcHungRendererDialog::wait(int *param_1)

{
  code *pcVar1;
  pid_t pVar2;
  
  if ((*(byte *)(param_1 + 6) & 1) != 0) {
    pVar2 = FUN_0435f1ac(*(undefined8 *)(param_1 + 2));
    return pVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4403ccc);
  (*pcVar1)();
}


