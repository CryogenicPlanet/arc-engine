// ==== 0x41b7c34  AppController::setMainWindowForTesting:  (12 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x18 bytes */

void AppController::setMainWindowForTesting_(ID param_1,SEL param_2,ID param_3)

{
  *(ID *)(param_1 + 0x90) = param_3;
  return;
}


