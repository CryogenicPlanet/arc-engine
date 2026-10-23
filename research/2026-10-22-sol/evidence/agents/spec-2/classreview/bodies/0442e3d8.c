// ==== 0x442e3d8  ArcHistoryItem::visitCount  (24 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcHistoryItem::visitCount(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return *(int *)(*(long *)(param_1 + 8) + 0xa0);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442e3f0);
  (*pcVar1)();
}


