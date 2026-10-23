// ==== 0x440d29c  FUN_0440d29c  (140 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0440d29c(long param_1,undefined8 param_2,undefined8 *param_3)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  undefined8 uVar4;
  
  cVar3 = *(char *)((long)param_3 + 0x17);
  puVar1 = (undefined8 *)*param_3;
  if (-1 < (long)cVar3) {
    puVar1 = param_3;
  }
  lVar2 = param_3[1];
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  uVar4 = FUN_01c391bc(puVar1,lVar2);
  _objc_msgSend(*(ID *)(param_1 + 8),"tracingController:didCompleteTracing:",
                *(undefined8 *)(param_1 + 0x10),uVar4);
  return 0;
}


