// ==== 0x440a85c  objc::Categories::ArcPermissionRequest_Extensions_::additionalData  (460 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcPermissionRequest_Extensions_::additionalData(ID param_1,SEL param_2)

{
  long *plVar1;
  char cVar2;
  int iVar3;
  long lVar4;
  ID IVar5;
  undefined8 uVar6;
  ID IVar7;
  code *pcVar8;
  long *plVar9;
  long local_68;
  long *plStack_60;
  byte local_58;
  
  IVar5 = NSMutableDictionary::dictionary((ID)&_OBJC_CLASS___NSMutableDictionary,"dictionary");
  _objc_msgSend(param_1,"permission_request",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(1,0x440aa28);
    (*pcVar8)();
  }
  if (plStack_60 != (long *)0x0) {
    LOAcquire();
    lVar4 = plStack_60[1];
    plStack_60[1] = lVar4 + -1;
    LORelease();
    if (lVar4 == 0) {
      (**(code **)(*plStack_60 + 0x10))(plStack_60);
      if (plStack_60[2] != 0) {
        LOAcquire();
        lVar4 = plStack_60[2];
        plStack_60[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) goto LAB_0440a8cc;
      }
      pcVar8 = *(code **)(*plStack_60 + 0x20);
      if (((pcVar8 == FUN_00f01bc0) || (pcVar8 == FUN_00f01bc0)) || (pcVar8 == FUN_013efb40)) {
        FUN_00005810(plStack_60,0x20);
      }
      else {
        (*pcVar8)(plStack_60);
      }
    }
  }
LAB_0440a8cc:
  if (*(char *)(local_68 + 0x48) != '\0') {
    for (plVar9 = *(long **)(local_68 + 0x30); plVar9 != (long *)0x0; plVar9 = (long *)*plVar9) {
      iVar3 = *(int *)(plVar9 + 2);
      cVar2 = *(char *)((long)plVar9 + 0x2f);
      plVar1 = (long *)plVar9[3];
      if (-1 < (long)cVar2) {
        plVar1 = plVar9 + 3;
      }
      lVar4 = plVar9[4];
      if (-1 < cVar2) {
        lVar4 = (long)cVar2;
      }
      uVar6 = FUN_01c391bc(plVar1,lVar4);
      IVar7 = NSNumber::numberWithUnsignedInteger_
                        ((ID)&_OBJC_CLASS___NSNumber,"numberWithUnsignedInteger:",(long)iVar3);
      NSMutableDictionary::setObject_forKey_(IVar5,"setObject:forKey:",uVar6,IVar7);
    }
  }
  IVar5 = NSMutableDictionary::copy(IVar5,"copy");
  return IVar5;
}


