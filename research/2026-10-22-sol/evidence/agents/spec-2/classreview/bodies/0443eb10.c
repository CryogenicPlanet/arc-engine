// ==== 0x443eb10  ArcFreezingVoteToken::initWithFreezingVoteToken:  (132 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcFreezingVoteToken::initWithFreezingVoteToken_(ID param_1,SEL param_2,void *param_3)

{
  ID IVar1;
  long lVar2;
  undefined8 uVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcFreezingVoteToken;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x10) != '\0') {
      lVar2 = *(long *)(IVar1 + 8);
      *(undefined8 *)(IVar1 + 8) = 0;
      if (lVar2 != 0) {
        uVar3 = FUN_043363b4();
        FUN_00005810(uVar3,0x58);
      }
      *(undefined1 *)(IVar1 + 0x10) = 0;
    }
    uVar3 = *(undefined8 *)param_3;
    *(undefined8 *)param_3 = 0;
    *(undefined8 *)(IVar1 + 8) = uVar3;
    *(undefined1 *)(IVar1 + 0x10) = 1;
  }
  return IVar1;
}


