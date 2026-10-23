// ==== 0x99f007c  ICloudKeychainCreateController::setRequest:  (68 bytes)

/* Function Stack Size: 0x308 bytes */

void ICloudKeychainCreateController::setRequest_
               (ID param_1,SEL param_2,CtapMakeCredentialRequest *param_3)

{
  code *pcVar1;
  long lVar2;
  
  lVar2 = param_1 + (long)request_;
  if (*(char *)(lVar2 + 0x2f8) != '\0') {
    FUN_09a0fb60(lVar2,param_3);
    return;
  }
  if (lVar2 != 0) {
    lVar2 = thunk_FUN_09a0f950(lVar2,param_3);
    *(undefined1 *)(lVar2 + 0x2f8) = 1;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x99f00c0);
  (*pcVar1)();
}


