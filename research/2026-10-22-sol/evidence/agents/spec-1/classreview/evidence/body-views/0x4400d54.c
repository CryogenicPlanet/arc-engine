// ==== 0x4400d54  ArcAnnotatedVisit::toAnnotatedVisit  (172 bytes)

/* WARNING: Type propagation algorithm not settling */
/* Function Stack Size: 0x10 bytes */

AnnotatedVisit ArcAnnotatedVisit::toAnnotatedVisit(ID param_1,SEL param_2)

{
  AnnotatedVisit AVar1;
  undefined8 uVar2;
  unique_ptr<arc::AnnotatedVisit::Impl,_std::default_delete<arc::AnnotatedVisit::Impl>> *in_x8;
  undefined8 uVar3;
  
  if (*(char *)(param_1 + 0x10) == '\0') {
    AVar1.field0_0x0.field0_0x0 =
         (unique_ptr<arc::AnnotatedVisit::Impl,_std::default_delete<arc::AnnotatedVisit::Impl>>)
         FUN_00008790(0x150);
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 8) = 0;
    *(undefined8 *)AVar1.field0_0x0.field0_0x0 = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x18) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x10) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x28) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x20) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x38) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x30) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x48) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x40) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x58) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x50) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x68) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x60) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x78) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x70) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x88) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x80) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x98) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x90) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xa8) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xa0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xb8) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xb0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xd8) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xd0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xe8) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xe0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xf8) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xf0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x108) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x100) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x118) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x110) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x128) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x120) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x138) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x130) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x148) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x140) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 200) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xc0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x90) = 0xffffffffffffffff;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x88) = 0xffffffffffffffff;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x80) = 0xffffffffffffffff;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xa0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x98) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xb0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xa8) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xb5) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 200) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xc0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xd8) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xd0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xe8) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xe0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xf8) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0xf0) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x108) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x100) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x118) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x110) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x128) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x120) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x138) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x130) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x141) = 0;
    *(undefined8 *)((long)AVar1.field0_0x0.field0_0x0 + 0x139) = 0;
  }
  else {
    uVar3 = *(undefined8 *)(param_1 + 8);
    uVar2 = FUN_00008790(0x150);
    AVar1.field0_0x0.field0_0x0 =
         (unique_ptr<arc::AnnotatedVisit::Impl,_std::default_delete<arc::AnnotatedVisit::Impl>>)
         FUN_04336aac(uVar2,uVar3);
  }
  in_x8->field0_0x0 = (Impl *)AVar1.field0_0x0.field0_0x0;
  return (AnnotatedVisit)AVar1.field0_0x0.field0_0x0;
}


