// ==== 0x473cf0c  FUN_0473cf0c  (480 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */

undefined8
FUN_0473cf0c(ID *param_1,undefined8 param_2,long param_3,undefined8 param_4,undefined8 *param_5)

{
  undefined8 *puVar1;
  ID IVar2;
  undefined8 uVar3;
  ID IVar4;
  long lVar5;
  undefined8 *local_58;
  
  IVar4 = *param_1;
  IVar2 = _objc_msgSend((ID)&objc::class_t::WebContentsHostView,"class");
  IVar2 = _objc_msgSend(IVar4,"isMemberOfClass:",IVar2);
  if ((IVar2 & 1) != 0) {
    IVar2 = _objc_msgSend(IVar4,"knownSubviews");
    IVar2 = _objc_msgSend(IVar2,"firstObject");
    IVar4 = _objc_msgSend(IVar2,"conformsToProtocol:",&objc::protocol_t::ViewsHostable);
    if ((int)IVar4 == 0) {
      lVar5 = 0;
    }
    else {
      IVar2 = _objc_msgSend(IVar2,"viewsHostableView");
      lVar5 = 0;
      if (IVar2 != 0) {
        lVar5 = IVar2 - 0x20;
      }
    }
    uVar3 = 0;
    if (lVar5 != 0) {
      FUN_02bd55c0(lVar5,param_2);
      local_58 = (undefined8 *)FUN_00008790(8);
      *local_58 = 0;
      *local_58 = *(undefined8 *)*param_5;
      FUN_022f2830(lVar5,&local_58);
      puVar1 = local_58;
      local_58 = (undefined8 *)0x0;
      if (puVar1 != (undefined8 *)0x0) {
        FUN_00005810(puVar1,8);
      }
      FUN_068549b0(lVar5,param_3,*(undefined4 *)(param_3 + 0xc));
      uVar3 = FUN_04163f80(*(undefined8 *)(lVar5 + 0xa0),param_4);
      if (*(long *)(lVar5 + 0xa8) != 0) {
        uVar3 = FUN_0598f3f0(*(undefined8 *)(lVar5 + 0xb8),param_4);
      }
    }
    return uVar3;
  }
  return 0;
}


