// ==== 0x4441b4c  ArcWebContents::aiAgent  (292 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcWebContents::aiAgent(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_40 [8];
  long *local_38;
  
  if (*(ID *)(param_1 + 0x28) != 0) {
    return *(ID *)(param_1 + 0x28);
  }
  IVar2 = _objc_alloc(&objc::class_t::ArcWebContentsAIAgent);
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4441c70);
    (*pcVar3)();
  }
  FUN_042c8400(auStack_40,*(undefined8 *)(*(long *)(param_1 + 0x118) + 0x298));
  IVar2 = ArcWebContentsAIAgent::initWithWebContentsAIAgent_
                    (IVar2,"initWithWebContentsAIAgent:",auStack_40);
  *(ID *)(param_1 + 0x28) = IVar2;
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_38[1];
    local_38[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_38 + 0x10))(local_38);
      if (local_38[2] != 0) {
        LOAcquire();
        lVar1 = local_38[2];
        local_38[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04441bd0;
      }
      pcVar3 = *(code **)(*local_38 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_38,0x20);
      }
      else {
        (*pcVar3)(local_38);
      }
    }
  }
LAB_04441bd0:
  return *(ID *)(param_1 + 0x28);
}


