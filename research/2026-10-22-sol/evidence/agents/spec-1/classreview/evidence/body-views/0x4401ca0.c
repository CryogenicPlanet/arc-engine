// ==== 0x4401ca0  objc::Categories::ArcCommandBarRankingExample_Extensions_::input  (452 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcCommandBarRankingExample_Extensions_::input(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  ID self;
  ID IVar3;
  long lVar4;
  undefined4 *puVar5;
  long lVar6;
  long lVar7;
  undefined4 *local_90;
  ulong local_88;
  long local_80;
  long local_78;
  long local_70;
  long local_68;
  byte local_58;
  
  _objc_msgSend(param_1,"command_bar_ranking_example",&local_90);
  if ((local_58 & 1) != 0) {
    IVar2 = _objc_alloc(&class_t::ArcCommandBarRankingInput);
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_88);
    puVar5 = local_90;
    for (lVar7 = local_88 << 2; lVar7 != 0; lVar7 = lVar7 + -4) {
      IVar3 = NSNumber::numberWithFloat_((ID)&_OBJC_CLASS___NSNumber,"numberWithFloat:",*puVar5);
      NSMutableArray::addObject_(self,"addObject:",IVar3);
      puVar5 = puVar5 + 1;
    }
    IVar2 = ArcCommandBarRankingInput::init_(IVar2,"init:",self);
    lVar7 = local_78;
    if (local_58 != 0) {
      if (local_78 != 0) {
        if (local_70 * 0xa8 != 0) {
          lVar4 = local_70 * 0xa8 + local_78;
          lVar6 = local_70 * -0xa8;
          do {
            if (lVar4 == 0xa8) goto LAB_04401e74;
            lVar4 = FUN_042a3ff4();
            lVar6 = lVar6 + 0xa8;
          } while (lVar6 != 0);
        }
        local_70 = (lVar7 - local_78 >> 3) * -0x30c30c30c30c30c3;
        FUN_00005810(local_78,local_68 * 0xa8);
      }
      if (local_90 != (undefined4 *)0x0) {
        if ((local_88 & 0x3fffffffffffffff) != 0) {
          lVar7 = local_88 * -4;
          puVar5 = local_90 + local_88;
          do {
            puVar5 = puVar5 + -1;
            if (puVar5 == (undefined4 *)0x0) goto LAB_04401e74;
            lVar7 = lVar7 + 4;
          } while (lVar7 != 0);
        }
        local_88 = 0;
        FUN_00005810(local_90,local_80 << 2);
      }
    }
    return IVar2;
  }
LAB_04401e74:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4401e78);
  (*pcVar1)();
}


