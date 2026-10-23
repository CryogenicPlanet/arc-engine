// ==== 0x440c80c  ArcTracingConfiguration::setTracingDuration:  (12 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcTracingConfiguration::setTracingDuration_(ID param_1,SEL param_2,double param_3)

{
  *(double *)(param_1 + 0x10) = param_3;
  *(double *)(param_1 + 0x30) = param_3;
  return;
}


