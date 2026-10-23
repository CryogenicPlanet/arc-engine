// ==== 0x440751c  ArcMediaImage::src  (148 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcMediaImage::src(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  undefined8 *puVar3;
  ID IVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  puVar3 = (undefined8 *)FUN_007cf864(*(undefined8 *)(param_1 + 0x10));
  if (*(char *)((long)puVar3 + 0x17) < '\0') {
    FUN_00060a60(&local_40,*puVar3,puVar3[1]);
  }
  else {
    lStack_38 = puVar3[1];
    local_40 = (undefined1 *)*puVar3;
    local_30 = puVar3[2];
  }
  ppuVar1 = (undefined1 **)local_40;
  if (-1 < (long)local_30._7_1_) {
    ppuVar1 = &local_40;
  }
  lVar2 = lStack_38;
  if (-1 < (long)local_30) {
    lVar2 = (long)local_30._7_1_;
  }
  IVar4 = FUN_01c391bc(ppuVar1,lVar2);
  if ((long)local_30 < 0) {
    FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
  }
  return IVar4;
}


