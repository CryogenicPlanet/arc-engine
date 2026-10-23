// ==== 0x43abd34  FUN_043abd34  (136 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */

undefined8 FUN_043abd34(undefined8 *param_1)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  undefined8 uVar4;
  undefined8 local_98 [2];
  undefined7 local_88;
  char cStack_81;
  undefined1 auStack_78 [80];
  undefined1 auStack_28 [8];
  
  cVar3 = *(char *)((long)param_1 + 0x17);
  puVar1 = (undefined8 *)*param_1;
  if (-1 < (long)cVar3) {
    puVar1 = param_1;
  }
  lVar2 = param_1[1];
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  FUN_00c28450(local_98,puVar1,lVar2);
  uVar4 = FUN_033ad4c8(local_98);
  FUN_000282bc(auStack_28,0);
  FUN_0002827c(auStack_78);
  if (cStack_81 < '\0') {
    FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  return uVar4;
}


