// ==== 0xf90b90  ArcCpuInfo::armPartNumber  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcCpuInfo::armPartNumber(ID param_1,SEL param_2)

{
  return *(unsigned_long_long *)(param_1 + 0x68);
}


