// ==== 0x443d7b4  ArcSessionTab::groupId  (256 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSessionTab::groupId(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  long lVar3;
  ID IVar4;
  undefined1 auVar5 [16];
  undefined1 *local_70;
  long lStack_68;
  undefined8 local_60;
  undefined8 local_58;
  undefined8 local_48;
  undefined *puStack_40;
  undefined8 local_38;
  undefined *puStack_30;
  long local_28;
  
  auVar5._8_8_ = param_2;
  auVar5._0_8_ = param_1;
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x443d8b0);
    (*pcVar2)();
  }
  lStack_68 = 0;
  local_70 = (undefined1 *)0x0;
  local_58 = 0;
  local_60 = 0;
  lVar3 = **(long **)(param_1 + 0x10);
  if (*(char *)(lVar3 + 0x20) == '\0') {
    IVar4 = 0;
  }
  else {
    local_48 = *(undefined8 *)(lVar3 + 0x10);
    puStack_40 = &DAT_01115580;
    local_38 = *(undefined8 *)(lVar3 + 0x18);
    puStack_30 = &DAT_01115580;
    FUN_00331000(&local_70,"%016llX%016llX",0xe,&local_48,2);
    local_58 = CONCAT71(local_58._1_7_,1);
    ppuVar1 = (undefined1 **)local_70;
    if (-1 < (long)local_60._7_1_) {
      ppuVar1 = &local_70;
    }
    lVar3 = lStack_68;
    if (-1 < (long)local_60) {
      lVar3 = (long)local_60._7_1_;
    }
    auVar5 = FUN_01c391bc(ppuVar1,lVar3);
    IVar4 = auVar5._0_8_;
    if (((char)local_58 != '\0') && ((long)local_60 < 0)) {
      auVar5 = FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return IVar4;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(auVar5._0_8_,auVar5._8_8_);
}


