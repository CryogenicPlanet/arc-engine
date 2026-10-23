// ==== 0x99f0494  ICloudKeychainGetController::_requestContextWithRequests:error:  (284 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ICloudKeychainGetController::_requestContextWithRequests_error_
             (ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  code *pcVar1;
  ID self;
  ID IVar2;
  ID IVar3;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ICloudKeychainGetController;
  local_40 = param_1;
  self = ASAuthorizationController::_requestContextWithRequests_error_
                   ((ID)&local_40,"_requestContextWithRequests:error:",param_3,param_4);
  IVar2 = ASAuthorizationController::platformKeyCredentialAssertionOptions
                    (self,"platformKeyCredentialAssertionOptions");
  if ((*(byte *)(param_1 + (long)request_ + 0x1d8) & 1) != 0) {
    IVar3 = NSData::dataWithBytes_length_
                      ((ID)&_OBJC_CLASS___NSData,"dataWithBytes:length:",
                       param_1 + (long)request_ + 0x30,0x20);
    ASAuthorizationController::setClientDataHash_(IVar2,"setClientDataHash:",IVar3);
    IVar2 = ASAuthorizationController::copyWithZone_(IVar2,"copyWithZone:",0);
    ASAuthorizationController::setPlatformKeyCredentialAssertionOptions_
              (self,"setPlatformKeyCredentialAssertionOptions:",IVar2);
    return self;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x99f05b0);
  (*pcVar1)();
}


