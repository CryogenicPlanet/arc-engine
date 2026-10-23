// ==== 0x4401710  ArcHistoryQueryOptions::options  (32 bytes)

/* Function Stack Size: 0x10 bytes */

HistoryQueryOptions ArcHistoryQueryOptions::options(ID param_1,SEL param_2)

{
  long *plVar1;
  undefined8 *in_x8;
  long lVar2;
  undefined8 uVar3;
  HistoryQueryOptions HVar4;
  
  lVar2 = *(long *)(param_1 + 0x10);
  uVar3 = *(undefined8 *)(param_1 + 8);
  in_x8[1] = *(undefined8 *)(param_1 + 0x10);
  *in_x8 = uVar3;
  if (lVar2 != 0) {
    plVar1 = (long *)(lVar2 + 8);
    *plVar1 = *plVar1 + 1;
  }
  HVar4.field0_0x0.field1_0x8 = (__shared_weak_count *)param_2;
  HVar4.field0_0x0.field0_0x0 = (Impl *)param_1;
  return (HistoryQueryOptions)HVar4.field0_0x0;
}


