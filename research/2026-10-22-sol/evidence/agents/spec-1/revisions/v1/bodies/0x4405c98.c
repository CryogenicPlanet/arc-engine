// ==== 0x4405c98  ArcImageRep::imageRep  (44 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcImageRep::imageRep(ImageRep *__return_storage_ptr__,ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ImageRepImpl *pIVar3;
  
  __return_storage_ptr__->field0_0x0 = (undefined4 **)&PTR_FUN_0da7eeb0;
  lVar2 = *(long *)(param_1 + 0x18);
  pIVar3 = *(ImageRepImpl **)(param_1 + 0x10);
  (__return_storage_ptr__->field1_0x8).field1_0x8 = *(__shared_weak_count **)(param_1 + 0x18);
  (__return_storage_ptr__->field1_0x8).field0_0x0 = pIVar3;
  if (lVar2 != 0) {
    plVar1 = (long *)(lVar2 + 8);
    *plVar1 = *plVar1 + 1;
  }
  return;
}


