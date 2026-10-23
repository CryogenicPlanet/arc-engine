// ==== 0x443f080  ArcLoginCredentialsSaveRequest::neverAskAgain  (32 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcLoginCredentialsSaveRequest::neverAskAgain(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443f0a0);
    (*pcVar1)();
  }
  if (*(long *)(*(long *)(param_1 + 8) + 0x30) != 0) {
    FUN_07d64b14();
    return;
  }
  return;
}


