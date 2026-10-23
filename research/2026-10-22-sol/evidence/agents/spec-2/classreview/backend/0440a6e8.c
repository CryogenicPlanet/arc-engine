// ==== 0x440a6e8  FUN_0440a6e8  (160 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 * FUN_0440a6e8(undefined8 param_1)

{
  code *pcVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  int *piVar5;
  
  uVar3 = FUN_00008790(8);
  _objc_initWeak(uVar3,param_1);
  puVar4 = (undefined8 *)FUN_00008790(0x40);
  *puVar4 = &PTR_LAB_0da87e38;
  puVar4[1] = uVar3;
  puVar4[4] = 0;
  puVar4[5] = 0;
  puVar4[3] = 0;
  puVar4[2] = puVar4 + 3;
  piVar5 = (int *)FUN_00008790(8);
  *piVar5 = 0;
  *(undefined1 *)(piVar5 + 1) = 0;
  if ((piVar5 != (int *)0x0) && (iVar2 = *piVar5, *piVar5 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x440a7ac);
    (*pcVar1)();
  }
  puVar4[6] = piVar5;
  puVar4[7] = puVar4;
  return puVar4;
}


