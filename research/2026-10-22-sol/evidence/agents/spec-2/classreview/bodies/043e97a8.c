// ==== 0x43e97a8  ArcBrowserApplication::setAllFlagsToDefault:  (160 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setAllFlagsToDefault_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  ID *pIVar1;
  undefined4 *puVar2;
  
  pIVar1 = (ID *)FUN_00008790(8,param_2,param_3,param_4);
  *pIVar1 = param_3;
  puVar2 = (undefined4 *)FUN_00008790(0x48);
  *puVar2 = 1;
  *(undefined1 **)(puVar2 + 2) = &LAB_042ae510;
  *(code **)(puVar2 + 4) = FUN_041a82e0;
  *(code **)(puVar2 + 6) = FUN_00b77080;
  *(ID **)(puVar2 + 10) = pIVar1;
  *(undefined1 **)(puVar2 + 0xe) = &LAB_043ebf18;
  *(undefined ***)(puVar2 + 0x10) = &PTR_FUN_0df6b808;
  FUN_02e7bb1c(0,puVar2);
  return;
}


