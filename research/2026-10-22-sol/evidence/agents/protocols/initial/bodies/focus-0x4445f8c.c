// ==== 0x4445f8c  FUN_04445f8c  (200 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8
FUN_04445f8c(long param_1,undefined8 param_2,uint param_3,const_void__ param_4,undefined8 param_5)

{
  code *pcVar1;
  ID self;
  undefined8 uVar2;
  ID IVar3;
  undefined8 uVar4;
  
  self = *(ID *)(param_1 + 0x10);
  uVar2 = *(undefined8 *)(param_1 + 8);
  IVar3 = _objc_alloc(&objc::class_t::ArcWebContents);
  IVar3 = ArcWebContents::initWithWebContents_(IVar3,"initWithWebContents:",param_4);
  uVar4 = FUN_043abd34(param_5);
  if (param_3 < 0xc) {
    _objc_msgSend(self,"webContents:didCreateNewWebContent:targetURL:disposition:",uVar2,IVar3,uVar4
                  ,(ulong)param_3);
    return 0;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x4446054);
  (*pcVar1)();
}


