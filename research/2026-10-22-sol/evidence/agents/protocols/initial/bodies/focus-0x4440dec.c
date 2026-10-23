// ==== 0x4440dec  ArcWebContents::setNavigationDelegate:  (832 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContents::setNavigationDelegate_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ArcWebContentsNavigationDelegateBridge *pAVar2;
  __shared_weak_count *p_Var3;
  __shared_weak_count *in_x4;
  code *pcVar4;
  ArcWebContentsNavigationDelegateBridge *pAVar5;
  shared_ptr<ArcWebContentsNavigationDelegateBridge> sVar6;
  undefined8 local_50;
  undefined1 auStack_48 [8];
  
  if (param_3 != 0) {
    pAVar2 = (ArcWebContentsNavigationDelegateBridge *)FUN_00008790(0x30);
    pAVar5 = pAVar2 + 8;
    *(long *)pAVar5 = 0;
    *(long *)(pAVar2 + 0x10) = 0;
    *(undefined ***)pAVar2 = &PTR_viewWillStartLiveResize_0da88e10;
    if (pAVar2 + 0x18 == (ArcWebContentsNavigationDelegateBridge *)0x0) goto LAB_04441128;
    _objc_initWeak(&local_50,param_1);
    _objc_initWeak(auStack_48,local_50);
    *(undefined ***)(pAVar2 + 0x18) = &PTR_FUN_0da88fe0;
    _objc_copyWeak(pAVar2 + 0x20,auStack_48);
    _objc_initWeak(pAVar2 + 0x28,param_3);
    _objc_destroyWeak(auStack_48);
    _objc_destroyWeak(&local_50);
    if (param_1 == 0) {
      if (pAVar2 != (ArcWebContentsNavigationDelegateBridge *)0x0) {
        LOAcquire();
        lVar1 = *(long *)pAVar5;
        *(long *)pAVar5 = lVar1 + -1;
        LORelease();
        if (lVar1 == 0) {
          (**(code **)(*(long *)pAVar2 + 0x10))(pAVar2);
          if (*(long *)(pAVar2 + 0x10) != 0) {
            LOAcquire();
            lVar1 = *(long *)(pAVar2 + 0x10);
            *(long *)(pAVar2 + 0x10) = lVar1 + -1;
            LORelease();
            if (lVar1 != 0) goto LAB_04440ecc;
          }
          pcVar4 = *(code **)(*(long *)pAVar2 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(pAVar2,0x20);
          }
          else {
            (*pcVar4)(pAVar2);
          }
        }
      }
    }
    else {
      sVar6.field1_0x8 = in_x4;
      sVar6.field0_0x0 = pAVar2;
      setWeb_contents_navigation_delegate_bridge_
                ((ArcWebContents *)param_1,(ID)"setWeb_contents_navigation_delegate_bridge:",
                 (SEL)(pAVar2 + 0x18),sVar6);
    }
LAB_04440ecc:
    if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
      sVar6 = web_contents_navigation_delegate_bridge
                        (param_1,"web_contents_navigation_delegate_bridge");
      p_Var3 = sVar6.field1_0x8;
      if (p_Var3 == (__shared_weak_count *)0x0) {
        FUN_04345ad8(param_1 + 0x118,sVar6.field0_0x0,0);
        return;
      }
      lVar1 = *(long *)(p_Var3 + 0x10);
      *(long *)(p_Var3 + 0x10) = lVar1 + 1;
      FUN_04345ad8(lVar1,param_1 + 0x118,sVar6.field0_0x0,p_Var3);
      LOAcquire();
      lVar1 = *(long *)(p_Var3 + 8);
      *(long *)(p_Var3 + 8) = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) {
        return;
      }
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40)) {
        (*pcVar4)(p_Var3);
        return;
      }
      FUN_00005810(p_Var3,0x20);
      return;
    }
    goto LAB_04441128;
  }
  sVar6 = web_contents_navigation_delegate_bridge(param_1,"web_contents_navigation_delegate_bridge")
  ;
  p_Var3 = sVar6.field1_0x8;
  if (p_Var3 != (__shared_weak_count *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(p_Var3 + 8);
    *(long *)(p_Var3 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04440f9c;
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(p_Var3,0x20);
      }
      else {
        (*pcVar4)(p_Var3);
      }
    }
  }
LAB_04440f9c:
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_04375b40(*(undefined8 *)(param_1 + 0x118),0,0);
    return;
  }
LAB_04441128:
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x444112c);
  (*pcVar4)();
}


