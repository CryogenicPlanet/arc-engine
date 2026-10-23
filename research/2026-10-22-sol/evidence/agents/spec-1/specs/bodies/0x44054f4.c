// ==== 0x44054f4  ArcImageRep::createFromPixelDataBGRA8888:height:scale:rowBytes:pixelData:  (348 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x38 bytes */

ID ArcImageRep::createFromPixelDataBGRA8888_height_scale_rowBytes_pixelData_
             (ID param_1,SEL param_2,long_long param_3,long_long param_4,double param_5,
             unsigned_long_long param_6,ID param_7)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  undefined **local_58;
  undefined8 uStack_50;
  long *local_48;
  
  local_58 = (undefined **)0x0;
  uStack_50 = 0;
  local_48 = (long *)0x0;
  IVar3 = _objc_retainAutorelease(param_7);
  IVar3 = _objc_msgSend(IVar3,"bytes");
  FUN_043378d8(&local_58,(float)param_5,param_3,param_4,param_6,IVar3);
  IVar3 = _objc_alloc(&objc::class_t::ArcImageRep);
  IVar3 = initWithArcImageRep_(IVar3,"initWithArcImageRep:",&local_58);
  plVar1 = local_48;
  local_58 = &PTR_FUN_0da7eeb0;
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_48[1];
    local_48[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = local_48 + 2;
      (**(code **)(*local_48 + 0x10))(local_48);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return IVar3;
        }
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
  return IVar3;
}


