// ==== 0x440d37c  ArcURLLoadCancellationToken::initWithToken:  (160 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcURLLoadCancellationToken::initWithToken_(ID param_1,SEL param_2,const_void__ param_3)

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
  
  pcStack_28 = &objc::class_t::ArcURLLoadCancellationToken;
  local_30 = param_1;
  IVar5 = NSObject::init((ID)&local_30,"init");
  puVar1 = (undefined8 *)(IVar5 + 8);
  if (IVar5 != 0 && puVar1 != param_3) {
    bVar3 = *(byte *)((long)param_3 + 0x17);
    if (*(char *)(IVar5 + 0x1f) < '\0') {
      uVar2 = *(ulong *)((long)param_3 + 8);
      pvVar4 = *(const_void__ *)param_3;
      if (-1 < (char)bVar3) {
        uVar2 = (ulong)bVar3;
        pvVar4 = param_3;
      }
      FUN_00359ad8(puVar1,pvVar4,uVar2);
    }
    else if ((char)bVar3 < '\0') {
      FUN_00359998(puVar1,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
    }
    else {
      uVar7 = *(undefined8 *)((long)param_3 + 8);
      uVar6 = *(undefined8 *)param_3;
      *(undefined8 *)(IVar5 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
      *(undefined8 *)(IVar5 + 0x10) = uVar7;
      *puVar1 = uVar6;
    }
  }
  return IVar5;
}


