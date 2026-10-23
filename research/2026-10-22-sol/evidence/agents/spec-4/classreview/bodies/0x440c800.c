// ==== 0x440c800  ArcTracingConfiguration::tracingDuration  (12 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcTracingConfiguration::tracingDuration(ID param_1,SEL param_2)

{
  *(double *)(param_1 + 0x10) = *(double *)(param_1 + 0x30);
  return *(double *)(param_1 + 0x30);
}


