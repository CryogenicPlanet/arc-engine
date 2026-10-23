// ==== 0x441cf64  ArcWebDialog::webContents  (280 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcWebDialog::webContents(ID param_1,SEL param_2)

{
  long lVar1;
  long lVar2;
  bool bVar3;
  long lVar4;
  ID IVar5;
  code *pcVar6;
  long *plVar7;
  const_void__ pvVar8;
  
  plVar7 = *(long **)(param_1 + 0x50);
  if (plVar7 != (long *)0x0) {
    lVar4 = plVar7[1];
    do {
      if (lVar4 == -1) goto LAB_0441d114;
      lVar1 = lVar4 + 1;
      lVar2 = plVar7[1];
      bVar3 = lVar2 != lVar4;
      lVar4 = lVar2;
    } while (bVar3);
    plVar7[1] = lVar1;
    pvVar8 = *(const_void__ *)(param_1 + 0x48);
    if (pvVar8 != (const_void__)0x0) {
      IVar5 = _objc_alloc(&objc::class_t::ArcWebContents);
      IVar5 = ArcWebContents::initWithWebContents_(IVar5,"initWithWebContents:",pvVar8);
      LOAcquire();
      lVar4 = plVar7[1];
      plVar7[1] = lVar4 + -1;
      LORelease();
      if (lVar4 == 0) {
        (**(code **)(*plVar7 + 0x10))(plVar7);
        if (plVar7[2] != 0) {
          LOAcquire();
          lVar4 = plVar7[2];
          plVar7[2] = lVar4 + -1;
          LORelease();
          if (lVar4 != 0) {
            return IVar5;
          }
        }
        pcVar6 = *(code **)(*plVar7 + 0x20);
        if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
          FUN_00005810(plVar7,0x20);
        }
        else {
          (*pcVar6)(plVar7);
        }
      }
      return IVar5;
    }
  }
LAB_0441d114:
                    /* WARNING: Does not return */
  pcVar6 = (code *)SoftwareBreakpoint(0,0x441d118);
  (*pcVar6)();
}


