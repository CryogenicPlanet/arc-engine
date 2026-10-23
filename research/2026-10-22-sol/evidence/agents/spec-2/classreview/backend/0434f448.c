// ==== 0x434f448  FUN_0434f448  (324 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_0434f448(long param_1,undefined8 *param_2)

{
  long lVar1;
  long *plVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined1 auStack_50 [32];
  
  lVar1 = FUN_041a4338(param_1 + 0x10);
  if (lVar1 != 0) {
    lVar4 = *(long *)(lVar1 + 0x28);
    *(undefined8 *)(lVar1 + 0x28) = 0;
    FUN_04350aa4(param_1 + 0x10,lVar1);
    plVar2 = (long *)(*(code *)PTR___tlv_bootstrap_0e379858)();
    if (((undefined8 *)*plVar2 == (undefined8 *)0x0) ||
       (puVar5 = *(undefined8 **)*plVar2, puVar5 == (undefined8 *)0x0)) {
      if (lVar4 != 0) {
        FUN_00005810(lVar4,0x10);
        return;
      }
    }
    else {
      FUN_00029e40(auStack_50,"CancelURLLoad","../../arc/browser/url_loader_helper.cc",0xd7);
      puVar3 = (undefined4 *)FUN_00008790(0x58);
      *puVar3 = 1;
      *(undefined1 **)(puVar3 + 2) = &LAB_041d1e60;
      *(code **)(puVar3 + 4) = FUN_04350618;
      *(code **)(puVar3 + 6) = FUN_00b77080;
      *(undefined8 *)(puVar3 + 10) = 1;
      *(undefined8 *)(puVar3 + 8) = 0x30;
      *(long *)(puVar3 + 0xc) = lVar4;
      if (*(char *)((long)param_2 + 0x17) < '\0') {
        FUN_00060a60(puVar3 + 0xe,*param_2,param_2[1]);
      }
      else {
        uVar6 = *param_2;
        *(undefined8 *)(puVar3 + 0x10) = param_2[1];
        *(undefined8 *)(puVar3 + 0xe) = uVar6;
        *(undefined8 *)(puVar3 + 0x12) = param_2[2];
      }
      puVar3[0x14] = 0xfffffffd;
      (**(code **)*puVar5)(puVar5,auStack_50,puVar3,0);
    }
  }
  return;
}


