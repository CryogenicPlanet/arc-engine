// ==== 0x4401aa0  FUN_04401aa0  (100 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04401aa0(long param_1,undefined8 param_2,undefined8 param_3)

{
  _objc_msgSend(*(ID *)(param_1 + 8),"sidePanelWillClose:withAnimation:",
                *(undefined8 *)(param_1 + 0x10),param_3);
  return 0;
}


