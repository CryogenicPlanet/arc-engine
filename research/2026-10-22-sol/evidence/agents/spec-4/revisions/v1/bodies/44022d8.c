// ==== 0x44022d8  FUN_044022d8  (424 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */

ID FUN_044022d8(undefined8 *param_1)

{
  undefined8 *puVar1;
  char cVar2;
  ID IVar3;
  ID IVar4;
  ID self;
  ID IVar5;
  ID IVar6;
  long lVar7;
  undefined4 *puVar8;
  uint *puVar9;
  
  IVar3 = _objc_alloc(&objc::class_t::ArcCommandBarRankingTensor);
  cVar2 = *(char *)((long)param_1 + 0x17);
  puVar1 = (undefined8 *)*param_1;
  if (-1 < (long)cVar2) {
    puVar1 = param_1;
  }
  lVar7 = param_1[1];
  if (-1 < cVar2) {
    lVar7 = (long)cVar2;
  }
  IVar4 = FUN_01c391bc(puVar1,lVar7);
  self = NSMutableArray::arrayWithCapacity_
                   ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",param_1[4]);
  lVar7 = param_1[4] << 2;
  if (lVar7 != 0) {
    puVar9 = (uint *)param_1[3];
    do {
      IVar5 = NSNumber::numberWithInt_((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:",(ulong)*puVar9);
      NSMutableArray::addObject_(self,"addObject:",IVar5);
      lVar7 = lVar7 + -4;
      puVar9 = puVar9 + 1;
    } while (lVar7 != 0);
  }
  IVar5 = NSMutableArray::arrayWithCapacity_
                    ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",param_1[7]);
  lVar7 = param_1[7] << 2;
  if (lVar7 != 0) {
    puVar8 = (undefined4 *)param_1[6];
    do {
      IVar6 = NSNumber::numberWithFloat_((ID)&_OBJC_CLASS___NSNumber,"numberWithFloat:",*puVar8);
      NSMutableArray::addObject_(IVar5,"addObject:",IVar6);
      lVar7 = lVar7 + -4;
      puVar8 = puVar8 + 1;
    } while (lVar7 != 0);
  }
  IVar3 = ArcCommandBarRankingTensor::init_shape_data_(IVar3,"init:shape:data:",IVar4,self,IVar5);
  return IVar3;
}


