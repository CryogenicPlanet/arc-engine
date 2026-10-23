// ==== 0x440c640  ArcTracingConfiguration::init  (120 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcTracingConfiguration::init(ID param_1,SEL param_2)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcTracingConfiguration;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x2f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar1 + 0x18),*(ulong *)(IVar1 + 0x28) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar1 + 0x30) = 0;
    *(undefined8 *)(IVar1 + 0x28) = 0;
    *(undefined8 *)(IVar1 + 0x20) = 0;
    *(undefined8 *)(IVar1 + 0x18) = 0;
    *(undefined1 *)(IVar1 + 0x38) = 1;
  }
  return IVar1;
}


