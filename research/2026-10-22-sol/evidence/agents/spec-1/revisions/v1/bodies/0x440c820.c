// ==== 0x440c820  ArcTracingConfiguration::setTracingConfiguration:  (140 bytes)

/* Function Stack Size: 0x38 bytes */

void ArcTracingConfiguration::setTracingConfiguration_
               (ID param_1,SEL param_2,TracingConfiguration *param_3)

{
  byte bVar1;
  TracingConfiguration *pTVar2;
  double dVar3;
  char *pcVar4;
  unsigned_long_long uVar5;
  
  if ((TracingConfiguration *)(param_1 + 0x18) != param_3) {
    bVar1 = *(byte *)((long)&(param_3->field0_0x0).field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x2f) < '\0') {
      uVar5 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      pTVar2 = (TracingConfiguration *)(param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      if (-1 < (char)bVar1) {
        uVar5 = (ulong)bVar1;
        pTVar2 = param_3;
      }
      FUN_00359ad8(param_1 + 0x18,pTVar2,uVar5);
    }
    else if ((char)bVar1 < '\0') {
      FUN_00359998(param_1 + 0x18,(param_3->field0_0x0).field0_0x0.field1.field0_0x0,
                   (param_3->field0_0x0).field0_0x0.field1.field1_0x8);
    }
    else {
      uVar5 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      pcVar4 = (param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      *(AnonymousBitField1 *)(param_1 + 0x28) = (param_3->field0_0x0).field0_0x0.field1.field2_0x10;
      *(unsigned_long_long *)(param_1 + 0x20) = uVar5;
      (((TracingConfiguration *)(param_1 + 0x18))->field0_0x0).field0_0x0.field1.field0_0x0 = pcVar4
      ;
    }
  }
  dVar3 = param_3->field1_0x18;
  *(char *)(param_1 + 0x38) = (char)param_3->field2_0x20;
  *(double *)(param_1 + 0x30) = dVar3;
  return;
}


