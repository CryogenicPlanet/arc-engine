// ==== 0x443d8e0  ArcTabGroupProperties::init  (136 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcTabGroupProperties::init(ID param_1,SEL param_2)

{
  ID IVar1;
  undefined1 local_60;
  undefined7 uStack_5f;
  undefined7 local_50;
  char cStack_49;
  char local_48;
  undefined1 local_40;
  undefined1 local_3c;
  undefined2 local_38;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcTabGroupProperties;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    local_60 = 0;
    local_48 = '\0';
    local_40 = 0;
    local_3c = 0;
    local_38 = 0;
    FUN_0441e9f0(IVar1 + 8,&local_60);
    if ((local_48 != '\0') && (cStack_49 < '\0')) {
      FUN_00005810(CONCAT71(uStack_5f,local_60),CONCAT17(cStack_49,local_50) & 0x7fffffffffffffff);
    }
  }
  return IVar1;
}


