// ==== 0x43eca0c  ArcBrowserApplicationConfiguration::init:argv:profilePath:applicationID:isProduction:sessionRestorationPolicy:shouldLoadAllProfilesAtStartup:quitRunLoopOnTermination:userDefaultsSuiteName:  (248 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x48 bytes */

ID ArcBrowserApplicationConfiguration::
   init_argv_profilePath_applicationID_isProduction_sessionRestorationPolicy_shouldLoadAllProfilesAtStartup_quitRunLoopOnTermination_userDefaultsSuiteName_
             (ID param_1,SEL param_2,int param_3,ID param_4,ID param_5,unsigned_long_long param_6,
             bool param_7,unsigned_long_long param_8,bool param_9,bool param_10,ID param_11)

{
  ID IVar1;
  undefined4 in_stack_0000000c;
  ID local_70;
  class_t *pcStack_68;
  
  pcStack_68 = &objc::class_t::ArcBrowserApplicationConfiguration;
  local_70 = param_1;
  IVar1 = NSObject::init((ID)&local_70,"init");
  if (IVar1 != 0) {
    *(int *)(IVar1 + 0xc) = param_3;
    *(ID *)(IVar1 + 0x10) = param_4;
    *(ID *)(IVar1 + 0x18) = param_5;
    *(char *)(IVar1 + 8) = (char)param_7;
    *(unsigned_long_long *)(IVar1 + 0x20) = param_6;
    *(unsigned_long_long *)(IVar1 + 0x28) = param_8;
    *(undefined1 *)(IVar1 + 9) = (undefined1)param_9;
    *(undefined1 *)(IVar1 + 10) = param_9._1_1_;
    *(ulong *)(IVar1 + 0x30) = CONCAT44(in_stack_0000000c,param_10);
  }
  return IVar1;
}


