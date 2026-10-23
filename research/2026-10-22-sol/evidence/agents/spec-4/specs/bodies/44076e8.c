// ==== 0x44076e8  ArcMemoryDumpMetric::initWithName:units:value:  (204 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

ID ArcMemoryDumpMetric::initWithName_units_value_
             (ID param_1,SEL param_2,ID param_3,ID param_4,unsigned_long_long param_5)

{
  ID IVar1;
  ID IVar2;
  undefined8 uVar3;
  ID local_50;
  class_t *pcStack_48;
  
  pcStack_48 = &objc::class_t::ArcMemoryDumpMetric;
  local_50 = param_1;
  IVar1 = NSObject::init((ID)&local_50,"init");
  if (IVar1 != 0) {
    IVar2 = _objc_msgSend(param_3,"copy");
    uVar3 = *(undefined8 *)(IVar1 + 8);
    *(ID *)(IVar1 + 8) = IVar2;
    IVar2 = _objc_msgSend(param_4,"copy",uVar3);
    *(ID *)(IVar1 + 0x10) = IVar2;
    *(unsigned_long_long *)(IVar1 + 0x18) = param_5;
  }
  return IVar1;
}


