// ==== 0x44326cc  ArcCommandBarRankingResult::init:score:  (284 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x1c bytes */

ID ArcCommandBarRankingResult::init_score_(ID param_1,SEL param_2,ID param_3,float param_4)

{
  code *pcVar1;
  ID IVar2;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcCommandBarRankingResult;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    if ((*(char *)(IVar2 + 0x28) != '\0') && (*(char *)(IVar2 + 0x1f) < '\0')) {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x20) = 0;
    *(undefined8 *)(IVar2 + 0x18) = 0;
    *(undefined8 *)(IVar2 + 0x10) = 0;
    *(undefined8 *)(IVar2 + 8) = 0;
    *(undefined1 *)(IVar2 + 0x28) = 1;
    if (param_3 == 0) {
      local_58 = 0;
      uStack_50 = 0;
      local_48 = 0;
    }
    else {
      FUN_01b356b8(&local_58,param_3);
    }
    if ((*(byte *)(IVar2 + 0x28) & 1) == 0) {
LAB_044327e4:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x44327e8);
      (*pcVar1)();
    }
    if (*(char *)(IVar2 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
      *(undefined8 *)(IVar2 + 0x10) = uStack_50;
      *(undefined8 *)(IVar2 + 8) = local_58;
      *(undefined8 *)(IVar2 + 0x18) = local_48;
      if ((*(byte *)(IVar2 + 0x28) & 1) == 0) goto LAB_044327e4;
    }
    else {
      *(undefined8 *)(IVar2 + 0x10) = uStack_50;
      *(undefined8 *)(IVar2 + 8) = local_58;
      *(undefined8 *)(IVar2 + 0x18) = local_48;
    }
    *(float *)(IVar2 + 0x20) = param_4;
  }
  return IVar2;
}


