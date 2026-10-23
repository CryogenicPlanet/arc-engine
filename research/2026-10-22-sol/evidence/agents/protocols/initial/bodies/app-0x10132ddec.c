// ==== 0x10132ddec  _TtCC15URLLoaderClient19URLLoaderClientImpl12DataDelegate::-[URLLoaderClientImpl.DataDelegate_didReceiveData:withData:]  (116 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void _TtCC15URLLoaderClient19URLLoaderClientImpl12DataDelegate::
     __URLLoaderClientImpl_DataDelegate_didReceiveData_withData__
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  Data DVar4;
  undefined7 extraout_var;
  
  DVar4 = Foundation::Data::__unconditionallyBridgeFromObjectiveC((Data)(char)param_4);
  puVar1 = (undefined8 *)(param_1 + data);
  uVar2 = *puVar1;
  uVar3 = puVar1[1];
  *puVar1 = CONCAT71(extraout_var,DVar4.orderedSpaceList);
  puVar1[1] = param_2;
  FUN_1000ac094(uVar2,uVar3);
  return;
}


