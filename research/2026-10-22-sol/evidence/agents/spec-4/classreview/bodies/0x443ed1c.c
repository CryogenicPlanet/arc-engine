// ==== 0x443ed1c  ArcAddressEntrySaveRequest::cancel  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAddressEntrySaveRequest::cancel(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_04330d38(param_1 + 8);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443ed30);
  (*pcVar1)();
}


