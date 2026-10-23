// ==== 0x440be50  ArcSpake2Context::generateMessageWithPassword:error:  (556 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcSpake2Context::generateMessageWithPassword_error_
             (ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  code *pcVar1;
  ID IVar2;
  ID IVar3;
  long lVar4;
  long local_80;
  ulong uStack_78;
  undefined8 local_70;
  ulong local_68;
  undefined8 local_58;
  cfstringStruct *pcStack_50;
  undefined8 local_48;
  cfstringStruct *local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  lVar4 = *(long *)(param_1 + 8);
  if (lVar4 == 0) {
    if (param_4 != (ID *)0x0) {
      local_48 = *(undefined8 *)PTR__NSLocalizedDescriptionKey_0d826478;
      local_40 = &cf_SPAKE2contextisinvalid;
      IVar2 = NSDictionary::dictionaryWithObjects_forKeys_count_
                        ((ID)&_OBJC_CLASS___NSDictionary,"dictionaryWithObjects:forKeys:count:",
                         &local_40,&local_48,1);
      IVar2 = NSError::errorWithDomain_code_userInfo_
                        ((ID)&_OBJC_CLASS___NSError,"errorWithDomain:code:userInfo:",
                         &cf_company_thebrowser_Arc_Spake2,0,IVar2);
      *param_4 = IVar2;
      param_4 = (ID *)0x0;
    }
  }
  else {
    uStack_78 = 0;
    local_80 = 0;
    local_68 = 0;
    local_70 = 0;
    IVar2 = _objc_retainAutorelease(param_3);
    IVar2 = _objc_msgSend(IVar2,"bytes");
    IVar3 = _objc_msgSend(param_3,"length");
    FUN_04341c4c(&local_80,lVar4,IVar2,IVar3);
    if ((local_68 & 1) == 0) {
      if (param_4 != (ID *)0x0) {
        local_58 = *(undefined8 *)PTR__NSLocalizedDescriptionKey_0d826478;
        pcStack_50 = &cf_SPAKE2messagegenerationfailed;
        IVar2 = NSDictionary::dictionaryWithObjects_forKeys_count_
                          ((ID)&_OBJC_CLASS___NSDictionary,"dictionaryWithObjects:forKeys:count:",
                           &pcStack_50,&local_58,1);
        IVar2 = NSError::errorWithDomain_code_userInfo_
                          ((ID)&_OBJC_CLASS___NSError,"errorWithDomain:code:userInfo:",
                           &cf_company_thebrowser_Arc_Spake2,0,IVar2);
        *param_4 = IVar2;
        param_4 = (ID *)0x0;
      }
    }
    else {
      param_4 = (ID *)NSData::dataWithBytes_length_
                                ((ID)&_OBJC_CLASS___NSData,"dataWithBytes:length:",local_80,
                                 uStack_78);
    }
    if (((char)local_68 != '\0') && (local_80 != 0)) {
      if ((ulong)-local_80 < uStack_78) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(1,0x440bfa8);
        (*pcVar1)();
      }
      uStack_78 = 0;
      FUN_00005810(local_80,local_70);
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return (ID)param_4;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


