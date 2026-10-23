// ==== 0x4443e50  ArcWebContents::errorStatus  (144 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcWebContents::errorStatus(ID param_1,SEL param_2)

{
  uint uVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  ErrorStatus local_20;
  undefined4 local_18;
  
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x4443edc);
    (*pcVar2)();
  }
  local_18 = 0;
  lVar4 = *(long *)(*(long *)(param_1 + 0x118) + 0x130);
  local_20.field0_0x0 = 0;
  local_20.field1_0x4 = 0;
  if (lVar4 == 0) {
LAB_04443e94:
    local_18 = 1;
    IVar3 = _objc_alloc(&objc::class_t::ArcErrorStatus);
    IVar3 = ArcErrorStatus::initWithErrorStatus_(IVar3,"initWithErrorStatus:",&local_20);
    return IVar3;
  }
  uVar1 = *(uint *)(lVar4 + 0x958);
  if (uVar1 < 0xc) {
    if ((0xd8fU >> (ulong)(uVar1 & 0x1f) & 1) != 0) {
      local_20.field1_0x4 = *(int *)(lVar4 + 0x95c);
      local_20.field0_0x0 = uVar1;
      goto LAB_04443e94;
    }
    if (uVar1 == 4) {
      return 0;
    }
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(0x550b,0x4443ee0);
  (*pcVar2)();
}


