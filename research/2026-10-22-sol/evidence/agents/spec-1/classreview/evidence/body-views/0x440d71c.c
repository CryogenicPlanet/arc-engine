// ==== 0x440d71c  ArcURLLoadConfiguration::method  (40 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcURLLoadConfiguration::method(ID param_1,SEL param_2)

{
  uint uVar1;
  
  uVar1 = *(int *)(param_1 + 0x90) - 1;
  if (2 < uVar1) {
    return 0;
  }
  return (ulong)(byte)(&DAT_0c9d8190)[uVar1];
}


