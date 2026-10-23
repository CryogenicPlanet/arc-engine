// ==== 0x4041f6c  objc::Categories::ArcBrowserContextDownloads_Extensions_::hash  (100 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long
objc::Categories::ArcBrowserContextDownloads_Extensions_::hash(ID param_1,SEL param_2)

{
  unsigned_long_long uVar1;
  undefined1 auStack_40 [16];
  byte local_30;
  undefined1 local_21;
  
  local_21 = 0;
  _objc_msgSend(param_1,"browser_context_downloads",auStack_40);
  if ((local_30 & 1) != 0) {
    uVar1 = FUN_04332988(&local_21,auStack_40);
    if (local_30 != 0) {
      FUN_04332750(auStack_40);
    }
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}


