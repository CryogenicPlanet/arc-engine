// ==== 0x43ea75c  ArcBrowserApplication::frameForDisplayId:error:  (180 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcBrowserApplication::frameForDisplayId_error_
             (ID param_1,SEL param_2,long_long param_3,ID *param_4)

{
  ID IVar1;
  ID IVar2;
  undefined8 local_38;
  undefined8 local_30;
  uint local_28;
  
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  FUN_042bdc48(&local_38,**(undefined8 **)(param_1 + 0x20),param_3);
  if ((local_28 & 1) == 0) {
    IVar1 = NSError::errorWithDomain_code_userInfo_
                      ((ID)&_OBJC_CLASS___NSError,"errorWithDomain:code:userInfo:",
                       &cf_company_thebrowser_Arc,3,0);
    IVar2 = 0;
    *param_4 = IVar1;
  }
  else {
    IVar2 = NSValue::valueWithRect_
                      ((ID)&_OBJC_CLASS___NSValue,"valueWithRect:",(double)(int)local_38,
                       (double)local_38._4_4_,(double)(int)local_30,(double)local_30._4_4_);
  }
  return IVar2;
}


