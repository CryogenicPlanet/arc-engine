// ==== 0x442eb38  ArcExtractedPageContent::extractionTime  (20 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcExtractedPageContent::extractionTime(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x1e8) & 1) != 0) {
    return *(double *)(param_1 + 0x40);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442eb4c);
  (*pcVar1)();
}


