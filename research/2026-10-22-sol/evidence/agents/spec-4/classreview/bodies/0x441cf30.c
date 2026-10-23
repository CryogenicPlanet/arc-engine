// ==== 0x441cf30  ArcWebDialog::minimumSize  (16 bytes)

/* Function Stack Size: 0x10 bytes */

CGSize ArcWebDialog::minimumSize(ID param_1,SEL param_2)

{
  CGSize CVar1;
  
  CVar1.field0_0x0 = (double)*(int *)(param_1 + 0x28);
  CVar1.field1_0x8 = (double)*(int *)(param_1 + 0x2c);
  return CVar1;
}


