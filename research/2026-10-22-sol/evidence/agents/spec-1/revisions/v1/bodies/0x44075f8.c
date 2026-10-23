// ==== 0x44075f8  ArcMediaImage::nativeMediaImage  (32 bytes)

/* Function Stack Size: 0x10 bytes */

MediaImage ArcMediaImage::nativeMediaImage(ID param_1,SEL param_2)

{
  long *plVar1;
  undefined8 *in_x8;
  long lVar2;
  undefined8 uVar3;
  MediaImage MVar4;
  
  lVar2 = *(long *)(param_1 + 0x18);
  uVar3 = *(undefined8 *)(param_1 + 0x10);
  in_x8[1] = *(undefined8 *)(param_1 + 0x18);
  *in_x8 = uVar3;
  if (lVar2 != 0) {
    plVar1 = (long *)(lVar2 + 8);
    *plVar1 = *plVar1 + 1;
  }
  MVar4.field0_0x0.field1_0x8 = (__shared_weak_count *)param_2;
  MVar4.field0_0x0.field0_0x0 = (MediaImage_conflict *)param_1;
  return (MediaImage)MVar4.field0_0x0;
}


