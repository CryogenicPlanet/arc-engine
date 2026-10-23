// ==== 0x43ecc2c  ArcBrowserContextAutocomplete::autocomplete:cursorPosition:preventInlineAutocomplete:completion:  (392 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x2c bytes */

void ArcBrowserContextAutocomplete::
     autocomplete_cursorPosition_preventInlineAutocomplete_completion_
               (ID param_1,SEL param_2,ID param_3,long_long param_4,bool param_5,ID param_6,
               undefined4 param_7)

{
  ID *pIVar1;
  ID *pIVar2;
  undefined **ppuVar3;
  long *plVar4;
  undefined8 local_90;
  undefined8 uStack_88;
  ulong local_80;
  ID *local_78;
  undefined8 uStack_70;
  code *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  pIVar1 = (ID *)FUN_00008790(8);
  *pIVar1 = param_6;
  plVar4 = *(long **)(param_1 + 8);
  if (param_3 == 0) {
    local_90 = 0;
    uStack_88 = 0;
    local_80 = 0;
  }
  else {
    FUN_01b356b8(&local_90,param_3);
  }
  pIVar2 = (ID *)FUN_00008790(8);
  *pIVar2 = *pIVar1;
  ppuVar3 = &PTR_FUN_0df6b868;
  if (*plVar4 != 0) {
    uStack_70 = 0;
    local_68 = FUN_043eceb0;
    local_60 = &PTR_FUN_0df6b868;
    local_78 = pIVar2;
    FUN_042c35e4(*plVar4,&local_90,param_4,param_5,&local_78);
    if ((code *)local_60[1] != (code *)0x0) {
      (*(code *)local_60[1])(local_78);
    }
    ppuVar3 = (undefined **)&DAT_0c4b3500;
  }
  if ((code *)ppuVar3[1] != (code *)0x0) {
    (*(code *)ppuVar3[1])(pIVar2);
  }
  if ((long)local_80 < 0) {
    FUN_00005810(local_90,local_80 & 0x7fffffffffffffff);
  }
  if (pIVar1 != (ID *)0x0) {
    FUN_00005810(pIVar1,8);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


