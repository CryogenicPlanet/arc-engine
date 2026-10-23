// ==== 0x4408554  ArcNavigationAction::navigationType  (16 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcNavigationAction::navigationType(ID param_1,SEL param_2)

{
  uint uVar1;
  
  uVar1 = *(uint *)(param_1 + 8);
  if (4 < uVar1) {
    uVar1 = 1;
  }
  return (ulong)uVar1;
}


