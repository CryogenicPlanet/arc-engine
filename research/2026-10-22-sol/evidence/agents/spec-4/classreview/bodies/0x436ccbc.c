// ==== 0x436ccbc  FUN_0436ccbc  (520 bytes)

void FUN_0436ccbc(undefined8 *param_1,long *param_2,undefined8 *param_3)

{
  long lVar1;
  undefined8 uVar2;
  char *pcVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  ulong uVar6;
  undefined8 *puVar7;
  undefined8 uVar8;
  undefined8 *local_78 [2];
  undefined1 *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  if (*param_2 == 0) {
    pcVar3 = "The object is in detached state";
    uVar2 = 0x25;
    uVar4 = 0x1f;
    uVar5 = 0x3c;
LAB_0436ce3c:
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
      uVar5 = 0x42;
      goto LAB_0436ce3c;
    }
    uVar6 = *(ulong *)(lVar1 + 0x20);
    if ((uVar6 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(uVar6);
    }
    if (uVar6 == 0) {
      uVar2 = FUN_04b37104(0x35,"Internal error: pdf_controller is null",0x26,0x47,
                           "../../arc/browser/web_contents/pdf_controller_impl.cc");
      *param_1 = uVar2;
    }
    else {
      uVar2 = *param_3;
      uVar4 = param_3[1];
      puVar7 = (undefined8 *)param_3[3];
      uVar8 = param_3[3];
      uVar5 = param_3[2];
      if ((code *)*puVar7 != (code *)0x0) {
        uVar2 = (*(code *)*puVar7)(uVar2);
      }
      local_60 = (undefined **)&DAT_0c4b3500;
      if ((*(byte *)(puVar7 + 2) & 1) == 0) {
        local_68 = &LAB_0436d21c;
        local_60 = &PTR_FUN_0df6afd8;
        local_78[0] = (undefined8 *)FUN_00008790(0x20);
        *local_78[0] = uVar2;
        local_78[0][1] = uVar4;
        local_78[0][3] = uVar8;
        local_78[0][2] = uVar5;
        if (puVar7[1] != 0) {
          puVar7 = &DAT_0c4b3500;
        }
      }
      FUN_041a7d50(uVar6,local_78);
      if ((code *)local_60[1] != (code *)0x0) {
        (*(code *)local_60[1])(local_78[0]);
      }
      if ((code *)puVar7[1] != (code *)0x0) {
        (*(code *)puVar7[1])(uVar2);
      }
      *param_1 = 1;
    }
    if ((uVar6 & 0xfffffffc00000000) == DAT_0e2f2208) {
      if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
        FUN_0001fee4(uVar6);
        return;
      }
      goto LAB_0436cec0;
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
    return;
  }
LAB_0436cec0:
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail();
}


