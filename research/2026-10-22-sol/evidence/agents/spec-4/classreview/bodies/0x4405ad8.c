// ==== 0x4405ad8  ArcImageRep::copyPixelDataBGRA8888  (140 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcImageRep::copyPixelDataBGRA8888(ID param_1,SEL param_2)

{
  int iVar1;
  ID IVar2;
  ID IVar3;
  
  IVar2 = NSMutableData::dataWithLength_
                    ((ID)&_OBJC_CLASS___NSMutableData,"dataWithLength:",
                     (long)(*(int *)(*(long *)(param_1 + 0x10) + 0x1c4) *
                            *(int *)(*(long *)(param_1 + 0x10) + 0x1c8) * 4));
  if (IVar2 == 0) {
    IVar2 = 0;
  }
  else {
    IVar3 = _objc_retainAutorelease();
    IVar3 = _objc_msgSend(IVar3,"mutableBytes");
    iVar1 = FUN_04338370(param_1 + 8,IVar3);
    if (iVar1 == 0) {
      IVar2 = 0;
    }
  }
  return IVar2;
}


