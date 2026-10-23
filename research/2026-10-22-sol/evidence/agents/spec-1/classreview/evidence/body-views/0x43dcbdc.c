// ==== 0x43dcbdc  ArcAIServiceConfiguration::init:displayName:libraryPath:libraryVersion:appEnvironment:fullAppVersion:maxRetries:timeout:error:  (1048 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x54 bytes */

ID ArcAIServiceConfiguration::
   init_displayName_libraryPath_libraryVersion_appEnvironment_fullAppVersion_maxRetries_timeout_error_
             (ID param_1,SEL param_2,long_long param_3,ID param_4,ID param_5,ID param_6,ID param_7,
             ID param_8,int param_9,double param_10,ID *param_11)

{
  code *pcVar1;
  ID IVar2;
  ID IVar3;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  ID local_98;
  class_t *pcStack_90;
  undefined8 local_88;
  cfstringStruct *pcStack_80;
  long local_78;
  
  local_78 = *(long *)PTR____stack_chk_guard_0d824020;
  if (param_3 == 0xffffffffffffffff) {
    local_88 = *(undefined8 *)PTR__NSLocalizedDescriptionKey_0d826478;
    pcStack_80 = &cf_Invalidtype;
    IVar2 = NSDictionary::dictionaryWithObjects_forKeys_count_
                      ((ID)&_OBJC_CLASS___NSDictionary,"dictionaryWithObjects:forKeys:count:",
                       &pcStack_80,&local_88,1);
    IVar2 = NSError::errorWithDomain_code_userInfo_
                      ((ID)&_OBJC_CLASS___NSError,"errorWithDomain:code:userInfo:",
                       &cf_company_thebrowser_Arc,2,IVar2);
    *param_11 = IVar2;
    IVar2 = 0;
  }
  else {
    pcStack_90 = &objc::class_t::ArcAIServiceConfiguration;
    local_98 = param_1;
    IVar2 = NSObject::init((ID)&local_98,"init");
    if (IVar2 != 0) {
      if (*(char *)(IVar2 + 0x98) != '\0') {
        FUN_042a3344(IVar2 + 8);
      }
      *(undefined8 *)(IVar2 + 0x90) = 0;
      *(undefined8 *)(IVar2 + 0x88) = 0;
      *(undefined8 *)(IVar2 + 0x80) = 0;
      *(undefined8 *)(IVar2 + 0x78) = 0;
      *(undefined8 *)(IVar2 + 0x70) = 0;
      *(undefined8 *)(IVar2 + 0x68) = 0;
      *(undefined8 *)(IVar2 + 0x60) = 0;
      *(undefined8 *)(IVar2 + 0x58) = 0;
      *(undefined8 *)(IVar2 + 0x50) = 0;
      *(undefined8 *)(IVar2 + 0x48) = 0;
      *(undefined8 *)(IVar2 + 0x40) = 0;
      *(undefined8 *)(IVar2 + 0x38) = 0;
      *(undefined8 *)(IVar2 + 0x30) = 0;
      *(undefined8 *)(IVar2 + 0x28) = 0;
      *(undefined8 *)(IVar2 + 0x20) = 0;
      *(undefined8 *)(IVar2 + 0x18) = 0;
      *(undefined8 *)(IVar2 + 0x10) = 0;
      *(undefined8 *)(IVar2 + 8) = 0;
      *(undefined1 *)(IVar2 + 0x98) = 1;
      *(int *)(IVar2 + 8) = (int)param_3;
      if (param_4 == 0) {
        local_b0 = 0;
        uStack_a8 = 0;
        local_a0 = 0;
      }
      else {
        FUN_01b356b8(&local_b0,param_4);
      }
      if ((*(byte *)(IVar2 + 0x98) & 1) != 0) {
        if (*(char *)(IVar2 + 0x27) < '\0') {
          FUN_00005810(*(undefined8 *)(IVar2 + 0x10),*(ulong *)(IVar2 + 0x20) & 0x7fffffffffffffff);
        }
        *(undefined8 *)(IVar2 + 0x18) = uStack_a8;
        *(undefined8 *)(IVar2 + 0x10) = local_b0;
        *(undefined8 *)(IVar2 + 0x20) = local_a0;
        IVar3 = _objc_msgSend(param_5,"path");
        if (IVar3 == 0) {
          local_b0 = 0;
          uStack_a8 = 0;
          local_a0 = 0;
        }
        else {
          FUN_01b356b8(&local_b0,IVar3);
        }
        if ((*(byte *)(IVar2 + 0x98) & 1) != 0) {
          if (*(char *)(IVar2 + 0x3f) < '\0') {
            FUN_00005810(*(undefined8 *)(IVar2 + 0x28),*(ulong *)(IVar2 + 0x38) & 0x7fffffffffffffff
                        );
          }
          *(undefined8 *)(IVar2 + 0x30) = uStack_a8;
          *(undefined8 *)(IVar2 + 0x28) = local_b0;
          *(undefined8 *)(IVar2 + 0x38) = local_a0;
          if (param_6 == 0) {
            local_b0 = 0;
            uStack_a8 = 0;
            local_a0 = 0;
          }
          else {
            FUN_01b356b8(&local_b0,param_6);
          }
          if ((*(byte *)(IVar2 + 0x98) & 1) != 0) {
            if (*(char *)(IVar2 + 0x57) < '\0') {
              FUN_00005810(*(undefined8 *)(IVar2 + 0x40),
                           *(ulong *)(IVar2 + 0x50) & 0x7fffffffffffffff);
            }
            *(undefined8 *)(IVar2 + 0x48) = uStack_a8;
            *(undefined8 *)(IVar2 + 0x40) = local_b0;
            *(undefined8 *)(IVar2 + 0x50) = local_a0;
            if (param_7 == 0) {
              local_b0 = 0;
              uStack_a8 = 0;
              local_a0 = 0;
            }
            else {
              FUN_01b356b8(&local_b0,param_7);
            }
            if ((*(byte *)(IVar2 + 0x98) & 1) != 0) {
              if (*(char *)(IVar2 + 0x6f) < '\0') {
                FUN_00005810(*(undefined8 *)(IVar2 + 0x58),
                             *(ulong *)(IVar2 + 0x68) & 0x7fffffffffffffff);
              }
              *(undefined8 *)(IVar2 + 0x60) = uStack_a8;
              *(undefined8 *)(IVar2 + 0x58) = local_b0;
              *(undefined8 *)(IVar2 + 0x68) = local_a0;
              if (param_8 == 0) {
                local_b0 = 0;
                uStack_a8 = 0;
                local_a0 = 0;
              }
              else {
                FUN_01b356b8(&local_b0,param_8);
              }
              if ((*(byte *)(IVar2 + 0x98) & 1) != 0) {
                if (*(char *)(IVar2 + 0x87) < '\0') {
                  FUN_00005810(*(undefined8 *)(IVar2 + 0x70),
                               *(ulong *)(IVar2 + 0x80) & 0x7fffffffffffffff);
                  *(undefined8 *)(IVar2 + 0x78) = uStack_a8;
                  *(undefined8 *)(IVar2 + 0x70) = local_b0;
                  *(undefined8 *)(IVar2 + 0x80) = local_a0;
                  if ((*(byte *)(IVar2 + 0x98) & 1) == 0) goto LAB_043dcfd8;
                }
                else {
                  *(undefined8 *)(IVar2 + 0x78) = uStack_a8;
                  *(undefined8 *)(IVar2 + 0x70) = local_b0;
                  *(undefined8 *)(IVar2 + 0x80) = local_a0;
                }
                *(char *)(IVar2 + 0x88) = (char)param_9;
                *(double *)(IVar2 + 0x90) = param_10;
                goto LAB_043dce78;
              }
            }
          }
        }
      }
LAB_043dcfd8:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x43dcfdc);
      (*pcVar1)();
    }
  }
LAB_043dce78:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_78) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return IVar2;
}


