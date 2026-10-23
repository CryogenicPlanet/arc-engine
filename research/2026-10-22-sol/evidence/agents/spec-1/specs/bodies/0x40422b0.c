// ==== 0x40422b0  objc::Categories::ArcBrowserContextDownloads_Extensions_::startDownload:error:  (276 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContextDownloads_Extensions_::startDownload_error_
               (ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  code *pcVar1;
  uint uVar2;
  ID IVar3;
  uint *puVar4;
  uint *local_70;
  undefined8 local_68 [2];
  undefined7 local_58;
  char cStack_51;
  undefined1 auStack_50 [16];
  byte local_40;
  uint *local_38;
  
  local_38 = (uint *)0x0;
  _objc_msgSend(param_1,"browser_context_downloads",auStack_50);
  if ((local_40 & 1) != 0) {
    IVar3 = _objc_msgSend(param_3,"absoluteString");
    FUN_03b60ee8(local_68,IVar3);
    FUN_043328cc(&local_38,auStack_50,local_68);
    if (cStack_51 < '\0') {
      FUN_00005810(local_68[0],CONCAT17(cStack_51,local_58) & 0x7fffffffffffffff);
    }
    if (local_40 != 0) {
      FUN_04332750(auStack_50);
    }
    if (local_38 != (uint *)((long)&MACH_HEADER.magic + 1) && param_4 != (ID *)0x0) {
      local_70 = local_38;
      puVar4 = local_38;
      if (((ulong)local_38 & 1) == 0) {
        uVar2 = *local_38;
        *local_38 = uVar2 + 1;
        puVar4 = (uint *)(ulong)uVar2;
      }
      IVar3 = FUN_044025f4(puVar4,&local_70);
      *param_4 = IVar3;
      if (((ulong)local_70 & 1) == 0) {
        FUN_04b35898();
      }
    }
    if (((ulong)local_38 & 1) == 0) {
      FUN_04b35898();
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x40423c4);
  (*pcVar1)();
}


