// ==== 0x442d974  ArcMostVisitedURL::initWithMostVisitedSite:  (224 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcMostVisitedURL::initWithMostVisitedSite_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcMostVisitedURL;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x38) != '\0') {
      if (*(char *)(IVar1 + 0x37) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 0x20),*(long *)(IVar1 + 0x30) << 1);
      }
      if (*(char *)(IVar1 + 0x1f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 8),*(ulong *)(IVar1 + 0x18) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar1 + 0x38) = 0;
    }
    if (*(char *)((long)param_3 + 0x17) < '\0') {
      FUN_00060a60(IVar1 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 8);
      uVar2 = *(undefined8 *)param_3;
      *(undefined8 *)(IVar1 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
      *(undefined8 *)(IVar1 + 0x10) = uVar3;
      *(undefined8 *)(IVar1 + 8) = uVar2;
    }
    if (*(char *)((long)param_3 + 0x2f) < '\0') {
      FUN_00054420(IVar1 + 0x20,*(undefined8 *)((long)param_3 + 0x18),
                   *(undefined8 *)((long)param_3 + 0x20));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 0x20);
      uVar2 = *(undefined8 *)((long)param_3 + 0x18);
      *(undefined8 *)(IVar1 + 0x30) = *(undefined8 *)((long)param_3 + 0x28);
      *(undefined8 *)(IVar1 + 0x28) = uVar3;
      *(undefined8 *)(IVar1 + 0x20) = uVar2;
    }
    *(undefined1 *)(IVar1 + 0x38) = 1;
  }
  return IVar1;
}


