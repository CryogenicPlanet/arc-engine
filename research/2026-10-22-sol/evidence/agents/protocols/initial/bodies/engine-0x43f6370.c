// ==== 0x43f6370  FUN_043f6370  (188 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043f6370(long param_1,const_void__ param_2,undefined8 *param_3)

{
  int iVar1;
  undefined8 uVar2;
  ID self;
  ID IVar3;
  int *piVar4;
  int *local_38;
  
  local_38 = (int *)*param_3;
  piVar4 = local_38;
  if (((ulong)local_38 & 1) == 0) {
    iVar1 = *local_38;
    *local_38 = iVar1 + 1;
    piVar4 = (int *)CONCAT44((int)((ulong)local_38 >> 0x20),iVar1);
  }
  uVar2 = FUN_044025f4(piVar4,&local_38);
  if (((ulong)local_38 & 1) == 0) {
    FUN_04b35898();
  }
  self = *(ID *)(param_1 + 8);
  IVar3 = _objc_alloc(&objc::class_t::ArcImportSourceProfile);
  IVar3 = ArcImportSourceProfile::initWithImportSourceProfile_
                    (IVar3,"initWithImportSourceProfile:",param_2);
  _objc_msgSend(self,"didImportEnd:withError:",IVar3,uVar2);
  return 0;
}


