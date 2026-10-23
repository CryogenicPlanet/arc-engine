// ==== 0x440df50  FUN_0440df50  (216 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_0440df50(long param_1,undefined8 *param_2,undefined8 *param_3)

{
  char cVar1;
  undefined8 uVar2;
  ID IVar3;
  undefined8 *puVar4;
  long lVar5;
  ID self;
  
  self = *(ID *)(param_1 + 8);
  cVar1 = *(char *)((long)param_2 + 0x17);
  puVar4 = (undefined8 *)*param_2;
  if (-1 < (long)cVar1) {
    puVar4 = param_2;
  }
  lVar5 = param_2[1];
  if (-1 < cVar1) {
    lVar5 = (long)cVar1;
  }
  uVar2 = FUN_01c391bc(puVar4,lVar5);
  IVar3 = _objc_alloc(&_OBJC_CLASS___NSData);
  lVar5 = (long)*(char *)((long)param_3 + 0x17);
  puVar4 = param_3;
  if (lVar5 < 0) {
    lVar5 = param_3[1];
    puVar4 = (undefined8 *)*param_3;
  }
  IVar3 = NSData::initWithBytes_length_(IVar3,"initWithBytes:length:",puVar4,lVar5);
  _objc_msgSend(self,"didReceiveData:withData:",uVar2,IVar3);
  if (param_3 != (undefined8 *)0x0) {
    if (*(char *)((long)param_3 + 0x17) < '\0') {
      FUN_00005810(*param_3,param_3[2] & 0x7fffffffffffffff);
    }
    uVar2 = FUN_00005810(param_3,0x18);
    return uVar2;
  }
  return 0;
}


