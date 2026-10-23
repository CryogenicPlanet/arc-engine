// ==== 0x4325ac8  FUN_04325ac8  (544 bytes)

void FUN_04325ac8(undefined8 *param_1,long *param_2,undefined8 *****param_3,ulong param_4,
                 undefined8 *param_5)

{
  code *pcVar1;
  undefined8 *****pppppuVar2;
  undefined8 *puVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined8 ****local_d0;
  ulong uStack_c8;
  undefined8 local_c0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 *local_98 [2];
  code *local_88;
  undefined **ppuStack_80;
  code *local_68;
  undefined *puStack_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  if (0x7fffffffffffffee < param_4) {
                    /* WARNING: Subroutine does not return */
    FUN_04b089c0();
  }
  pppppuVar2 = &local_d0;
  if (param_4 < 0x17) {
    local_c0 = CONCAT17((char)param_4,(undefined7)local_c0);
  }
  else {
    pppppuVar2 = (undefined8 *****)FUN_00008790((param_4 & 0x7ffffffffffffff0) + 0x10);
    local_c0 = (param_4 & 0x7ffffffffffffff0) + 0x8000000000000010;
    local_d0 = pppppuVar2;
    uStack_c8 = param_4;
  }
  if ((pppppuVar2 <= (undefined8 *****)((long)pppppuVar2 + param_4)) &&
     ((undefined8 *****)((long)pppppuVar2 + param_4) <= param_3 || param_3 < pppppuVar2)) {
    _memmove(pppppuVar2,param_3,param_4);
    *(undefined1 *)((long)pppppuVar2 + param_4) = 0;
    uStack_b0 = param_5[1];
    local_b8 = *param_5;
    uVar6 = param_5[3];
    uVar4 = param_5[2];
    puVar5 = (undefined8 *)param_5[3];
    if ((code *)*puVar5 != (code *)0x0) {
      local_b8 = (*(code *)*puVar5)(*param_5);
    }
    local_88 = FUN_043263d4;
    ppuStack_80 = &PTR_FUN_0df6acc8;
    puVar3 = (undefined8 *)FUN_00008790(0x40);
    *puVar3 = param_2;
    puVar3[2] = uStack_c8;
    puVar3[1] = local_d0;
    puVar3[3] = local_c0;
    puVar3[5] = uStack_b0;
    puVar3[4] = local_b8;
    puVar3[7] = uVar6;
    puVar3[6] = uVar4;
    if (puVar5[1] != 0) {
      puVar5 = &DAT_0c4b3500;
    }
    local_68 = FUN_0836d86c;
    puStack_60 = &DAT_0c9d6a20;
    local_98[0] = puVar3;
    if (*param_2 == 0) {
      uVar4 = FUN_04b37104(0x39,"This object is detached",0x17,0x7e,
                           "../../arc/browser/local_state_impl.cc");
      *param_1 = uVar4;
    }
    else {
      FUN_043263d4(param_1,local_98);
    }
    if (puVar3 != (undefined8 *)0x0) {
      if (*(code **)(puVar3[7] + 8) != (code *)0x0) {
        (**(code **)(puVar3[7] + 8))(puVar3[4]);
      }
      if (*(char *)((long)puVar3 + 0x1f) < '\0') {
        FUN_00005810(puVar3[1],puVar3[3] & 0x7fffffffffffffff);
      }
      FUN_00005810(puVar3,0x40);
    }
    if ((code *)puVar5[1] != (code *)0x0) {
      (*(code *)puVar5[1])(local_b8);
    }
    if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
      return;
    }
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail();
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4325ce0);
  (*pcVar1)();
}


