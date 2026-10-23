// ==== 0x43fc844  FUN_043fc844  (156 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043fc844(long param_1,undefined8 param_2,const_void__ param_3,int param_4)

{
  code *pcVar1;
  ID self;
  undefined8 uVar2;
  ID IVar3;
  
  self = *(ID *)(param_1 + 0x10);
  uVar2 = *(undefined8 *)(param_1 + 8);
  IVar3 = _objc_alloc(&objc::class_t::ArcSearchEngine);
  IVar3 = ArcSearchEngine::initWithSearchEngineTemplateURL_
                    (IVar3,"initWithSearchEngineTemplateURL:",param_3);
  if (param_4 == 0) {
    _objc_msgSend(self,"browserContextPreferences:didAddSearchEngine:by:",uVar2,IVar3,0);
    return 0;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43fc8e0);
  (*pcVar1)();
}


