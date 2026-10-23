// ==== 0x4041e2c  objc::Categories::ArcBrowserContextDownloads_Extensions_::isEqual:  (268 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool objc::Categories::ArcBrowserContextDownloads_Extensions_::isEqual_
               (ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  bool bVar2;
  ID IVar3;
  undefined1 auStack_60 [16];
  byte local_50;
  undefined1 auStack_48 [16];
  byte local_38;
  
  if (param_1 == param_3) {
    bVar2 = 1;
  }
  else {
    if (param_3 != 0) {
      IVar3 = _objc_msgSend((ID)&class_t::ArcBrowserContextDownloads,"class");
      IVar3 = _objc_msgSend(param_3,"isKindOfClass:",IVar3);
      if ((int)IVar3 != 0) {
        _objc_msgSend(param_1,"browser_context_downloads",auStack_48);
        _objc_msgSend(param_3,"browser_context_downloads",auStack_60);
        if ((local_50 & local_38) == 0) {
          bVar2 = (bool)(local_38 == local_50);
        }
        else {
          if ((local_50 & 1) == 0) {
                    /* WARNING: Does not return */
            pcVar1 = (code *)SoftwareBreakpoint(1,0x4041f38);
            (*pcVar1)();
          }
          bVar2 = FUN_0411db00(auStack_48,auStack_60);
          local_50 = local_50 & 1;
        }
        if (local_50 != 0) {
          FUN_04332750(auStack_60);
        }
        if (local_38 == 0) {
          return bVar2;
        }
        FUN_04332750(auStack_48);
        return bVar2;
      }
    }
    bVar2 = 0;
  }
  return bVar2;
}


