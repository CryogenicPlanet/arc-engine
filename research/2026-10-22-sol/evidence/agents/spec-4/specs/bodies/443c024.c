// ==== 0x443c024  ArcFindTextWithDirectiveResult::setFind_text_with_directive_result:  (132 bytes)

/* Function Stack Size: 0x30 bytes */

void ArcFindTextWithDirectiveResult::setFind_text_with_directive_result_
               (ID param_1,SEL param_2,FindTextWithDirectiveResult *param_3)

{
  byte bVar1;
  FindTextWithDirectiveResult *pFVar2;
  char *pcVar3;
  unsigned_long_long uVar4;
  
  if ((FindTextWithDirectiveResult *)(param_1 + 8) != param_3) {
    bVar1 = *(byte *)((long)&(param_3->field0_0x0).field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x1f) < '\0') {
      uVar4 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      pFVar2 = (FindTextWithDirectiveResult *)(param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      if (-1 < (char)bVar1) {
        uVar4 = (ulong)bVar1;
        pFVar2 = param_3;
      }
      FUN_00359ad8(param_1 + 8,pFVar2,uVar4);
    }
    else if ((char)bVar1 < '\0') {
      FUN_00359998(param_1 + 8,(param_3->field0_0x0).field0_0x0.field1.field0_0x0,
                   (param_3->field0_0x0).field0_0x0.field1.field1_0x8);
    }
    else {
      uVar4 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      pcVar3 = (param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      *(AnonymousBitField1 *)(param_1 + 0x18) = (param_3->field0_0x0).field0_0x0.field1.field2_0x10;
      *(unsigned_long_long *)(param_1 + 0x10) = uVar4;
      (((FindTextWithDirectiveResult *)(param_1 + 8))->field0_0x0).field0_0x0.field1.field0_0x0 =
           pcVar3;
    }
  }
  *(char *)(param_1 + 0x20) = (char)param_3->field1_0x18;
  return;
}


