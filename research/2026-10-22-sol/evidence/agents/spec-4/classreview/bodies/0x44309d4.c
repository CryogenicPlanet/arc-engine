// ==== 0x44309d4  ArcContentNode::type  (20 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcContentNode::type(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x1b8) & 1) != 0) {
    return (long)*(int *)(param_1 + 0x18);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44309e8);
  (*pcVar1)();
}


