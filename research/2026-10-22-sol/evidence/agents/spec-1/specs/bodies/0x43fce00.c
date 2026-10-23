// ==== 0x43fce00  ArcBrowserContextExtensions::isExtensionSystemReady  (92 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBrowserContextExtensions::isExtensionSystemReady(ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  uint uVar3;
  long lVar4;
  
  if ((*(byte *)(param_1 + 0x30) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x43fce5c);
    (*pcVar1)();
  }
  lVar4 = *(long *)(*(long *)(param_1 + 0x20) + 0x68);
  if (lVar4 != 0) {
    FUN_0084f5ac();
    uVar2 = FUN_0084f5ac();
    lVar4 = FUN_0009278c(uVar2,lVar4,1);
    if (lVar4 != 0) {
      uVar3 = (uint)*(byte *)(*(long *)(lVar4 + 0x10) + 0x78);
      goto LAB_043fce48;
    }
  }
  uVar3 = 0;
LAB_043fce48:
  return uVar3 & 1;
}


