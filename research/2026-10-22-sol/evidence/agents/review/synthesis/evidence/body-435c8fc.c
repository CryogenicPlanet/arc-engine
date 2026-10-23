// ==== 0x435c8fc  FUN_0435c8fc  (296 bytes)

void FUN_0435c8fc(long param_1,long *param_2)

{
  code *pcVar1;
  long *plVar2;
  undefined4 *puVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  long lVar6;
  undefined8 *puVar7;
  long lVar8;
  undefined1 auStack_50 [32];
  
  if ((*(byte *)(*(long *)(param_1 + 0x20) + 0x10) & 1) == 0) {
    plVar2 = (long *)(*(code *)PTR___tlv_bootstrap_0e379858)();
    if (((undefined8 *)*plVar2 == (undefined8 *)0x0) ||
       (puVar7 = *(undefined8 **)*plVar2, puVar7 == (undefined8 *)0x0)) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x435ca24);
      (*pcVar1)();
    }
    FUN_00029e40(auStack_50,"Notify","../../arc/browser/web_contents/devtools_impl.cc",0x6e);
    puVar3 = (undefined4 *)FUN_00008790(0x68);
    *puVar3 = 1;
    *(code **)(puVar3 + 2) = FUN_0435ca2c;
    *(undefined **)(puVar3 + 4) = &DAT_0435cb18;
    *(code **)(puVar3 + 6) = FUN_00b77080;
    uVar4 = *(undefined8 *)(param_1 + 8);
    *(undefined8 *)(puVar3 + 0xc) = *(undefined8 *)(param_1 + 0x10);
    *(undefined8 *)(puVar3 + 10) = uVar4;
    puVar5 = *(undefined8 **)(param_1 + 0x20);
    uVar4 = *(undefined8 *)(param_1 + 0x18);
    *(undefined8 *)(puVar3 + 0x10) = *(undefined8 *)(param_1 + 0x20);
    *(undefined8 *)(puVar3 + 0xe) = uVar4;
    if ((code *)*puVar5 != (code *)0x0) {
      uVar4 = (*(code *)*puVar5)(*(undefined8 *)(param_1 + 8));
      *(undefined8 *)(puVar3 + 10) = uVar4;
    }
    lVar6 = *param_2;
    if (lVar6 == 1) {
      lVar8 = param_2[1];
      *(long *)(puVar3 + 0x16) = param_2[2];
      *(long *)(puVar3 + 0x14) = lVar8;
      *(long *)(puVar3 + 0x18) = param_2[3];
      param_2[2] = 0;
      param_2[3] = 0;
      param_2[1] = 0;
    }
    else {
      *param_2 = 0x37;
    }
    *(long *)(puVar3 + 0x12) = lVar6;
    (**(code **)*puVar7)(puVar7,auStack_50,puVar3,0);
  }
  return;
}


