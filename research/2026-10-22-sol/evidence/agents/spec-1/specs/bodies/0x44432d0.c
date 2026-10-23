// ==== 0x44432d0  ArcWebContents::performAccessibilityAction:treeId:nodeId:  (256 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x24 bytes */

bool ArcWebContents::performAccessibilityAction_treeId_nodeId_
               (ID param_1,SEL param_2,ID param_3,ID param_4,int param_5)

{
  code *pcVar1;
  bool bVar2;
  SEL extraout_x1;
  undefined1 auStack_90 [8];
  undefined8 local_88;
  undefined7 local_78;
  char cStack_71;
  char local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  ulong local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    if (param_4 == 0) {
      local_68 = 0;
      uStack_60 = 0;
      local_58 = 0;
    }
    else {
      FUN_01b356b8(&local_68,param_4);
      param_2 = extraout_x1;
    }
    uStack_48 = uStack_60;
    local_50 = local_68;
    local_40 = local_58;
    FUN_043ded58(auStack_90,param_3,param_2);
    bVar2 = FUN_0836dee0(param_1 + 0x118,&local_50,param_5,auStack_90);
    if ((local_70 != '\0') && (cStack_71 < '\0')) {
      FUN_00005810(local_88,CONCAT17(cStack_71,local_78) & 0x7fffffffffffffff);
    }
    if ((long)local_40 < 0) {
      FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
    }
    return bVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44433d0);
  (*pcVar1)();
}


