// ==== 0x443be7c  ArcExternalProtocolDialog::close  (24 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcExternalProtocolDialog::close(int param_1)

{
  code *pcVar1;
  int iVar2;
  ulong uVar3;
  
  uVar3 = (ulong)(uint)param_1;
  if ((*(byte *)(uVar3 + 0x98) & 1) != 0) {
                    /* WARNING: Could not recover jumptable at 0x0443be8c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar2 = (**(code **)(uVar3 + 0x88))(uVar3 + 0x78);
    return iVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443be94);
  (*pcVar1)();
}


