// ==== 0x43f4a54  objc::Categories::ArcBrowserContext_Extensions_::faviconTempFileForPage:iconTypes:sizeInPixels:fallbackToHost:completion:  (192 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x34 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     faviconTempFileForPage_iconTypes_sizeInPixels_fallbackToHost_completion_
               (ID param_1,SEL param_2,ID param_3,long_long param_4,long_long param_5,bool param_6,
               ID param_7,uint param_8)

{
  undefined *local_78;
  undefined8 uStack_70;
  undefined1 *local_68;
  undefined *puStack_60;
  ID local_58;
  
  local_78 = PTR___NSConcreteStackBlock_0d824120;
  uStack_70 = 0xc2000000;
  local_68 = &LAB_043f4b14;
  puStack_60 = &DAT_0df6b888;
  local_58 = param_7;
  _objc_msgSend(param_1,"favicon:iconTypes:sizeInPixels:fallbackToHost:completion:",param_3,param_4,
                param_5,(ulong)param_6,&local_78,(ulong)param_8);
  return;
}


