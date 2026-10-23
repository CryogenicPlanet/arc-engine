// ==== 0x4081f00  MenuControllerCocoa::isMenuOpen  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool MenuControllerCocoa::isMenuOpen(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0x20);
}


