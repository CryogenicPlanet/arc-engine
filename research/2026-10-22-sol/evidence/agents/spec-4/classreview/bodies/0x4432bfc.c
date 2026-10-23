// ==== 0x4432bfc  ArcCommandBarRankingInput::init:  (304 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcCommandBarRankingInput::init_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  ID IVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined8 local_48;
  undefined8 local_40;
  undefined8 local_38;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcCommandBarRankingInput;
  local_30 = param_1;
  IVar2 = NSObject::init((ID)&local_30,"init");
  if (IVar2 != 0) {
    if ((*(char *)(IVar2 + 0x20) != '\0') && (lVar3 = *(long *)(IVar2 + 8), lVar3 != 0)) {
      lVar5 = *(long *)(IVar2 + 0x10) * 4;
      if (lVar5 != 0) {
        lVar4 = *(long *)(IVar2 + 0x10) * -4;
        lVar5 = lVar5 + lVar3;
        do {
          lVar5 = lVar5 + -4;
          if (lVar5 == 0) goto LAB_04432d08;
          lVar4 = lVar4 + 4;
        } while (lVar4 != 0);
      }
      *(undefined8 *)(IVar2 + 0x10) = 0;
      FUN_00005810(lVar3,*(long *)(IVar2 + 0x18) << 2);
    }
    *(undefined8 *)(IVar2 + 8) = 0;
    *(undefined8 *)(IVar2 + 0x10) = 0;
    *(undefined8 *)(IVar2 + 0x18) = 0;
    *(undefined1 *)(IVar2 + 0x20) = 1;
    FUN_0443181c(&local_48,param_3);
    if ((*(byte *)(IVar2 + 0x20) & 1) == 0) {
LAB_04432d08:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4432d0c);
      (*pcVar1)();
    }
    lVar3 = *(long *)(IVar2 + 8);
    if (lVar3 != 0) {
      lVar5 = *(long *)(IVar2 + 0x10) * 4;
      if (lVar5 != 0) {
        lVar4 = *(long *)(IVar2 + 0x10) * -4;
        lVar5 = lVar5 + lVar3;
        do {
          lVar5 = lVar5 + -4;
          if (lVar5 == 0) goto LAB_04432d08;
          lVar4 = lVar4 + 4;
        } while (lVar4 != 0);
      }
      *(undefined8 *)(IVar2 + 0x10) = 0;
      FUN_00005810(lVar3,*(long *)(IVar2 + 0x18) << 2);
    }
    *(undefined8 *)(IVar2 + 8) = local_48;
    *(undefined8 *)(IVar2 + 0x10) = local_40;
    *(undefined8 *)(IVar2 + 0x18) = local_38;
  }
  return IVar2;
}


