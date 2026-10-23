// ==== 0x441cf20  ArcWebDialog::preferredSize  (16 bytes)

/* Function Stack Size: 0x10 bytes */

CGSize ArcWebDialog::preferredSize(ID param_1,SEL param_2)

{
  CGSize CVar1;
  
  CVar1.field0_0x0 = (double)*(int *)(param_1 + 0x20);
  CVar1.field1_0x8 = (double)*(int *)(param_1 + 0x24);
  return CVar1;
}


