// ==== 0x440c710  ArcTracingConfiguration::filePath  (84 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcTracingConfiguration::filePath(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  undefined8 uVar4;
  
  cVar3 = *(char *)(param_1 + 0x2f);
  puVar1 = *(undefined8 **)(param_1 + 0x18);
  if (-1 < (long)cVar3) {
    puVar1 = (undefined8 *)(param_1 + 0x18);
  }
  lVar2 = *(long *)(param_1 + 0x20);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  uVar4 = FUN_01c391bc(puVar1,lVar2);
  *(undefined8 *)(param_1 + 8) = uVar4;
  return *(ID *)(param_1 + 8);
}


