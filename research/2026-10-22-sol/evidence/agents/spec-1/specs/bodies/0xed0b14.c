// ==== 0xed0b14  ArcTracingConfiguration::privacyFilteringEnabled  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcTracingConfiguration::privacyFilteringEnabled(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0x38);
}


