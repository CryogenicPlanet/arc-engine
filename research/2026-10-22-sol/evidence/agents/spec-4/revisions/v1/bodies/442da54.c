// ==== 0x442da54  ArcMostVisitedURL::url  (20 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcMostVisitedURL::url(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    IVar2 = FUN_043abd34(param_1 + 8);
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442da68);
  (*pcVar1)();
}


