// ==== 0x443e0b4  ArcWebContentsPDFController::hasBookmarks:error:  (308 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool ArcWebContentsPDFController::hasBookmarks_error_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4,ID *param_5)

{
  code *pcVar1;
  uint uVar2;
  bool bVar3;
  uint *puVar4;
  undefined8 uVar5;
  uint *local_78;
  uint *local_70;
  ID *local_68 [2];
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443e1e4);
    (*pcVar1)();
  }
  local_70 = (uint *)0x0;
  uVar5 = *(undefined8 *)(param_1 + 8);
  local_58 = &LAB_04045100;
  local_50 = &PTR_FUN_0df6c338;
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_3;
  FUN_0436c99c(&local_70,uVar5,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  local_78 = local_70;
  puVar4 = local_70;
  if (((ulong)local_70 & 1) == 0) {
    uVar2 = *local_70;
    *local_70 = uVar2 + 1;
    puVar4 = (uint *)(ulong)uVar2;
  }
  bVar3 = FUN_04402548(puVar4,&local_78,param_4);
  if (((ulong)local_78 & 1) == 0) {
    FUN_04b35898();
  }
  if (((ulong)local_70 & 1) == 0) {
    FUN_04b35898();
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return bVar3;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


