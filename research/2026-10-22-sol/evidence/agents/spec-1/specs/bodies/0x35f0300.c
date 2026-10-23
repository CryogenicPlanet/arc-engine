// ==== 0x35f0300  ArcCpuInfo::extFamily  (8 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcCpuInfo::extFamily(ID param_1,SEL param_2)

{
  return *(long_long *)(param_1 + 0x60);
}


