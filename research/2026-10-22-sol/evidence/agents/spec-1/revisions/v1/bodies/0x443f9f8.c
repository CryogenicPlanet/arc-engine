// ==== 0x443f9f8  ArcPermissionRequest::type  (24 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcPermissionRequest::type(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (long)*(int *)(*(long *)(param_1 + 0x10) + 0x18);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443fa10);
  (*pcVar1)();
}


