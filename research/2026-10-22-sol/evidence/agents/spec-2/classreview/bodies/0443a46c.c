// ==== 0x443a46c  ArcAIDistilBertClassifier::init:loraConfigJSON:configFilePath:classifierHeadFilePath:numClasses:  (644 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x34 bytes */

ID ArcAIDistilBertClassifier::init_loraConfigJSON_configFilePath_classifierHeadFilePath_numClasses_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,int param_7)

{
  code *pcVar1;
  ID IVar2;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  ID local_50;
  class_t *pcStack_48;
  
  pcStack_48 = &objc::class_t::ArcAIDistilBertClassifier;
  local_50 = param_1;
  IVar2 = NSObject::init((ID)&local_50,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x70) != '\0') {
    FUN_042a4a04(IVar2 + 8);
  }
  *(undefined8 *)(IVar2 + 0x68) = 0;
  *(undefined8 *)(IVar2 + 0x60) = 0;
  *(undefined8 *)(IVar2 + 0x58) = 0;
  *(undefined8 *)(IVar2 + 0x50) = 0;
  *(undefined8 *)(IVar2 + 0x48) = 0;
  *(undefined8 *)(IVar2 + 0x40) = 0;
  *(undefined8 *)(IVar2 + 0x38) = 0;
  *(undefined8 *)(IVar2 + 0x30) = 0;
  *(undefined8 *)(IVar2 + 0x28) = 0;
  *(undefined8 *)(IVar2 + 0x20) = 0;
  *(undefined8 *)(IVar2 + 0x18) = 0;
  *(undefined8 *)(IVar2 + 0x10) = 0;
  *(undefined8 *)(IVar2 + 8) = 0;
  *(undefined1 *)(IVar2 + 0x70) = 1;
  if (param_3 == 0) {
    local_68 = 0;
    uStack_60 = 0;
    local_58 = 0;
  }
  else {
    FUN_01b356b8(&local_68,param_3);
  }
  if ((*(byte *)(IVar2 + 0x70) & 1) != 0) {
    if (*(char *)(IVar2 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x10) = uStack_60;
    *(undefined8 *)(IVar2 + 8) = local_68;
    *(undefined8 *)(IVar2 + 0x18) = local_58;
    if (param_4 == 0) {
      local_68 = 0;
      uStack_60 = 0;
      local_58 = 0;
    }
    else {
      FUN_01b356b8(&local_68,param_4);
    }
    if ((*(byte *)(IVar2 + 0x70) & 1) != 0) {
      if (*(char *)(IVar2 + 0x37) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar2 + 0x20),*(ulong *)(IVar2 + 0x30) & 0x7fffffffffffffff);
      }
      *(undefined8 *)(IVar2 + 0x28) = uStack_60;
      *(undefined8 *)(IVar2 + 0x20) = local_68;
      *(undefined8 *)(IVar2 + 0x30) = local_58;
      if (param_5 == 0) {
        local_68 = 0;
        uStack_60 = 0;
        local_58 = 0;
      }
      else {
        FUN_01b356b8(&local_68,param_5);
      }
      if ((*(byte *)(IVar2 + 0x70) & 1) != 0) {
        if (*(char *)(IVar2 + 0x4f) < '\0') {
          FUN_00005810(*(undefined8 *)(IVar2 + 0x38),*(ulong *)(IVar2 + 0x48) & 0x7fffffffffffffff);
        }
        *(undefined8 *)(IVar2 + 0x40) = uStack_60;
        *(undefined8 *)(IVar2 + 0x38) = local_68;
        *(undefined8 *)(IVar2 + 0x48) = local_58;
        if (param_6 == 0) {
          local_68 = 0;
          uStack_60 = 0;
          local_58 = 0;
        }
        else {
          FUN_01b356b8(&local_68,param_6);
        }
        if ((*(byte *)(IVar2 + 0x70) & 1) != 0) {
          if (*(char *)(IVar2 + 0x67) < '\0') {
            FUN_00005810(*(undefined8 *)(IVar2 + 0x50),*(ulong *)(IVar2 + 0x60) & 0x7fffffffffffffff
                        );
            *(undefined8 *)(IVar2 + 0x58) = uStack_60;
            *(undefined8 *)(IVar2 + 0x50) = local_68;
            *(undefined8 *)(IVar2 + 0x60) = local_58;
            if ((*(byte *)(IVar2 + 0x70) & 1) == 0) goto LAB_0443a6ec;
          }
          else {
            *(undefined8 *)(IVar2 + 0x58) = uStack_60;
            *(undefined8 *)(IVar2 + 0x50) = local_68;
            *(undefined8 *)(IVar2 + 0x60) = local_58;
          }
          *(short *)(IVar2 + 0x68) = (short)param_7;
          return IVar2;
        }
      }
    }
  }
LAB_0443a6ec:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443a6f0);
  (*pcVar1)();
}


