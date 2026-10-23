// ==== 0x442f174  ArcExtensionInfo::init  (164 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::init(ID param_1,SEL param_2)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcExtensionInfo;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0xe0) != '\0') {
      FUN_04334d2c(IVar1 + 8);
      *(undefined1 *)(IVar1 + 0xe0) = 0;
    }
    *(undefined8 *)(IVar1 + 0x78) = 0;
    *(undefined8 *)(IVar1 + 0x70) = 0;
    *(undefined8 *)(IVar1 + 0x68) = 0;
    *(undefined8 *)(IVar1 + 0x60) = 0;
    *(undefined8 *)(IVar1 + 0x58) = 0;
    *(undefined8 *)(IVar1 + 0x50) = 0;
    *(undefined8 *)(IVar1 + 0x48) = 0;
    *(undefined8 *)(IVar1 + 0x40) = 0;
    *(undefined8 *)(IVar1 + 0x38) = 0;
    *(undefined8 *)(IVar1 + 0x30) = 0;
    *(undefined8 *)(IVar1 + 0x28) = 0;
    *(undefined8 *)(IVar1 + 0x20) = 0;
    *(undefined8 *)(IVar1 + 0x18) = 0;
    *(undefined8 *)(IVar1 + 0x10) = 0;
    *(undefined8 *)(IVar1 + 8) = 0;
    FUN_043386a0(IVar1 + 0x80);
    *(undefined1 *)(IVar1 + 0xd8) = 0;
    *(undefined8 *)(IVar1 + 0xd0) = 0;
    *(undefined8 *)(IVar1 + 200) = 0;
    *(undefined8 *)(IVar1 + 0xc0) = 0;
    *(undefined8 *)(IVar1 + 0xb8) = 0;
    *(undefined8 *)(IVar1 + 0xb0) = 0;
    *(undefined8 *)(IVar1 + 0xa8) = 0;
    *(undefined1 *)(IVar1 + 0xe0) = 1;
  }
  return IVar1;
}


