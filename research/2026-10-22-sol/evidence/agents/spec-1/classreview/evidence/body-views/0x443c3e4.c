// ==== 0x443c3e4  ArcNetworkLoggingConfiguration::init  (108 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcNetworkLoggingConfiguration::init(ID param_1,SEL param_2)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcNetworkLoggingConfiguration;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar1 + 8),*(ulong *)(IVar1 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar1 + 8) = 0;
    *(undefined8 *)(IVar1 + 0x10) = 0;
    *(undefined8 *)(IVar1 + 0x18) = 0;
  }
  return IVar1;
}


