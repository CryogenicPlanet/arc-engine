// ==== 0x4413b7c  FUN_04413b7c  (256 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04413b7c(long param_1,undefined8 param_2,long *param_3,uint param_4)

{
  code *pcVar1;
  ID self;
  undefined8 uVar2;
  ID IVar3;
  ID IVar4;
  
  self = *(ID *)(param_1 + 0x10);
  uVar2 = *(undefined8 *)(param_1 + 8);
  if ((undefined8 *)*param_3 == (undefined8 *)0x0) {
    IVar3 = 0;
  }
  else {
    IVar3 = **(ID **)*param_3;
  }
  IVar3 = _objc_msgSend(IVar3,"keyCode");
  if ((undefined8 *)*param_3 == (undefined8 *)0x0) {
    IVar4 = 0;
  }
  else {
    IVar4 = **(ID **)*param_3;
  }
  IVar4 = _objc_msgSend(IVar4,"modifierFlags");
  if (param_4 < 4) {
    _objc_msgSend(self,"webContents:didConsumeKey:withModifierFlags:underlyingEventType:",uVar2,
                  IVar3,IVar4,(ulong)(byte)(&DAT_0d741d24)[param_4]);
    return 0;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x4413c7c);
  (*pcVar1)();
}


