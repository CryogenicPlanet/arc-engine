// ==== 0x440bd1c  ArcSpake2Context::initWithRole:myName:theirName:  (308 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

ID ArcSpake2Context::initWithRole_myName_theirName_
             (ID param_1,SEL param_2,long_long param_3,ID param_4,ID param_5)

{
  ID IVar1;
  ID IVar2;
  ID IVar3;
  ID IVar4;
  ID IVar5;
  undefined8 uVar6;
  long lVar7;
  ID local_70;
  class_t *pcStack_68;
  
  pcStack_68 = &objc::class_t::ArcSpake2Context;
  local_70 = param_1;
  IVar1 = NSObject::init((ID)&local_70,"init");
  if (IVar1 != 0) {
    IVar2 = _objc_retainAutorelease(param_4);
    IVar2 = _objc_msgSend(IVar2,"bytes");
    IVar3 = _objc_msgSend(param_4,"length");
    IVar4 = _objc_retainAutorelease(param_5);
    IVar4 = _objc_msgSend(IVar4,"bytes");
    IVar5 = _objc_msgSend(param_5,"length");
    uVar6 = FUN_00008790(8);
    FUN_04341ab0(uVar6,param_3 != 0,IVar2,IVar3,IVar4,IVar5);
    lVar7 = *(long *)(IVar1 + 8);
    *(undefined8 *)(IVar1 + 8) = uVar6;
    if (lVar7 != 0) {
      uVar6 = FUN_04341b9c();
      FUN_00005810(uVar6,8);
    }
  }
  return IVar1;
}


