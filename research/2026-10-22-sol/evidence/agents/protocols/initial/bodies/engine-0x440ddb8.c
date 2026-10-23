// ==== 0x440ddb8  FUN_0440ddb8  (408 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8
FUN_0440ddb8(long param_1,undefined8 *param_2,undefined8 *param_3,undefined8 param_4,long param_5)

{
  long *plVar1;
  long lVar2;
  undefined8 *puVar3;
  char cVar4;
  ID IVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  ID self;
  long *plVar8;
  
  IVar5 = NSMutableDictionary::dictionary((ID)&_OBJC_CLASS___NSMutableDictionary,"dictionary");
  for (plVar8 = *(long **)(param_5 + 0x10); plVar8 != (long *)0x0; plVar8 = (long *)*plVar8) {
    cVar4 = *(char *)((long)plVar8 + 0x3f);
    plVar1 = (long *)plVar8[5];
    if (-1 < (long)cVar4) {
      plVar1 = plVar8 + 5;
    }
    lVar2 = plVar8[6];
    if (-1 < cVar4) {
      lVar2 = (long)cVar4;
    }
    uVar6 = FUN_01c391bc(plVar1,lVar2);
    cVar4 = *(char *)((long)plVar8 + 0x27);
    plVar1 = (long *)plVar8[2];
    if (-1 < (long)cVar4) {
      plVar1 = plVar8 + 2;
    }
    lVar2 = plVar8[3];
    if (-1 < cVar4) {
      lVar2 = (long)cVar4;
    }
    uVar7 = FUN_01c391bc(plVar1,lVar2);
    NSMutableDictionary::setValue_forKey_(IVar5,"setValue:forKey:",uVar6,uVar7);
  }
  self = *(ID *)(param_1 + 8);
  cVar4 = *(char *)((long)param_2 + 0x17);
  puVar3 = (undefined8 *)*param_2;
  if (-1 < (long)cVar4) {
    puVar3 = param_2;
  }
  lVar2 = param_2[1];
  if (-1 < cVar4) {
    lVar2 = (long)cVar4;
  }
  uVar6 = FUN_01c391bc(puVar3,lVar2);
  cVar4 = *(char *)((long)param_3 + 0x17);
  puVar3 = (undefined8 *)*param_3;
  if (-1 < (long)cVar4) {
    puVar3 = param_3;
  }
  lVar2 = param_3[1];
  if (-1 < cVar4) {
    lVar2 = (long)cVar4;
  }
  uVar7 = FUN_01c391bc(puVar3,lVar2);
  IVar5 = NSMutableDictionary::copy(IVar5,"copy");
  _objc_msgSend(self,"didReceiveHeaders:withFinalURL:withCode:withHeaders:",uVar6,uVar7,param_4,
                IVar5);
  return 0;
}


