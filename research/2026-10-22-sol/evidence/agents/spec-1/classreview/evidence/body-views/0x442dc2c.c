// ==== 0x442dc2c  ArcSiteEngagementDetails::url  (20 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcSiteEngagementDetails::url(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    IVar2 = FUN_043abd34(param_1 + 8);
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442dc40);
  (*pcVar1)();
}


