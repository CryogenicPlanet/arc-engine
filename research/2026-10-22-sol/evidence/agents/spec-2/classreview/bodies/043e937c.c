// ==== 0x43e937c  ArcBrowserApplication::getHistogramsJSON  (112 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::getHistogramsJSON(ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  ID IVar2;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  FUN_08385340(&local_38,0);
  pppuVar1 = (undefined8 ***)local_38;
  if (-1 < (long)cStack_21) {
    pppuVar1 = &local_38;
  }
  if (-1 < cStack_21) {
    lStack_30 = (long)cStack_21;
  }
  IVar2 = FUN_01c391bc(pppuVar1,lStack_30);
  if (cStack_21 < '\0') {
    FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
  }
  return IVar2;
}


