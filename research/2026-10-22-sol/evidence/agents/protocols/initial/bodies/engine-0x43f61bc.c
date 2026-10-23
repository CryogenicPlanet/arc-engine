// ==== 0x43f61bc  FUN_043f61bc  (168 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043f61bc(long param_1,const_void__ param_2,uint param_3)

{
  uint uVar1;
  ID self;
  ID IVar2;
  ulong uVar3;
  long lVar4;
  
  self = *(ID *)(param_1 + 8);
  IVar2 = _objc_alloc(&objc::class_t::ArcImportSourceProfile);
  IVar2 = ArcImportSourceProfile::initWithImportSourceProfile_
                    (IVar2,"initWithImportSourceProfile:",param_2);
  if ((param_3 - 1 < (param_3 ^ param_3 - 1)) &&
     (uVar1 = (param_3 & 0xaaaaaaaa) >> 1 | (param_3 & 0x55555555) << 1,
     uVar1 = (uVar1 & 0xcccccccc) >> 2 | (uVar1 & 0x33333333) << 2,
     uVar1 = (uVar1 & 0xf0f0f0f0) >> 4 | (uVar1 & 0xf0f0f0f) << 4,
     uVar1 = (uVar1 & 0xff00ff00) >> 8 | (uVar1 & 0xff00ff) << 8,
     lVar4 = LZCOUNT(uVar1 >> 0x10 | uVar1 << 0x10), (uint)lVar4 < 0xc)) {
    uVar3 = (ulong)*(ushort *)(&DAT_0c9d8128 + lVar4 * 2);
  }
  else {
    uVar3 = 0;
  }
  _objc_msgSend(self,"didImportCategoryStart:withCategory:",IVar2,uVar3);
  return 0;
}


