// ==== 0x443684c  ArcBrowserContextEffects::registerScript:withIdentifier:forHostPattern:  (344 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void ArcBrowserContextEffects::registerScript_withIdentifier_forHostPattern_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  code *pcVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  undefined8 local_78;
  undefined8 uStack_70;
  ulong local_68;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  undefined8 local_48;
  undefined8 uStack_40;
  ulong local_38;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if (param_4 == 0) {
      local_48 = 0;
      uStack_40 = 0;
      local_38 = 0;
    }
    else {
      FUN_01b356b8(&local_48,param_4);
      param_2 = extraout_x1;
    }
    if (param_5 == 0) {
      local_60 = 0;
      uStack_58 = 0;
      local_50 = 0;
    }
    else {
      FUN_01b356b8(&local_60,param_5,param_2);
      param_2 = extraout_x1_00;
    }
    if (param_3 == 0) {
      local_78 = 0;
      uStack_70 = 0;
      local_68 = 0;
    }
    else {
      FUN_01b356b8(&local_78,param_3,param_2);
    }
    FUN_042e1d80(*(undefined8 *)(param_1 + 0x10),&local_48,&local_60,&local_78,1);
    if ((long)local_68 < 0) {
      FUN_00005810(local_78,local_68 & 0x7fffffffffffffff);
    }
    if ((long)local_50 < 0) {
      FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
    }
    if ((long)local_38 < 0) {
      FUN_00005810(local_48,local_38 & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44369a4);
  (*pcVar1)();
}


