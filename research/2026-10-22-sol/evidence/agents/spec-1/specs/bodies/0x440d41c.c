// ==== 0x440d41c  ArcURLLoadCancellationToken::setToken:  (84 bytes)

/* Function Stack Size: 0x28 bytes */

void ArcURLLoadCancellationToken::setToken_
               (ID param_1,SEL param_2,
               basic_string<char,_std::char_traits<char>,_std::allocator<char>> *param_3)

{
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar1;
  byte bVar2;
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar3;
  char *pcVar4;
  unsigned_long_long uVar5;
  
  pbVar1 = (basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)(param_1 + 8);
  if (pbVar1 != param_3) {
    bVar2 = *(byte *)((long)&param_3->field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x1f) < '\0') {
      uVar5 = (param_3->field0_0x0).field1.field1_0x8;
      pbVar3 = (basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)
               (param_3->field0_0x0).field1.field0_0x0;
      if (-1 < (char)bVar2) {
        uVar5 = (ulong)bVar2;
        pbVar3 = param_3;
      }
      FUN_00359ad8(pbVar1,pbVar3,uVar5);
      return;
    }
    if ((char)bVar2 < '\0') {
      FUN_00359998(pbVar1,(param_3->field0_0x0).field1.field0_0x0,
                   (param_3->field0_0x0).field1.field1_0x8);
      return;
    }
    uVar5 = (param_3->field0_0x0).field1.field1_0x8;
    pcVar4 = (param_3->field0_0x0).field1.field0_0x0;
    *(AnonymousBitField1 *)(param_1 + 0x18) = (param_3->field0_0x0).field1.field2_0x10;
    *(unsigned_long_long *)(param_1 + 0x10) = uVar5;
    (pbVar1->field0_0x0).field1.field0_0x0 = pcVar4;
  }
  return;
}


