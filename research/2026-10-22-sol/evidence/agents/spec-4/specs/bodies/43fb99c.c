// ==== 0x43fb99c  objc::Categories::ArcBrowserContextPreferences_Extensions_::canAutoOpenFile:  (348 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool objc::Categories::ArcBrowserContextPreferences_Extensions_::canAutoOpenFile_
               (ID param_1,SEL param_2,ID param_3)

{
  ulong uVar1;
  byte bVar2;
  long lVar3;
  code *pcVar4;
  undefined8 local_70 [2];
  undefined7 local_60;
  char cStack_59;
  undefined8 local_58;
  long *local_50;
  byte local_48;
  ulong local_40;
  undefined8 local_38;
  
  local_40 = 0;
  local_38 = 0;
  _objc_msgSend(param_1,"browser_context_preferences",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43fbaf8);
    (*pcVar4)();
  }
  FUN_043abdf8(local_70,param_3);
  FUN_0836bbd0(&local_40,local_58,local_70);
  if (cStack_59 < '\0') {
    FUN_00005810(local_70[0],CONCAT17(cStack_59,local_60) & 0x7fffffffffffffff);
  }
  if ((local_48 != 0) && (local_50 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_50[1];
    local_50[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_50 + 0x10))(local_50);
      if (local_50[2] != 0) {
        LOAcquire();
        lVar3 = local_50[2];
        local_50[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043fba34;
      }
      pcVar4 = *(code **)(*local_50 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_50,0x20);
      }
      else {
        (*pcVar4)(local_50);
      }
    }
  }
LAB_043fba34:
  uVar1 = local_40;
  bVar2 = (byte)local_38;
  if ((local_40 & 1) == 0) {
    FUN_04b35898(local_40);
  }
  return (uint)(uVar1 == 1 & bVar2);
}


