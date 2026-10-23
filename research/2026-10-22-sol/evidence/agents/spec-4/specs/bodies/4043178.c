// ==== 0x4043178  objc::Categories::ArcBrowserContextHistory_Extensions_::addUrlToHistory:title:  (220 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::addUrlToHistory_title_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  code *pcVar1;
  undefined8 local_78 [2];
  undefined7 local_68;
  char cStack_61;
  undefined8 local_60 [2];
  undefined7 local_50;
  char cStack_49;
  undefined1 auStack_48 [16];
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context_history",auStack_48);
  if ((local_38 & 1) != 0) {
    FUN_043abdf8(local_60,param_3);
    FUN_03b60ee8(local_78,param_4);
    FUN_04332ccc(auStack_48,local_60,local_78);
    if (cStack_61 < '\0') {
      FUN_00005810(local_78[0],CONCAT17(cStack_61,local_68) & 0x7fffffffffffffff);
    }
    if (cStack_49 < '\0') {
      FUN_00005810(local_60[0],CONCAT17(cStack_49,local_50) & 0x7fffffffffffffff);
    }
    if (local_38 != 0) {
      FUN_04332750(auStack_48);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4043254);
  (*pcVar1)();
}


