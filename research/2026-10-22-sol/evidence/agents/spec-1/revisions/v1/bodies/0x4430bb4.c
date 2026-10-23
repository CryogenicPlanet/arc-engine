// ==== 0x4430bb4  ArcContentNode::hasEmphasis  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcContentNode::hasEmphasis(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x1b8) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0x58);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430bc8);
  (*pcVar1)();
}


