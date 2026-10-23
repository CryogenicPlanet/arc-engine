// ==== 0x441b68c  FUN_0441b68c  (140 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0441b68c(long param_1,undefined8 param_2,const_void__ param_3)

{
  ID IVar1;
  
  IVar1 = _objc_alloc(&objc::class_t::ArcWebContentsMediaSessionPiPController);
  IVar1 = ArcWebContentsMediaSessionPiPController::initWithPiPController_
                    (IVar1,"initWithPiPController:",param_3);
  _objc_msgSend(*(ID *)(param_1 + 8),"webContents:pipControllerCreated:",
                *(undefined8 *)(param_1 + 0x10),IVar1);
  return 0;
}


