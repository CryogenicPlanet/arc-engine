// ==== 0x4415204  ArcWebContentsDevTools::toolboxWebContents  (364 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcWebContentsDevTools::toolboxWebContents(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  ID IVar3;
  long lVar4;
  long local_58;
  long *plStack_50;
  byte local_48;
  
  if ((*(byte *)(param_1 + 0x30) & 1) == 0) goto LAB_0441536c;
  if (*(long *)(*(long *)(param_1 + 0x20) + 0x70) == 0) {
    return 0;
  }
  if (*(ID *)(param_1 + 8) != 0) {
    _objc_msgSend(*(ID *)(param_1 + 8),"web_contents",&local_58);
    if (((local_48 & 1) == 0) || ((*(byte *)(param_1 + 0x30) & 1) == 0)) goto LAB_0441536c;
    lVar4 = **(long **)(*(long *)(param_1 + 0x20) + 0x70);
    if (plStack_50 != (long *)0x0) {
      LOAcquire();
      lVar2 = plStack_50[1];
      plStack_50[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        (**(code **)(*plStack_50 + 0x10))(plStack_50);
        if (plStack_50[2] != 0) {
          LOAcquire();
          lVar2 = plStack_50[2];
          plStack_50[2] = lVar2 + -1;
          LORelease();
          if (lVar2 != 0) goto LAB_04415280;
        }
        pcVar1 = *(code **)(*plStack_50 + 0x20);
        if (((pcVar1 == FUN_00f01bc0) || (pcVar1 == FUN_00f01bc0)) || (pcVar1 == FUN_013efb40)) {
          FUN_00005810(plStack_50,0x20);
        }
        else {
          (*pcVar1)(plStack_50);
        }
      }
    }
LAB_04415280:
    if (local_58 == lVar4) goto LAB_044152c0;
  }
  IVar3 = _objc_alloc(&objc::class_t::ArcWebContents);
  if ((*(byte *)(param_1 + 0x30) & 1) == 0) {
LAB_0441536c:
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4415370);
    (*pcVar1)();
  }
  IVar3 = ArcWebContents::initWithWebContents_
                    (IVar3,"initWithWebContents:",
                     *(const_void__ *)(*(long *)(param_1 + 0x20) + 0x70));
  *(ID *)(param_1 + 8) = IVar3;
LAB_044152c0:
  return *(ID *)(param_1 + 8);
}


