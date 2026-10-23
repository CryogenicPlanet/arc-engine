// ==== 0x436d054  FUN_0436d054  (456 bytes)

void FUN_0436d054(undefined8 *param_1,long *param_2,undefined8 param_3,undefined8 *param_4)

{
  long lVar1;
  undefined8 uVar2;
  undefined4 *puVar3;
  char *pcVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  ulong uVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  
  if (*param_2 == 0) {
    pcVar4 = "The object is in detached state";
    uVar2 = 0x25;
    uVar5 = 0x1f;
    uVar6 = 0x65;
  }
  else {
    lVar1 = FUN_000805e0(*param_2 + 8,&DAT_0c6ca550);
    if (lVar1 != 0) {
      uVar7 = *(ulong *)(lVar1 + 0x20);
      if ((uVar7 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0000ffa8(uVar7);
      }
      if (uVar7 == 0) {
        uVar2 = FUN_04b37104(0x35,"Internal error: pdf_controller is null",0x26,0x70,
                             "../../arc/browser/web_contents/pdf_controller_impl.cc");
        *param_1 = uVar2;
      }
      else {
        uVar2 = *param_4;
        uVar5 = param_4[1];
        puVar8 = (undefined8 *)param_4[3];
        uVar9 = param_4[3];
        uVar6 = param_4[2];
        if ((code *)*puVar8 != (code *)0x0) {
          uVar2 = (*(code *)*puVar8)(uVar2);
        }
        puVar3 = (undefined4 *)FUN_00008790(0x48);
        *puVar3 = 1;
        *(undefined1 **)(puVar3 + 2) = &LAB_041a82c0;
        *(code **)(puVar3 + 4) = FUN_041a82e0;
        *(code **)(puVar3 + 6) = FUN_00b77080;
        *(undefined8 *)(puVar3 + 10) = uVar2;
        *(undefined8 *)(puVar3 + 0xc) = uVar5;
        *(undefined8 *)(puVar3 + 0x10) = uVar9;
        *(undefined8 *)(puVar3 + 0xe) = uVar6;
        if (puVar8[1] != 0) {
          puVar8 = &DAT_0c4b3500;
        }
        FUN_06f88120(uVar7 + 0x118);
        FUN_041a8a58(*(undefined8 *)(uVar7 + 0x140),param_3,puVar3);
        if ((code *)puVar8[1] != (code *)0x0) {
          (*(code *)puVar8[1])(uVar2);
        }
        *param_1 = 1;
      }
      if ((uVar7 & 0xfffffffc00000000) != DAT_0e2f2208) {
        return;
      }
      FUN_0001fee4(uVar7);
      return;
    }
    pcVar4 = "Internal error: glue_layer is null";
    uVar2 = 0x35;
    uVar5 = 0x22;
    uVar6 = 0x6b;
  }
  uVar2 = FUN_04b37104(uVar2,pcVar4,uVar5,uVar6,
                       "../../arc/browser/web_contents/pdf_controller_impl.cc");
  *param_1 = uVar2;
  return;
}


