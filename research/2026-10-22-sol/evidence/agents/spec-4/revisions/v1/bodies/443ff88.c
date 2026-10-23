// ==== 0x443ff88  ArcMenuItemModel::setDelegate:  (832 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcMenuItemModel::setDelegate_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ArcMenuItemModelDelegateBridge *pAVar2;
  __shared_weak_count *p_Var3;
  __shared_weak_count *in_x4;
  code *pcVar4;
  ArcMenuItemModelDelegateBridge *pAVar5;
  shared_ptr<ArcMenuItemModelDelegateBridge> sVar6;
  undefined8 local_50;
  undefined1 auStack_48 [8];
  
  if (param_3 != 0) {
    pAVar2 = (ArcMenuItemModelDelegateBridge *)FUN_00008790(0x30);
    pAVar5 = pAVar2 + 8;
    *(long *)pAVar5 = 0;
    *(long *)(pAVar2 + 0x10) = 0;
    *(undefined ***)pAVar2 = &PTR_viewWillStartLiveResize_0da88dd8;
    if (pAVar2 + 0x18 == (ArcMenuItemModelDelegateBridge *)0x0) goto LAB_044402c4;
    _objc_initWeak(&local_50,param_1);
    _objc_initWeak(auStack_48,local_50);
    *(undefined ***)(pAVar2 + 0x18) = &PTR_FUN_0da88f28;
    _objc_copyWeak(pAVar2 + 0x20,auStack_48);
    _objc_initWeak(pAVar2 + 0x28,param_3);
    _objc_destroyWeak(auStack_48);
    _objc_destroyWeak(&local_50);
    if (param_1 == 0) {
      if (pAVar2 != (ArcMenuItemModelDelegateBridge *)0x0) {
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
            if (lVar1 != 0) goto LAB_04440068;
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
      setMenu_item_model_delegate_bridge_
                ((ArcMenuItemModel *)param_1,(ID)"setMenu_item_model_delegate_bridge:",
                 (SEL)(pAVar2 + 0x18),sVar6);
    }
LAB_04440068:
    if ((*(byte *)(param_1 + 0x48) & 1) != 0) {
      sVar6 = menu_item_model_delegate_bridge(param_1,"menu_item_model_delegate_bridge");
      p_Var3 = sVar6.field1_0x8;
      if (p_Var3 == (__shared_weak_count *)0x0) {
        FUN_0433cc10(param_1 + 0x38,sVar6.field0_0x0,0);
        return;
      }
      lVar1 = *(long *)(p_Var3 + 0x10);
      *(long *)(p_Var3 + 0x10) = lVar1 + 1;
      FUN_0433cc10(lVar1,param_1 + 0x38,sVar6.field0_0x0,p_Var3);
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
    goto LAB_044402c4;
  }
  sVar6 = menu_item_model_delegate_bridge(param_1,"menu_item_model_delegate_bridge");
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
        if (lVar1 != 0) goto LAB_04440138;
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
LAB_04440138:
  if ((*(byte *)(param_1 + 0x48) & 1) != 0) {
    FUN_0432bf9c(*(undefined8 *)(param_1 + 0x38),0,0);
    return;
  }
LAB_044402c4:
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x44402c8);
  (*pcVar4)();
}


