// ==== 0x440db2c  ArcURLLoadConfiguration::setCachePolicy:  (16 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setCachePolicy_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  *(uint *)(param_1 + 0x94) = (uint)(param_3 == 1);
  return;
}


