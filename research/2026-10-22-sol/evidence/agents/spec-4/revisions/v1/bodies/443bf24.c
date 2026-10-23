// ==== 0x443bf24  ArcFindTextWithDirectiveResult::initWithFindTextWithDirectiveResult:  (176 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcFindTextWithDirectiveResult::initWithFindTextWithDirectiveResult_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ulong uVar1;
  byte bVar2;
  const_void__ pvVar3;
  ID IVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcFindTextWithDirectiveResult;
  local_30 = param_1;
  IVar4 = NSObject::init((ID)&local_30,"init");
  if (IVar4 != 0) {
    if ((undefined8 *)(IVar4 + 8) != param_3) {
      bVar2 = *(byte *)((long)param_3 + 0x17);
      if (*(char *)(IVar4 + 0x1f) < '\0') {
        uVar1 = *(ulong *)((long)param_3 + 8);
        pvVar3 = *(const_void__ *)param_3;
        if (-1 < (char)bVar2) {
          uVar1 = (ulong)bVar2;
          pvVar3 = param_3;
        }
        FUN_00359ad8(IVar4 + 8,pvVar3,uVar1);
      }
      else if ((char)bVar2 < '\0') {
        FUN_00359998(IVar4 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
      }
      else {
        uVar6 = *(undefined8 *)((long)param_3 + 8);
        uVar5 = *(undefined8 *)param_3;
        *(undefined8 *)(IVar4 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
        *(undefined8 *)(IVar4 + 0x10) = uVar6;
        *(undefined8 *)(IVar4 + 8) = uVar5;
      }
    }
    *(undefined1 *)(IVar4 + 0x20) = *(undefined1 *)((long)param_3 + 0x18);
  }
  return IVar4;
}


