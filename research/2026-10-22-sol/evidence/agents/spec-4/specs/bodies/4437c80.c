// ==== 0x4437c80  ArcBrowserContext::pageContentExtraction  (364 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContext::pageContentExtraction(ID param_1,SEL param_2)

{
  ID IVar1;
  long lVar2;
  undefined8 *puVar3;
  code *pcVar4;
  undefined8 uVar5;
  undefined1 auStack_50 [8];
  long *local_48;
  
  IVar1 = _objc_getAssociatedObject(param_1,"pageContentExtraction");
  if (IVar1 != 0) {
    return IVar1;
  }
  IVar1 = _objc_alloc(&objc::class_t::ArcBrowserContextPageContentExtraction);
  if ((*(byte *)(param_1 + 0x40) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4437de8);
    (*pcVar4)();
  }
  puVar3 = *(undefined8 **)(*(long *)(param_1 + 0x30) + 0x130);
  if (puVar3[1] != 0) {
    uVar5 = *puVar3;
    lVar2 = FUN_00641d68();
    if (lVar2 != 0) {
      FUN_043334c8(auStack_50,uVar5,lVar2);
      IVar1 = ArcBrowserContextPageContentExtraction::
              initWithBrowserContextPageContentExtraction_forBrowserContext_
                        (IVar1,"initWithBrowserContextPageContentExtraction:forBrowserContext:",
                         auStack_50,param_1);
      if (local_48 != (long *)0x0) {
        LOAcquire();
        lVar2 = local_48[1];
        local_48[1] = lVar2 + -1;
        LORelease();
        if (lVar2 == 0) {
          (**(code **)(*local_48 + 0x10))(local_48);
          if (local_48[2] != 0) {
            LOAcquire();
            lVar2 = local_48[2];
            local_48[2] = lVar2 + -1;
            LORelease();
            if (lVar2 != 0) goto LAB_04437d30;
          }
          pcVar4 = *(code **)(*local_48 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(local_48,0x20);
          }
          else {
            (*pcVar4)(local_48);
          }
        }
      }
LAB_04437d30:
      _objc_setAssociatedObject(param_1,"pageContentExtraction",IVar1,1);
      return IVar1;
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_050d342c();
}


