// ==== 0x4414bf0  ArcWebContentsConfiguration::forcedViewSize  (40 bytes)

/* Function Stack Size: 0x10 bytes */

CGSize ArcWebContentsConfiguration::forcedViewSize(ID param_1,SEL param_2)

{
  CGSize CVar1;
  
  if (*(char *)(param_1 + 0x1c) != '\0') {
    CVar1.field0_0x0 = (double)*(int *)(param_1 + 0x14);
    CVar1.field1_0x8 = (double)*(int *)(param_1 + 0x18);
    return CVar1;
  }
  return *(CGSize *)PTR__CGSizeZero_0d826488;
}


