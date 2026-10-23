// ==== 0x8371aa8  FUN_08371aa8  (304 bytes)

void FUN_08371aa8(undefined8 param_1,undefined8 *param_2,undefined8 *param_3)

{
  long lVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  long lVar4;
  undefined8 local_78 [2];
  undefined7 local_68;
  char cStack_61;
  byte local_60;
  undefined8 local_58 [2];
  undefined7 local_48;
  char cStack_41;
  byte local_40;
  undefined1 uStack_31;
  
  FUN_0681222c();
  lVar4 = (long)*(char *)((long)param_2 + 0x17);
  puVar2 = param_2;
  if (lVar4 < 0) {
    puVar2 = (undefined8 *)*param_2;
    lVar4 = param_2[1];
  }
  lVar1 = (long)*(char *)((long)param_3 + 0x17);
  puVar3 = param_3;
  if (lVar1 < 0) {
    puVar3 = (undefined8 *)*param_3;
    lVar1 = param_3[1];
  }
  FUN_00c87c40(local_78,puVar3,lVar1);
  FUN_00c87c40(local_58,puVar2,lVar4);
  FUN_067080b4(param_1,"cr.webUIListenerCallback",0x18,local_58,local_78);
  if ((local_40 != 0xff) && ((local_40 & 0xfd) != 0)) {
    if (local_40 == 4) {
      if (cStack_41 < '\0') {
        thunk_FUN_00005810(local_58[0],CONCAT17(cStack_41,local_48) & 0x7fffffffffffffff);
      }
    }
    else {
      (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_40])(&uStack_31,local_58);
    }
  }
  if ((local_60 != 0xff) && ((local_60 & 0xfd) != 0)) {
    if (local_60 == 4) {
      if (cStack_61 < '\0') {
        thunk_FUN_00005810(local_78[0],CONCAT17(cStack_61,local_68) & 0x7fffffffffffffff);
      }
    }
    else {
      (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_60])(local_58,local_78);
    }
  }
  return;
}


