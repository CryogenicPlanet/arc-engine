// ==== 0x443f020  ArcLoginCredentialsSaveRequest::isUpdate  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcLoginCredentialsSaveRequest::isUpdate(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 8) + 0x2c);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443f038);
  (*pcVar1)();
}


