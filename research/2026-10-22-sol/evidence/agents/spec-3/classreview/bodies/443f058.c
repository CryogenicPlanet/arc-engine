// ==== 0x443f058  ArcLoginCredentialsSaveRequest::cancel  (40 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcLoginCredentialsSaveRequest::cancel(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443f080);
    (*pcVar1)();
  }
  if ((*(long *)(*(long *)(param_1 + 8) + 0x30) != 0) &&
     (*(char *)(*(long *)(param_1 + 8) + 0x2c) != '\0')) {
    FUN_07d641d0();
    return;
  }
  return;
}


