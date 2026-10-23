// ==== 0x4439f8c  ArcAIInferenceModel::init:directory:  (264 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcAIInferenceModel::init_directory_(ID param_1,SEL param_2,long_long param_3,ID param_4)

{
  code *pcVar1;
  ID IVar2;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcAIInferenceModel;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    if ((*(char *)(IVar2 + 0x28) != '\0') && (*(char *)(IVar2 + 0x27) < '\0')) {
      FUN_00005810(*(undefined8 *)(IVar2 + 0x10),*(ulong *)(IVar2 + 0x20) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x20) = 0;
    *(undefined8 *)(IVar2 + 0x18) = 0;
    *(undefined8 *)(IVar2 + 0x10) = 0;
    *(undefined8 *)(IVar2 + 8) = 0;
    *(undefined1 *)(IVar2 + 0x28) = 1;
    *(int *)(IVar2 + 8) = (int)param_3;
    if (param_4 == 0) {
      local_58 = 0;
      uStack_50 = 0;
      local_48 = 0;
    }
    else {
      FUN_01b356b8(&local_58,param_4);
    }
    if ((*(byte *)(IVar2 + 0x28) & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x443a094);
      (*pcVar1)();
    }
    if (*(char *)(IVar2 + 0x27) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 0x10),*(ulong *)(IVar2 + 0x20) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x18) = uStack_50;
    *(undefined8 *)(IVar2 + 0x10) = local_58;
    *(undefined8 *)(IVar2 + 0x20) = local_48;
  }
  return IVar2;
}


