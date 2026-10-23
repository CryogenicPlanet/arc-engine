// ==== 0x440500c  ArcImage::isNull  (36 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcImage::isNull(ID param_1,SEL param_2)

{
  if (**(long **)(param_1 + 0x10) != 0) {
    return (bool)(*(long *)(**(long **)(param_1 + 0x10) + 0x18) == 0);
  }
  return 1;
}


