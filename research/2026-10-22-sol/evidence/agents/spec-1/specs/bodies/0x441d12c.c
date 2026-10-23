// ==== 0x441d12c  ArcWebDialog::setDialog:  (372 bytes)

/* Function Stack Size: 0x60 bytes */

void ArcWebDialog::setDialog_(ID param_1,SEL param_2,WebDialog *param_3)

{
  basic_string<char16_t,_std::char_traits<char16_t>,_std::allocator<char16_t>> *pbVar1;
  byte bVar2;
  WebDialog *pWVar3;
  basic_string<char16_t,_std::char_traits<char16_t>,_std::allocator<char16_t>> *pbVar4;
  long lVar5;
  long *plVar6;
  code *UNRECOVERED_JUMPTABLE;
  __shared_weak_count *p_Var7;
  unsigned_short *puVar8;
  Size SVar9;
  WebContents *pWVar10;
  unsigned_long_long uVar11;
  __shared_weak_count *p_Var12;
  
  if ((WebDialog *)(param_1 + 8) != param_3) {
    bVar2 = *(byte *)((long)&(param_3->field0_0x0).field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x1f) < '\0') {
      uVar11 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      pWVar3 = (WebDialog *)(param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      if (-1 < (char)bVar2) {
        uVar11 = (ulong)bVar2;
        pWVar3 = param_3;
      }
      FUN_007ab764(param_1 + 8,pWVar3,uVar11);
    }
    else if ((char)bVar2 < '\0') {
      FUN_007ab624(param_1 + 8,(param_3->field0_0x0).field0_0x0.field1.field0_0x0,
                   (param_3->field0_0x0).field0_0x0.field1.field1_0x8);
    }
    else {
      uVar11 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      puVar8 = (param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      *(AnonymousBitField1 *)(param_1 + 0x18) = (param_3->field0_0x0).field0_0x0.field1.field2_0x10;
      *(unsigned_long_long *)(param_1 + 0x10) = uVar11;
      (((WebDialog *)(param_1 + 8))->field0_0x0).field0_0x0.field1.field0_0x0 = puVar8;
    }
  }
  SVar9 = param_3->field1_0x18;
  *(Size *)(param_1 + 0x28) = param_3->field2_0x20;
  *(Size *)(param_1 + 0x20) = SVar9;
  pbVar1 = &param_3->field3_0x28;
  if ((basic_string<char16_t,_std::char_traits<char16_t>,_std::allocator<char16_t>> *)
      (param_1 + 0x30) != pbVar1) {
    bVar2 = *(byte *)((long)&(param_3->field3_0x28).field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x47) < '\0') {
      uVar11 = (param_3->field3_0x28).field0_0x0.field1.field1_0x8;
      pbVar4 = (basic_string<char16_t,_std::char_traits<char16_t>,_std::allocator<char16_t>> *)
               (param_3->field3_0x28).field0_0x0.field1.field0_0x0;
      if (-1 < (char)bVar2) {
        uVar11 = (ulong)bVar2;
        pbVar4 = pbVar1;
      }
      FUN_007ab764(param_1 + 0x30,pbVar4,uVar11);
    }
    else if ((char)bVar2 < '\0') {
      FUN_007ab624(param_1 + 0x30,(param_3->field3_0x28).field0_0x0.field1.field0_0x0,
                   (param_3->field3_0x28).field0_0x0.field1.field1_0x8);
    }
    else {
      uVar11 = (param_3->field3_0x28).field0_0x0.field1.field1_0x8;
      puVar8 = (pbVar1->field0_0x0).field1.field0_0x0;
      *(AnonymousBitField1 *)(param_1 + 0x40) = (param_3->field3_0x28).field0_0x0.field1.field2_0x10
      ;
      *(unsigned_long_long *)(param_1 + 0x38) = uVar11;
      *(unsigned_short **)(param_1 + 0x30) = puVar8;
    }
  }
  p_Var7 = (param_3->field4_0x40).field1_0x8;
  p_Var12 = (param_3->field4_0x40).field1_0x8;
  pWVar10 = (param_3->field4_0x40).field0_0x0;
  if (p_Var7 != (__shared_weak_count *)0x0) {
    p_Var7 = p_Var7 + 0x10;
    *(long *)p_Var7 = *(long *)p_Var7 + 1;
  }
  plVar6 = *(long **)(param_1 + 0x50);
  *(__shared_weak_count **)(param_1 + 0x50) = p_Var12;
  *(WebContents **)(param_1 + 0x48) = pWVar10;
  if (plVar6 == (long *)0x0) {
    return;
  }
  if (plVar6[2] != 0) {
    LOAcquire();
    lVar5 = plVar6[2];
    plVar6[2] = lVar5 + -1;
    LORelease();
    if (lVar5 != 0) {
      return;
    }
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*plVar6 + 0x20);
  if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
     (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x0441d25c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)();
    return;
  }
  FUN_00005810(plVar6,0x20);
  return;
}


