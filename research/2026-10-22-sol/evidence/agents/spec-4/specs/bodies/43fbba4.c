// ==== 0x43fbba4  objc::Categories::ArcBrowserContextPreferences_Extensions_::setAutoOpenFile:autoOpen:error:  (392 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x24 bytes */

bool objc::Categories::ArcBrowserContextPreferences_Extensions_::setAutoOpenFile_autoOpen_error_
               (ID param_1,SEL param_2,ID param_3,bool param_4,ID *param_5)

{
  uint uVar1;
  long lVar2;
  bool bVar3;
  uint *puVar4;
  code *pcVar5;
  uint *local_70;
  undefined8 local_68 [2];
  undefined7 local_58;
  char cStack_51;
  undefined8 local_50;
  long *local_48;
  byte local_40;
  uint *local_38;
  
  local_38 = (uint *)0x0;
  _objc_msgSend(param_1,"browser_context_preferences",&local_50);
  if ((local_40 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x43fbd2c);
    (*pcVar5)();
  }
  FUN_043abdf8(local_68,param_3);
  FUN_0836be78(&local_38,local_50,local_68,param_4);
  if (cStack_51 < '\0') {
    FUN_00005810(local_68[0],CONCAT17(cStack_51,local_58) & 0x7fffffffffffffff);
  }
  if ((local_40 != 0) && (local_48 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_48[1];
    local_48[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar2 = local_48[2];
        local_48[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043fbc48;
      }
      pcVar5 = *(code **)(*local_48 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar5)(local_48);
      }
    }
  }
LAB_043fbc48:
  local_70 = local_38;
  puVar4 = local_38;
  if (((ulong)local_38 & 1) == 0) {
    uVar1 = *local_38;
    *local_38 = uVar1 + 1;
    puVar4 = (uint *)(ulong)uVar1;
  }
  bVar3 = FUN_04402548(puVar4,&local_70,param_5);
  if (((ulong)local_70 & 1) == 0) {
    FUN_04b35898();
  }
  if (((ulong)local_38 & 1) == 0) {
    FUN_04b35898();
  }
  return bVar3;
}


