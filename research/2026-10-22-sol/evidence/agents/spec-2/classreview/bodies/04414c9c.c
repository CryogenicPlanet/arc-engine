// ==== 0x4414c9c  ArcWebContentsConfiguration::setNativeWebContentsConfiguration:  (20 bytes)

/* Function Stack Size: 0x28 bytes */

void ArcWebContentsConfiguration::setNativeWebContentsConfiguration_
               (ID param_1,SEL param_2,WebContentsConfiguration *param_3)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  uVar2 = *(undefined8 *)&param_3->field2_0x8;
  uVar1 = *(undefined8 *)param_3;
  *(undefined8 *)(param_1 + 0x15) = *(undefined8 *)((long)&param_3->field3_0xc + 1);
  *(undefined8 *)(param_1 + 0x10) = uVar2;
  *(undefined8 *)(param_1 + 8) = uVar1;
  return;
}


