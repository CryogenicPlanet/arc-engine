// ==== 0x67080b4  FUN_067080b4  (200 bytes)

void FUN_067080b4(long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 param_5)

{
  code *pcVar1;
  ulong uVar2;
  undefined8 uVar3;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  uVar2 = FUN_0681228c();
  if ((uVar2 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x670817c);
    (*pcVar1)();
  }
  uVar3 = *(undefined8 *)(param_1 + 0x10);
  FUN_021beb38(&local_98,param_4);
  local_60 = 0;
  uStack_58 = 0;
  uStack_78 = uStack_90;
  local_80 = local_98;
  local_70 = local_88;
  uStack_68 = 0;
  FUN_021beb38(&uStack_68,param_5);
  FUN_06811ac4(uVar3,param_2,param_3,&local_80,2);
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


