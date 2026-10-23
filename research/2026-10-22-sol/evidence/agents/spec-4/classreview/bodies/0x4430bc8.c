// ==== 0x4430bc8  ArcContentNode::textSize  (20 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcContentNode::textSize(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x1b8) & 1) != 0) {
    return (long)*(int *)(param_1 + 0x5c);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430bdc);
  (*pcVar1)();
}


