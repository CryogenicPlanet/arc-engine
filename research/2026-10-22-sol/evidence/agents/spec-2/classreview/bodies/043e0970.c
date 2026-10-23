// ==== 0x43e0970  ArcTabGroupInfo::init  (132 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcTabGroupInfo::init(ID param_1,SEL param_2)

{
  undefined *puVar1;
  ID IVar2;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcTabGroupInfo;
  local_30 = param_1;
  IVar2 = NSObject::init((ID)&local_30,"init");
  if (IVar2 != 0) {
    *(cfstringStruct **)(IVar2 + 0x10) = &cf___;
    *(undefined8 *)(IVar2 + 0x18) = 0xffffffffffffffff;
    *(cfstringStruct **)(IVar2 + 0x20) = &cf___;
    *(undefined1 *)(IVar2 + 8) = 0;
    puVar1 = PTR____NSArray0__struct_0d824938;
    *(undefined8 *)(IVar2 + 0x28) = 0;
    *(undefined **)(IVar2 + 0x30) = puVar1;
  }
  return IVar2;
}


