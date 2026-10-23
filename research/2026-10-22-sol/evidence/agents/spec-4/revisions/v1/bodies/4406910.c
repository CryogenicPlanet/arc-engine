// ==== 0x4406910  ArcLocalState::getDictionary:error:  (572 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcLocalState::getDictionary_error_(ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  undefined8 *puVar1;
  undefined8 *****pppppuVar2;
  long lVar3;
  char cVar4;
  code *pcVar5;
  uint uVar6;
  ID IVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  uint *puVar10;
  undefined8 *puVar11;
  long lVar12;
  uint *local_90;
  undefined8 ****local_88;
  long lStack_80;
  undefined8 local_78;
  uint *local_70;
  undefined8 *puStack_68;
  long local_60;
  long local_58;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    puStack_68 = (undefined8 *)0x0;
    local_70 = (uint *)0x0;
    local_58 = 0;
    local_60 = 0;
    if (param_3 == 0) {
      local_88 = (undefined8 *****)0x0;
      lStack_80 = 0;
      local_78 = 0;
    }
    else {
      FUN_01b356b8(&local_88,param_3);
    }
    pppppuVar2 = (undefined8 *****)local_88;
    if (-1 < (long)local_78._7_1_) {
      pppppuVar2 = &local_88;
    }
    lVar12 = lStack_80;
    if (-1 < (long)local_78) {
      lVar12 = (long)local_78._7_1_;
    }
    FUN_04325fcc(&local_70,*(undefined8 *)(param_1 + 8),pppppuVar2,lVar12);
    if ((long)local_78 < 0) {
      FUN_00005810(local_88,local_78 & 0x7fffffffffffffff);
    }
    if (local_70 == (uint *)((long)&MACH_HEADER.magic + 1)) {
      IVar7 = _objc_alloc_init(&_OBJC_CLASS___NSMutableDictionary);
      puVar1 = puStack_68;
      for (lVar12 = local_60 * 0x38; lVar12 != 0; lVar12 = lVar12 + -0x38) {
        uVar8 = FUN_04406e44(puVar1 + 3);
        cVar4 = *(char *)((long)puVar1 + 0x17);
        puVar11 = (undefined8 *)*puVar1;
        if (-1 < (long)cVar4) {
          puVar11 = puVar1;
        }
        lVar3 = puVar1[1];
        if (-1 < cVar4) {
          lVar3 = (long)cVar4;
        }
        uVar9 = FUN_01c391bc(puVar11,lVar3);
        NSMutableDictionary::setObject_forKey_(IVar7,"setObject:forKey:",uVar8,uVar9);
        puVar1 = puVar1 + 7;
      }
    }
    else {
      local_90 = local_70;
      puVar10 = local_70;
      if (((ulong)local_70 & 1) == 0) {
        uVar6 = *local_70;
        *local_70 = uVar6 + 1;
        puVar10 = (uint *)(ulong)uVar6;
      }
      IVar7 = FUN_044025f4(puVar10,&local_90);
      *param_4 = IVar7;
      if (((ulong)local_90 & 1) == 0) {
        FUN_04b35898();
      }
      IVar7 = 0;
    }
    puVar1 = puStack_68;
    if (local_70 == (uint *)((long)&MACH_HEADER.magic + 1)) {
      if (puStack_68 != (undefined8 *)0x0) {
        if (local_60 * 0x38 != 0) {
          lVar12 = local_60 * -0x38;
          puVar11 = puStack_68 + local_60 * 7;
          do {
            puVar11 = puVar11 + -7;
            FUN_0433ba7c(puVar11);
            lVar12 = lVar12 + 0x38;
          } while (lVar12 != 0);
        }
        local_60 = ((long)puVar1 - (long)puStack_68 >> 3) * 0x6db6db6db6db6db7;
        FUN_00005810(puStack_68,local_58 * 0x38);
      }
    }
    else if (((ulong)local_70 & 1) == 0) {
      FUN_04b35898();
    }
    return IVar7;
  }
                    /* WARNING: Does not return */
  pcVar5 = (code *)SoftwareBreakpoint(1,0x4406b78);
  (*pcVar5)();
}


