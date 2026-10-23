// ==== 0x4406e44  FUN_04406e44  (576 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

ID FUN_04406e44(uint *param_1)

{
  undefined8 *puVar1;
  uint *puVar2;
  byte bVar3;
  char cVar4;
  ID IVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  char *op;
  ulong uVar8;
  long lVar9;
  long lVar10;
  undefined8 *puVar11;
  
  bVar3 = (byte)param_1[6];
  if (bVar3 < 4) {
    if (bVar3 == 1) {
      uVar8 = (ulong)(byte)*param_1;
      op = "numberWithBool:";
    }
    else {
      if (bVar3 != 2) {
        if (bVar3 == 3) {
          IVar5 = NSNumber::numberWithDouble_
                            ((ID)&_OBJC_CLASS___NSNumber,"numberWithDouble:",*(undefined8 *)param_1)
          ;
          return IVar5;
        }
        goto LAB_04406fc8;
      }
      uVar8 = (ulong)*param_1;
      op = "numberWithInt:";
    }
    IVar5 = NSNumber::numberWithBool_((ID)&_OBJC_CLASS___NSNumber,op,uVar8);
  }
  else {
    if (bVar3 < 6) {
      if (bVar3 == 4) {
        cVar4 = *(char *)((long)param_1 + 0x17);
        puVar2 = *(uint **)param_1;
        if (-1 < (long)cVar4) {
          puVar2 = param_1;
        }
        lVar10 = *(long *)(param_1 + 2);
        if (-1 < cVar4) {
          lVar10 = (long)cVar4;
        }
        IVar5 = FUN_01c391bc(puVar2,lVar10);
        return IVar5;
      }
      if (bVar3 == 5) {
        IVar5 = _objc_alloc_init(&_OBJC_CLASS___NSMutableDictionary);
        lVar10 = *(long *)(param_1 + 2) * 0x38;
        if (lVar10 == 0) {
          return IVar5;
        }
        puVar11 = *(undefined8 **)param_1;
        do {
          uVar6 = FUN_04406e44(puVar11 + 3);
          cVar4 = *(char *)((long)puVar11 + 0x17);
          puVar1 = (undefined8 *)*puVar11;
          if (-1 < (long)cVar4) {
            puVar1 = puVar11;
          }
          lVar9 = puVar11[1];
          if (-1 < cVar4) {
            lVar9 = (long)cVar4;
          }
          uVar7 = FUN_01c391bc(puVar1,lVar9);
          NSMutableDictionary::setObject_forKey_(IVar5,"setObject:forKey:",uVar6,uVar7);
          puVar11 = puVar11 + 7;
          lVar10 = lVar10 + -0x38;
        } while (lVar10 != 0);
        return IVar5;
      }
    }
    else {
      if (bVar3 == 6) {
        IVar5 = _objc_alloc_init(&_OBJC_CLASS___NSMutableArray);
        if ((char)param_1[6] != '\x06') {
          if ((char)param_1[6] == -1) {
                    /* WARNING: Subroutine does not return */
            FUN_04b335b8("std::get: variant is valueless");
          }
                    /* WARNING: Subroutine does not return */
          FUN_04b335b8("std::get: wrong alternative for variant");
        }
        lVar10 = *(long *)(param_1 + 2) << 5;
        if (lVar10 == 0) {
          return IVar5;
        }
        lVar9 = *(long *)param_1;
        do {
          uVar6 = FUN_04406e44(lVar9);
          NSMutableArray::addObject_(IVar5,"addObject:",uVar6);
          lVar9 = lVar9 + 0x20;
          lVar10 = lVar10 + -0x20;
        } while (lVar10 != 0);
        return IVar5;
      }
      if (bVar3 == 0xff) {
                    /* WARNING: Subroutine does not return */
        FUN_04b335b8("std::visit: variant is valueless");
      }
    }
LAB_04406fc8:
    IVar5 = NSNull::null((ID)&_OBJC_CLASS___NSNull,"null");
  }
  return IVar5;
}


