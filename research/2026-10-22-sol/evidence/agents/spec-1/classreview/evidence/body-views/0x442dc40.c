// ==== 0x442dc40  ArcSiteEngagementDetails::score  (20 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcSiteEngagementDetails::score(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return *(double *)(param_1 + 0x20);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442dc54);
  (*pcVar1)();
}


