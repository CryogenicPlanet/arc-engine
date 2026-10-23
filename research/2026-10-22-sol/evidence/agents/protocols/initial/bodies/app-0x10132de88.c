// ==== 0x10132de88  _TtCC15URLLoaderClient19URLLoaderClientImpl12DataDelegate::-[URLLoaderClientImpl.DataDelegate_didFail:withError:]  (160 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x1c bytes */

void _TtCC15URLLoaderClient19URLLoaderClientImpl12DataDelegate::
     __URLLoaderClientImpl_DataDelegate_didFail_withError__
               (ID param_1,SEL param_2,ID param_3,int param_4)

{
  uint uVar1;
  code *pcVar2;
  undefined *puVar3;
  uint *puVar4;
  
  puVar4 = (uint *)(ulong)param_4;
  pcVar2 = *(code **)(param_1 + failure);
  FUN_10132f788();
  uVar1 = (uint)puVar4 & 0xff;
  if (uVar1 != 0xe8) {
    param_4 = uVar1;
  }
  FUN_10132ff88();
  puVar3 = &DAT_1042431c8;
  _swift_allocError(&DAT_1042431c8,puVar4,0,0);
  *puVar4 = param_4;
  *(bool *)(puVar4 + 1) = uVar1 == 0xe8;
  (*pcVar2)(puVar3);
  _swift_errorRelease(puVar3);
  return;
}


