// ==== 0x440a0fc  ArcNotification::didClose:  (24 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcNotification::didClose_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    FUN_0433edd0(param_1 + 0x10,param_3);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440a114);
  (*pcVar1)();
}


