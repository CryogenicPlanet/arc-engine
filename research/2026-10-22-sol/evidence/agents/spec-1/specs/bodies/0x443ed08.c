// ==== 0x443ed08  ArcAddressEntrySaveRequest::save  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAddressEntrySaveRequest::save(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_04330cdc(param_1 + 8);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443ed1c);
  (*pcVar1)();
}


