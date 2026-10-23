// ==== 0x40432b0  objc::Categories::ArcBrowserContextHistory_Extensions_::deleteUrlsFromHistory:completion:  (548 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::deleteUrlsFromHistory_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  undefined8 *puVar1;
  long lVar2;
  code *pcVar3;
  ID IVar4;
  undefined8 *puVar5;
  long lVar6;
  undefined1 auStack_c0 [16];
  byte local_b0;
  undefined *local_a8;
  undefined8 uStack_a0;
  undefined1 *local_98;
  undefined *puStack_90;
  long *local_88;
  long local_80;
  long local_78;
  ID local_70;
  ID *local_68 [2];
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  local_80 = 0;
  local_78 = 0;
  local_70 = 0;
  IVar4 = _objc_msgSend(param_3,"count",param_3,param_4,(ulong)param_5);
  if (IVar4 != 0) {
    if (0xaaaaaaaaaaaaaaa < IVar4) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    local_80 = FUN_00008790(IVar4 * 0x18);
    local_78 = 0;
    local_70 = IVar4;
  }
  local_a8 = PTR___NSConcreteGlobalBlock_0d824130;
  uStack_a0 = 0xd0800000;
  local_98 = &LAB_04045240;
  puStack_90 = &DAT_0da87b28;
  local_88 = &local_80;
  _objc_msgSend(param_3,"enumerateObjectsUsingBlock:",&local_a8);
  _objc_msgSend(param_1,"browser_context_history",auStack_c0);
  if ((local_b0 & 1) == 0) {
LAB_040434c4:
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x40434c8);
    (*pcVar3)();
  }
  local_58 = &LAB_04044e30;
  local_50 = &PTR_FUN_0da87ba8;
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_4;
  FUN_04332d28(auStack_c0,&local_80,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if (local_b0 != 0) {
    FUN_04332750(auStack_c0);
  }
  lVar2 = local_80;
  if (local_80 != 0) {
    if (local_78 * 0x18 != 0) {
      lVar6 = local_78 * -0x18;
      puVar1 = (undefined8 *)(local_78 * 0x18 + local_80);
      do {
        puVar5 = puVar1 + -3;
        if (puVar5 == (undefined8 *)0x0) goto LAB_040434c4;
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar5,puVar1[-1] & 0x7fffffffffffffff);
        }
        lVar6 = lVar6 + 0x18;
        puVar1 = puVar5;
      } while (lVar6 != 0);
    }
    local_78 = (lVar2 - local_80 >> 3) * -0x5555555555555555;
    FUN_00005810(local_80,local_70 * 0x18);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


