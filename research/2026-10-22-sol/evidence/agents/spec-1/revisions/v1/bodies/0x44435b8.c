// ==== 0x44435b8  ArcWebContents::sendKeyEvent:treeId:  (248 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool ArcWebContents::sendKeyEvent_treeId_(ID param_1,SEL param_2,ID param_3,ID param_4)

{
  code *pcVar1;
  bool bVar2;
  undefined8 local_88;
  undefined8 uStack_80;
  ulong local_78;
  undefined1 auStack_70 [8];
  undefined8 local_68;
  undefined7 local_58;
  char cStack_51;
  undefined8 local_50;
  undefined7 local_40;
  char cStack_39;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_043df00c(auStack_70,param_3);
    if (param_4 == 0) {
      local_88 = 0;
      uStack_80 = 0;
      local_78 = 0;
    }
    else {
      FUN_01b356b8(&local_88,param_4);
    }
    bVar2 = FUN_0439dc40(*(undefined8 *)(param_1 + 0x118),auStack_70,&local_88);
    if ((long)local_78 < 0) {
      FUN_00005810(local_88,local_78 & 0x7fffffffffffffff);
    }
    if (cStack_39 < '\0') {
      FUN_00005810(local_50,CONCAT17(cStack_39,local_40) & 0x7fffffffffffffff);
    }
    if (cStack_51 < '\0') {
      FUN_00005810(local_68,CONCAT17(cStack_51,local_58) & 0x7fffffffffffffff);
    }
    return bVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44436b0);
  (*pcVar1)();
}


