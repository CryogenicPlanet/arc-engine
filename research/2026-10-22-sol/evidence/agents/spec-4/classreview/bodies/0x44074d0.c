// ==== 0x44074d0  ArcMediaImage::dealloc  (76 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcMediaImage::dealloc(ID param_1,SEL param_2)

{
  ID local_30;
  class_t *pcStack_28;
  
  if (*(long *)(param_1 + 8) != 0) {
    FUN_00010ba4();
  }
  pcStack_28 = &objc::class_t::ArcMediaImage;
  local_30 = param_1;
  NSObject::dealloc((ID)&local_30,"dealloc");
  return;
}


