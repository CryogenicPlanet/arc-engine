// ==== 0x441bfcc  FUN_0441bfcc  (192 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0441bfcc(long param_1,undefined8 param_2,undefined8 *param_3)

{
  code *pcVar1;
  ID self;
  long lVar2;
  uint *puVar3;
  
  self = _objc_alloc_init(&_OBJC_CLASS___NSMutableIndexSet);
  lVar2 = param_3[1] << 2;
  if (lVar2 != 0) {
    puVar3 = (uint *)*param_3;
    do {
      if (0x14 < *puVar3) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x441c08c);
        (*pcVar1)();
      }
      NSMutableIndexSet::addIndex_(self,"addIndex:");
      lVar2 = lVar2 + -4;
      puVar3 = puVar3 + 1;
    } while (lVar2 != 0);
  }
  _objc_msgSend(*(ID *)(param_1 + 8),"webContents:didChangeAvailableMediaActions:",
                *(undefined8 *)(param_1 + 0x10),self);
  return 0;
}


