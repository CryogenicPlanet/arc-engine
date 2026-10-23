// ==== 0x43f4fd8  objc::Categories::ArcBrowserContext_Extensions_::setFaviconData:forPage:iconURL:iconType:completion:  (372 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     setFaviconData_forPage_iconURL_iconType_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,long_long param_6,ID param_7
               ,undefined4 param_8)

{
  ID IVar1;
  ID IVar2;
  ID *local_88 [2];
  undefined1 *local_78;
  undefined **local_70;
  long local_68;
  
  local_68 = *(long *)PTR____stack_chk_guard_0d824020;
  IVar1 = _objc_retainAutorelease(param_3);
  IVar1 = _objc_msgSend(IVar1,"bytes");
  IVar2 = _objc_msgSend(param_3,"length");
  local_78 = &LAB_043f89b4;
  local_70 = &PTR_FUN_0df6ba88;
  local_88[0] = (ID *)FUN_00008790(0x28);
  *local_88[0] = param_1;
  local_88[0][1] = param_4;
  local_88[0][2] = param_5;
  local_88[0][3] = param_6;
  local_88[0][4] = param_7;
  FUN_04338920(0x3f800000,IVar1,IVar2,local_88,0,0,0);
  if ((code *)local_70[1] != (code *)0x0) {
    (*(code *)local_70[1])(local_88[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


