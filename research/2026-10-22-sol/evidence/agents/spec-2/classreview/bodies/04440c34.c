// ==== 0x4440c34  ArcWebContentsAIAgent::sendKeyPress:  (24 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcWebContentsAIAgent::sendKeyPress_(ID param_1,SEL param_2,long_long param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_04354ea0(*(undefined8 *)(param_1 + 8),param_3);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4440c4c);
  (*pcVar1)();
}


