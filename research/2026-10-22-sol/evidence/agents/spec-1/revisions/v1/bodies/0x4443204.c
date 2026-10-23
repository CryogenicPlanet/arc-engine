// ==== 0x4443204  ArcWebContents::emitFreezingVote:  (168 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcWebContents::emitFreezingVote_(ID param_1,SEL param_2,long_long param_3)

{
  code *pcVar1;
  ID IVar2;
  undefined8 uVar3;
  long lVar4;
  long local_38;
  
  IVar2 = _objc_alloc(&objc::class_t::ArcFreezingVoteToken);
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    lVar4 = *(long *)(*(long *)(param_1 + 0x118) + 0x130);
    local_38 = 0;
    if (lVar4 != 0) {
      uVar3 = FUN_00008790(0x58);
      local_38 = FUN_04335f4c(uVar3,lVar4,param_3);
    }
    IVar2 = ArcFreezingVoteToken::initWithFreezingVoteToken_
                      (IVar2,"initWithFreezingVoteToken:",&local_38);
    lVar4 = local_38;
    local_38 = 0;
    if (lVar4 != 0) {
      uVar3 = FUN_043363b4();
      FUN_00005810(uVar3,0x58);
    }
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44432ac);
  (*pcVar1)();
}


