// ==== 0x4406c00  ArcLocalState::getList:error:  (464 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcLocalState::getList_error_(ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  long lVar1;
  int ****ppppiVar2;
  code *pcVar3;
  uint uVar4;
  ID IVar5;
  undefined8 uVar6;
  int ***pppiVar7;
  long lVar8;
  int **local_80;
  int ***local_78;
  long lStack_70;
  undefined8 local_68;
  int **local_60;
  long lStack_58;
  long local_50;
  long local_48;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4406db8);
    (*pcVar3)();
  }
  lStack_58 = 0;
  local_60 = (int **)0x0;
  local_48 = 0;
  local_50 = 0;
  if (param_3 == 0) {
    local_78 = (int ***)0x0;
    lStack_70 = 0;
    local_68 = 0;
  }
  else {
    FUN_01b356b8(&local_78,param_3);
  }
  ppppiVar2 = (int ****)local_78;
  if (-1 < (long)local_68._7_1_) {
    ppppiVar2 = &local_78;
  }
  lVar1 = lStack_70;
  if (-1 < (long)local_68) {
    lVar1 = (long)local_68._7_1_;
  }
  FUN_04326084(&local_60,*(undefined8 *)(param_1 + 8),ppppiVar2,lVar1);
  if ((long)local_68 < 0) {
    FUN_00005810(local_78,local_68 & 0x7fffffffffffffff);
  }
  if ((int ***)local_60 == (int ***)((long)&MACH_HEADER.magic + 1)) {
    IVar5 = _objc_alloc_init(&_OBJC_CLASS___NSMutableArray);
    if ((int ***)local_60 != (int ***)((long)&MACH_HEADER.magic + 1)) {
      local_78 = (int ***)local_60;
      pppiVar7 = (int ***)local_60;
      if (((ulong)local_60 & 1) == 0) {
        uVar4 = *(uint *)local_60;
        *(uint *)local_60 = uVar4 + 1;
        pppiVar7 = (int ***)(ulong)uVar4;
      }
                    /* WARNING: Subroutine does not return */
      FUN_04b37ee8(pppiVar7,&local_78);
    }
    lVar1 = lStack_58;
    for (lVar8 = local_50 << 5; lVar8 != 0; lVar8 = lVar8 + -0x20) {
      uVar6 = FUN_04406e44(lVar1);
      NSMutableArray::addObject_(IVar5,"addObject:",uVar6);
      lVar1 = lVar1 + 0x20;
    }
  }
  else {
    local_80 = local_60;
    pppiVar7 = (int ***)local_60;
    if (((ulong)local_60 & 1) == 0) {
      uVar4 = *(uint *)local_60;
      *(uint *)local_60 = uVar4 + 1;
      pppiVar7 = (int ***)(ulong)uVar4;
    }
    IVar5 = FUN_044025f4(pppiVar7,&local_80);
    *param_4 = IVar5;
    if (((ulong)local_80 & 1) == 0) {
      FUN_04b35898();
    }
    IVar5 = 0;
  }
  if ((int ***)local_60 == (int ***)((long)&MACH_HEADER.magic + 1)) {
    if (lStack_58 != 0) {
      FUN_0433b47c((ulong)&local_60 | 8);
      FUN_00005810(lStack_58,local_48 << 5);
    }
  }
  else if (((ulong)local_60 & 1) == 0) {
    FUN_04b35898();
  }
  return IVar5;
}


