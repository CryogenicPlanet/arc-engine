// ==== 0x44025f4  FUN_044025f4  (536 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */

ID FUN_044025f4(ulong *param_1)

{
  uint uVar1;
  code *pcVar2;
  char **ppcVar3;
  ID IVar4;
  ulong uVar5;
  uint uVar6;
  ulong uVar7;
  size_t sVar8;
  char *pcVar9;
  char *local_70;
  ulong uStack_68;
  undefined8 local_60;
  undefined8 local_58;
  undefined8 uStack_50;
  long local_48;
  
  ppcVar3 = &local_70;
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  uVar5 = *param_1;
  if (uVar5 == 1) {
    IVar4 = 0;
    goto LAB_044027cc;
  }
  if ((uVar5 & 1) == 0) {
    uVar6 = *(uint *)(uVar5 + 4);
  }
  else {
    uVar6 = (uint)(uVar5 >> 2);
  }
  uVar1 = uVar6 - 1;
  if (0x10 < uVar6) {
    uVar1 = 1;
  }
  if (uVar1 < 0xc) {
    uVar7 = (ulong)(byte)(&DAT_0c9d8148)[uVar1];
    if ((uVar5 & 1) != 0) goto LAB_04402698;
LAB_0440265c:
    sVar8 = (size_t)*(char *)(uVar5 + 0x1f);
    if ((long)sVar8 < 0) {
      sVar8 = *(ulong *)(uVar5 + 0x10);
      if (0x7fffffffffffffee < sVar8) {
                    /* WARNING: Subroutine does not return */
        FUN_04b089c0();
      }
      pcVar9 = *(char **)(uVar5 + 8);
    }
    else {
      pcVar9 = (char *)(uVar5 + 8);
    }
    if (0x16 < sVar8) goto LAB_044026a8;
LAB_044026d4:
    local_60 = CONCAT17((char)sVar8,(undefined7)local_60);
  }
  else {
    uVar7 = 10;
    if ((uVar5 & 1) == 0) goto LAB_0440265c;
LAB_04402698:
    if (((uint)uVar5 >> 1 & 1) == 0) {
      sVar8 = 0;
      pcVar9 = (char *)0x0;
      goto LAB_044026d4;
    }
    pcVar9 = "Status accessed after move.";
    sVar8 = 0x1b;
LAB_044026a8:
    ppcVar3 = (char **)FUN_00008790((sVar8 & 0x7ffffffffffffff0) + 0x10);
    local_60 = (sVar8 & 0x7ffffffffffffff0) + 0x8000000000000010;
    local_70 = (char *)ppcVar3;
    uStack_68 = sVar8;
  }
  if (((char *)((long)ppcVar3 + sVar8) < ppcVar3) ||
     ((ppcVar3 <= pcVar9 && (pcVar9 < (char *)((long)ppcVar3 + sVar8))))) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x4402804);
    (*pcVar2)();
  }
  _memmove(ppcVar3,pcVar9,sVar8);
  *(char *)((long)ppcVar3 + sVar8) = '\0';
  ppcVar3 = (char **)local_70;
  if (-1 < (long)local_60._7_1_) {
    ppcVar3 = &local_70;
  }
  uVar5 = uStack_68;
  if (-1 < (long)local_60) {
    uVar5 = (long)local_60._7_1_;
  }
  uStack_50 = FUN_01c391bc(ppcVar3,uVar5);
  if ((long)local_60 < 0) {
    FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
  }
  local_58 = *(undefined8 *)PTR__NSLocalizedDescriptionKey_0d826478;
  IVar4 = NSDictionary::dictionaryWithObjects_forKeys_count_
                    ((ID)&_OBJC_CLASS___NSDictionary,"dictionaryWithObjects:forKeys:count:",
                     &uStack_50,&local_58,1);
  IVar4 = NSError::errorWithDomain_code_userInfo_
                    ((ID)&_OBJC_CLASS___NSError,"errorWithDomain:code:userInfo:",
                     &cf_company_thebrowser_Arc,uVar7,IVar4);
  param_1 = (ulong *)0x0;
LAB_044027cc:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(param_1);
  }
  return IVar4;
}


