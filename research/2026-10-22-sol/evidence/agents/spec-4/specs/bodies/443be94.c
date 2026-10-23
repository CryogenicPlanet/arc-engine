// ==== 0x443be94  ArcExternalProtocolDialog::accept:  (28 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcExternalProtocolDialog::accept_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x98) & 1) != 0) {
                    /* WARNING: Could not recover jumptable at 0x0443bea8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(param_1 + 0x48))(param_1 + 0x38,param_3);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443beb0);
  (*pcVar1)();
}


