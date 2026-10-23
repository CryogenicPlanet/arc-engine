// ==== 0x4442190  ArcWebContents::clone  (268 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

int ArcWebContents::clone(dev_t dev,int action)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_40 [8];
  long *local_38;
  
  IVar2 = _objc_alloc(&objc::class_t::ArcWebContents,action);
  if ((*(byte *)((ulong)(uint)dev + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x444229c);
    (*pcVar3)();
  }
  FUN_043796ec(auStack_40,*(undefined8 *)((ulong)(uint)dev + 0x118));
  IVar2 = initWithWebContents_(IVar2,"initWithWebContents:",auStack_40);
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
        if (lVar1 != 0) goto LAB_044421fc;
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
LAB_044421fc:
  return (int)IVar2;
}


