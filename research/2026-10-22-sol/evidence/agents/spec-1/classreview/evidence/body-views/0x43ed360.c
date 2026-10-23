// ==== 0x43ed360  ArcBrowserContextID::initWithBrowserContextID:  (244 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcBrowserContextID::initWithBrowserContextID_(ID param_1,SEL param_2,const_void__ param_3)

{
  undefined8 *puVar1;
  ulong uVar2;
  byte bVar3;
  const_void__ pvVar4;
  ID IVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcBrowserContextID;
  local_30 = param_1;
  IVar5 = NSObject::init((ID)&local_30,"init");
  if (IVar5 != 0) {
    puVar1 = (undefined8 *)(IVar5 + 8);
    if (*(char *)(IVar5 + 0x28) == '\0') {
      if (*(char *)((long)param_3 + 0x17) < '\0') {
        FUN_00060a60(IVar5 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
      }
      else {
        uVar7 = *(undefined8 *)((long)param_3 + 8);
        uVar6 = *(undefined8 *)param_3;
        *(undefined8 *)(IVar5 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
        *(undefined8 *)(IVar5 + 0x10) = uVar7;
        *puVar1 = uVar6;
      }
      *(undefined4 *)(IVar5 + 0x20) = *(undefined4 *)((long)param_3 + 0x18);
      *(undefined1 *)(IVar5 + 0x28) = 1;
    }
    else {
      if (puVar1 != param_3) {
        bVar3 = *(byte *)((long)param_3 + 0x17);
        if (*(char *)(IVar5 + 0x1f) < '\0') {
          uVar2 = *(ulong *)((long)param_3 + 8);
          pvVar4 = *(const_void__ *)param_3;
          if (-1 < (char)bVar3) {
            uVar2 = (ulong)bVar3;
            pvVar4 = param_3;
          }
          FUN_00359ad8(IVar5 + 8,pvVar4,uVar2);
        }
        else if ((char)bVar3 < '\0') {
          FUN_00359998(IVar5 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
        }
        else {
          uVar7 = *(undefined8 *)((long)param_3 + 8);
          uVar6 = *(undefined8 *)param_3;
          *(undefined8 *)(IVar5 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
          *(undefined8 *)(IVar5 + 0x10) = uVar7;
          *puVar1 = uVar6;
        }
      }
      *(undefined4 *)(IVar5 + 0x20) = *(undefined4 *)((long)param_3 + 0x18);
    }
  }
  return IVar5;
}


