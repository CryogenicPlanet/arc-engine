// ==== 0x440ce18  ArcTracingController::start:  (152 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool ArcTracingController::start_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  bool bVar2;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  undefined8 uStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    if (param_3 == 0) {
      local_30 = 0;
      uStack_48 = 0;
      local_50 = 0;
      uStack_38 = 0;
      local_40 = 0;
    }
    else {
      _objc_msgSend(param_3,"tracingConfiguration",&local_50);
    }
    bVar2 = FUN_0434a700(*(undefined8 *)(param_1 + 0x20),&local_50);
    if ((long)local_40 < 0) {
      FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
    }
    return bVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440ceb0);
  (*pcVar1)();
}


