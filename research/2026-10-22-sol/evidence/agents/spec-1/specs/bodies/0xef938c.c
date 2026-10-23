// ==== 0xef938c  ArcURLLoadCancellationToken::token  (40 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcURLLoadCancellationToken::token
               (basic_string<char,_std::char_traits<char>,_std::allocator<char>>
                *__return_storage_ptr__,ID param_1,SEL param_2)

{
  char *pcVar1;
  
  if (-1 < *(char *)(param_1 + 0x1f)) {
    pcVar1 = *(char **)(param_1 + 8);
    (__return_storage_ptr__->field0_0x0).field1.field1_0x8 = *(unsigned_long_long *)(param_1 + 0x10)
    ;
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0 = pcVar1;
    (__return_storage_ptr__->field0_0x0).field1.field2_0x10 =
         *(AnonymousBitField1 *)(param_1 + 0x18);
    return;
  }
  FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 8),*(undefined8 *)(param_1 + 0x10));
  return;
}


