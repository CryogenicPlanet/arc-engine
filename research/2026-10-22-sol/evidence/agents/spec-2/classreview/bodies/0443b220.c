// ==== 0x443b220  ArcColor4f::setColor4f:  (12 bytes)

/* Function Stack Size: 0x20 bytes */

void ArcColor4f::setColor4f_(ID param_1,SEL param_2,Color4f param_3)

{
  undefined8 *in_x2;
  undefined8 uVar1;
  
  uVar1 = *in_x2;
  *(undefined8 *)(param_1 + 0x10) = in_x2[1];
  *(undefined8 *)(param_1 + 8) = uVar1;
  return;
}


