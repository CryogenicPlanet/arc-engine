// ==== 0x443b78c  FUN_0443b78c  (232 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0443b78c(undefined8 *param_1,long *param_2,undefined8 *param_3)

{
  undefined1 **ppuVar1;
  long lVar2;
  long lVar3;
  code *pcVar4;
  ID *pIVar5;
  ID self;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined1 *local_50;
  long lStack_48;
  undefined8 local_40;
  
  pIVar5 = (ID *)*param_1;
  lVar2 = *param_2;
  lVar3 = param_2[1];
  lStack_48 = param_3[1];
  local_50 = (undefined1 *)*param_3;
  local_40 = param_3[2];
  param_3[1] = 0;
  param_3[2] = 0;
  *param_3 = 0;
  self = *pIVar5;
  if (self != 0) {
    if ((lVar3 < 0) || ((lVar2 == 0 && (lVar3 != 0)))) {
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(1,0x443b874);
      (*pcVar4)();
    }
    uVar6 = FUN_01c391bc(lVar2,lVar3);
    ppuVar1 = (undefined1 **)local_50;
    if (-1 < (long)local_40._7_1_) {
      ppuVar1 = &local_50;
    }
    lVar2 = lStack_48;
    if (-1 < (long)local_40) {
      lVar2 = (long)local_40._7_1_;
    }
    uVar7 = FUN_01c391bc(ppuVar1,lVar2);
    _objc_msgSend(self,"onCrashKeyEmitted:value:",uVar6,uVar7);
  }
  uVar6 = 0;
  if ((long)local_40 < 0) {
    uVar6 = FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  return uVar6;
}


