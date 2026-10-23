// ==== 0x4405b64  ArcImageRep::toNSImage  (68 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcImageRep::toNSImage(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  
  lVar1 = *(long *)(param_1 + 0x10);
  if (*(int *)(lVar1 + 0x1c0) == 0) {
    if (*(long *)(lVar1 + 0x1d0) != 0) goto LAB_04405b88;
  }
  else if (*(char *)(lVar1 + 0x1b8) != '\0') {
LAB_04405b88:
    FUN_022f4af4();
    IVar2 = FUN_085eac30();
    return IVar2;
  }
  return 0;
}


