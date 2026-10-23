// ==== 0x443bda8  ArcExternalProtocolDialog::initWithExternalProtocolDialog:  (116 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcExternalProtocolDialog::initWithExternalProtocolDialog_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcExternalProtocolDialog;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x98) != '\0') {
      FUN_04335bdc(IVar1 + 8);
      *(undefined1 *)(IVar1 + 0x98) = 0;
    }
    FUN_04335c68(IVar1 + 8,param_3);
    *(undefined1 *)(IVar1 + 0x98) = 1;
  }
  return IVar1;
}


