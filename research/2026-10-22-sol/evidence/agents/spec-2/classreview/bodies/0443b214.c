// ==== 0x443b214  ArcColor4f::color4f  (12 bytes)

/* Function Stack Size: 0x10 bytes */

Color4f ArcColor4f::color4f(ID param_1,SEL param_2)

{
  undefined8 *in_x8;
  undefined8 uVar1;
  undefined4 in_s1;
  undefined4 in_s2;
  undefined4 in_s3;
  Color4f CVar2;
  
  uVar1 = *(undefined8 *)(param_1 + 8);
  in_x8[1] = *(undefined8 *)(param_1 + 0x10);
  *in_x8 = uVar1;
  CVar2.field0_0x0 = (undefined4)uVar1;
  CVar2.field1_0x4 = (float)in_s1;
  CVar2.field3_0xc = (float)in_s3;
  CVar2.field2_0x8 = (float)in_s2;
  return CVar2;
}


