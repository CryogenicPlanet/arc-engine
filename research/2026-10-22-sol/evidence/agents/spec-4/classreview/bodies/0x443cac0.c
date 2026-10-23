// ==== 0x443cac0  ArcSessionNavigation::hasPostData  (28 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcSessionNavigation::hasPostData(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(**(long **)(param_1 + 8) + 0x134);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443cadc);
  (*pcVar1)();
}


