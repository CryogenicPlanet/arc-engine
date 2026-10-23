// ==== 0x43e6224  ArcBrowserApplicationDebugProperty::initWithDebugProperty:  (264 bytes)

/* Function Stack Size: 0x40 bytes */

ID ArcBrowserApplicationDebugProperty::initWithDebugProperty_
             (ID param_1,SEL param_2,DebugProperty *param_3)

{
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar1;
  byte bVar2;
  DebugProperty *pDVar3;
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar4;
  ID IVar5;
  char *pcVar6;
  unsigned_long_long uVar7;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcBrowserApplicationDebugProperty;
  local_30 = param_1;
  IVar5 = NSObject::init((ID)&local_30,"init");
  if (IVar5 != 0) {
    if ((DebugProperty *)(IVar5 + 8) != param_3) {
      bVar2 = *(byte *)((long)&(param_3->field0_0x0).field0_0x0 + 0x17);
      if (*(char *)(IVar5 + 0x1f) < '\0') {
        uVar7 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
        pDVar3 = (DebugProperty *)(param_3->field0_0x0).field0_0x0.field1.field0_0x0;
        if (-1 < (char)bVar2) {
          uVar7 = (ulong)bVar2;
          pDVar3 = param_3;
        }
        FUN_00359ad8(IVar5 + 8,pDVar3,uVar7);
      }
      else if ((char)bVar2 < '\0') {
        FUN_00359998(IVar5 + 8,(param_3->field0_0x0).field0_0x0.field1.field0_0x0,
                     (param_3->field0_0x0).field0_0x0.field1.field1_0x8);
      }
      else {
        uVar7 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
        pcVar6 = (param_3->field0_0x0).field0_0x0.field1.field0_0x0;
        *(AnonymousBitField1 *)(IVar5 + 0x18) = (param_3->field0_0x0).field0_0x0.field1.field2_0x10;
        *(unsigned_long_long *)(IVar5 + 0x10) = uVar7;
        (((DebugProperty *)(IVar5 + 8))->field0_0x0).field0_0x0.field1.field0_0x0 = pcVar6;
      }
    }
    pbVar1 = &param_3->field1_0x18;
    if ((basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)(IVar5 + 0x20) != pbVar1
       ) {
      bVar2 = *(byte *)((long)&(param_3->field1_0x18).field0_0x0 + 0x17);
      if (*(char *)(IVar5 + 0x37) < '\0') {
        uVar7 = (param_3->field1_0x18).field0_0x0.field1.field1_0x8;
        pbVar4 = (basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)
                 (param_3->field1_0x18).field0_0x0.field1.field0_0x0;
        if (-1 < (char)bVar2) {
          uVar7 = (ulong)bVar2;
          pbVar4 = pbVar1;
        }
        FUN_00359ad8(IVar5 + 0x20,pbVar4,uVar7);
      }
      else if ((char)bVar2 < '\0') {
        FUN_00359998(IVar5 + 0x20,(param_3->field1_0x18).field0_0x0.field1.field0_0x0,
                     (param_3->field1_0x18).field0_0x0.field1.field1_0x8);
      }
      else {
        uVar7 = (param_3->field1_0x18).field0_0x0.field1.field1_0x8;
        pcVar6 = (pbVar1->field0_0x0).field1.field0_0x0;
        *(AnonymousBitField1 *)(IVar5 + 0x30) = (param_3->field1_0x18).field0_0x0.field1.field2_0x10
        ;
        *(unsigned_long_long *)(IVar5 + 0x28) = uVar7;
        *(char **)(IVar5 + 0x20) = pcVar6;
      }
    }
  }
  return IVar5;
}


