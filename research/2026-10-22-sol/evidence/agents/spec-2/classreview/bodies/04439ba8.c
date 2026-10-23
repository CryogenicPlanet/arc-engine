// ==== 0x4439ba8  ArcAutocompleteMatch::associatedKeyword  (296 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::associatedKeyword(ID param_1,SEL param_2)

{
  long lVar1;
  const_void__ pvVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  
  if (*(ID *)(param_1 + 0x10) != 0) {
    return *(ID *)(param_1 + 0x10);
  }
  if ((*(byte *)(param_1 + 0x28) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4439cd0);
    (*pcVar4)();
  }
  pvVar2 = (const_void__)FUN_043319d8(*(undefined8 *)(param_1 + 0x18));
  if (pvVar2 == (const_void__)0x0) {
    return 0;
  }
  IVar3 = _objc_alloc(&objc::class_t::ArcAutocompleteMatch);
  IVar3 = initWithAutocompleteMatch_(IVar3,"initWithAutocompleteMatch:",pvVar2);
  *(ID *)(param_1 + 0x10) = IVar3;
  plVar5 = *(long **)((long)pvVar2 + 8);
  if (plVar5 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar5[1];
    plVar5[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar5 + 0x10))(plVar5);
      if (plVar5[2] != 0) {
        LOAcquire();
        lVar1 = plVar5[2];
        plVar5[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04439c34;
      }
      pcVar4 = *(code **)(*plVar5 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar5,0x20);
      }
      else {
        (*pcVar4)(plVar5);
      }
    }
  }
LAB_04439c34:
  FUN_00005810(pvVar2,0x10);
  return *(ID *)(param_1 + 0x10);
}


