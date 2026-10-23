// ==== 0x436c99c  FUN_0436c99c  (400 bytes)

void FUN_0436c99c(undefined8 *param_1,long *param_2,undefined8 *param_3)

{
  long lVar1;
  undefined8 uVar2;
  char *pcVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  ulong uVar6;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  long local_48;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if (*param_2 == 0) {
    pcVar3 = "The object is in detached state";
    uVar2 = 0x25;
    uVar4 = 0x1f;
    uVar5 = 0x14;
LAB_0436caac:
    uVar2 = FUN_04b37104(uVar2,pcVar3,uVar4,uVar5,
                         "../../arc/browser/web_contents/pdf_controller_impl.cc");
    *param_1 = uVar2;
  }
  else {
    lVar1 = FUN_000805e0(*param_2 + 8,&DAT_0c6ca550);
    if (lVar1 == 0) {
      pcVar3 = "Internal error: glue_layer is null";
      uVar2 = 0x35;
      uVar4 = 0x22;
      uVar5 = 0x1a;
      goto LAB_0436caac;
    }
    uVar6 = *(ulong *)(lVar1 + 0x20);
    if ((uVar6 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(uVar6);
    }
    if (uVar6 == 0) {
      uVar2 = FUN_04b37104(0x35,"Internal error: pdf_controller is null",0x26,0x1f,
                           "../../arc/browser/web_contents/pdf_controller_impl.cc");
      *param_1 = uVar2;
    }
    else {
      uStack_58 = param_3[1];
      local_60 = *param_3;
      local_48 = param_3[3];
      uStack_50 = param_3[2];
      if (*(code **)param_3[3] != (code *)0x0) {
        local_60 = (**(code **)param_3[3])(*param_3);
      }
      FUN_041a7920(uVar6,&local_60);
      if (*(code **)(local_48 + 8) != (code *)0x0) {
        (**(code **)(local_48 + 8))(local_60);
      }
      *param_1 = 1;
    }
    if ((uVar6 & 0xfffffffc00000000) == DAT_0e2f2208) {
      if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
        FUN_0001fee4(uVar6);
        return;
      }
      goto LAB_0436cb28;
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
LAB_0436cb28:
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


