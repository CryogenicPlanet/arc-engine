// ==== 0x4442a64  ArcWebContents::consumeExternalProtocolLaunchPermission  (40 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContents::consumeExternalProtocolLaunchPermission(ID param_1,SEL param_2)

{
  code *pcVar1;
  uint uVar2;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    uVar2 = (uint)DAT_0e4a0c58;
    if ((DAT_0e4a0c58 & 1) == 0) {
      DAT_0e4a0c58 = 1;
    }
    return uVar2 ^ 1;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4442a8c);
  (*pcVar1)();
}


