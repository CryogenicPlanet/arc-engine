// ==== 0x44010ec  ArcBrowserCreateParams::initialBounds  (44 bytes)

/* Function Stack Size: 0x10 bytes */

CGRect ArcBrowserCreateParams::initialBounds(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  CGRect CVar3;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar2 = *(long *)(param_1 + 8);
    CVar3.field0_0x0.field0_0x0 = (double)*(int *)(lVar2 + 0x20);
    CVar3.field0_0x0.field1_0x8 = (double)*(int *)(lVar2 + 0x24);
    CVar3.field1_0x10.field0_0x0 = (double)*(int *)(lVar2 + 0x28);
    CVar3.field1_0x10.field1_0x8 = (double)*(int *)(lVar2 + 0x2c);
    return CVar3;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4401118);
  (*pcVar1)();
}


