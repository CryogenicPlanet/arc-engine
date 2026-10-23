// ==== 0x440c818  ArcTracingConfiguration::setPrivacyFilteringEnabled:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcTracingConfiguration::setPrivacyFilteringEnabled_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0x38) = (char)param_3;
  return;
}


