// ==== 0x442ec50  ArcExtensionRuntimeErrorStackFrame::initWithExtensionRuntimeErrorStackFrame:  (232 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcExtensionRuntimeErrorStackFrame::initWithExtensionRuntimeErrorStackFrame_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcExtensionRuntimeErrorStackFrame;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x40) != '\0') {
      if (*(char *)(IVar1 + 0x3f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 0x28),*(long *)(IVar1 + 0x38) << 1);
      }
      if (*(char *)(IVar1 + 0x27) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 0x10),*(long *)(IVar1 + 0x20) << 1);
      }
      *(undefined1 *)(IVar1 + 0x40) = 0;
    }
    *(undefined8 *)(IVar1 + 8) = *(undefined8 *)param_3;
    if (*(char *)((long)param_3 + 0x1f) < '\0') {
      FUN_00054420(IVar1 + 0x10,*(undefined8 *)((long)param_3 + 8),
                   *(undefined8 *)((long)param_3 + 0x10));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 0x10);
      uVar2 = *(undefined8 *)((long)param_3 + 8);
      *(undefined8 *)(IVar1 + 0x20) = *(undefined8 *)((long)param_3 + 0x18);
      *(undefined8 *)(IVar1 + 0x18) = uVar3;
      *(undefined8 *)(IVar1 + 0x10) = uVar2;
    }
    if (*(char *)((long)param_3 + 0x37) < '\0') {
      FUN_00054420(IVar1 + 0x28,*(undefined8 *)((long)param_3 + 0x20),
                   *(undefined8 *)((long)param_3 + 0x28));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 0x28);
      uVar2 = *(undefined8 *)((long)param_3 + 0x20);
      *(undefined8 *)(IVar1 + 0x38) = *(undefined8 *)((long)param_3 + 0x30);
      *(undefined8 *)(IVar1 + 0x30) = uVar3;
      *(undefined8 *)(IVar1 + 0x28) = uVar2;
    }
    *(undefined1 *)(IVar1 + 0x40) = 1;
  }
  return IVar1;
}


