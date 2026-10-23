// ==== 0x43f6264  FUN_043f6264  (244 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043f6264(long param_1,const_void__ param_2,uint param_3,undefined8 *param_4)

{
  uint uVar1;
  int iVar2;
  undefined8 uVar3;
  ID self;
  ID IVar4;
  ulong uVar5;
  int *piVar6;
  long lVar7;
  int *local_38;
  
  local_38 = (int *)*param_4;
  piVar6 = local_38;
  if (((ulong)local_38 & 1) == 0) {
    iVar2 = *local_38;
    *local_38 = iVar2 + 1;
    piVar6 = (int *)CONCAT44((int)((ulong)local_38 >> 0x20),iVar2);
  }
  uVar3 = FUN_044025f4(piVar6,&local_38);
  if (((ulong)local_38 & 1) == 0) {
    FUN_04b35898();
  }
  self = *(ID *)(param_1 + 8);
  IVar4 = _objc_alloc(&objc::class_t::ArcImportSourceProfile);
  IVar4 = ArcImportSourceProfile::initWithImportSourceProfile_
                    (IVar4,"initWithImportSourceProfile:",param_2);
  if ((param_3 - 1 < (param_3 ^ param_3 - 1)) &&
     (uVar1 = (param_3 & 0xaaaaaaaa) >> 1 | (param_3 & 0x55555555) << 1,
     uVar1 = (uVar1 & 0xcccccccc) >> 2 | (uVar1 & 0x33333333) << 2,
     uVar1 = (uVar1 & 0xf0f0f0f0) >> 4 | (uVar1 & 0xf0f0f0f) << 4,
     uVar1 = (uVar1 & 0xff00ff00) >> 8 | (uVar1 & 0xff00ff) << 8,
     lVar7 = LZCOUNT(uVar1 >> 0x10 | uVar1 << 0x10), (uint)lVar7 < 0xc)) {
    uVar5 = (ulong)*(ushort *)(&DAT_0c9d8128 + lVar7 * 2);
  }
  else {
    uVar5 = 0;
  }
  _objc_msgSend(self,"didImportCategoryEnd:withCategory:withError:",IVar4,uVar5,uVar3);
  return 0;
}


