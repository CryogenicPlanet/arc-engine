// ==== 0x440acd8  ArcSessionID::identifier  (28 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcSessionID::identifier(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0xc) & 1) != 0) {
    return (long)*(int *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}


