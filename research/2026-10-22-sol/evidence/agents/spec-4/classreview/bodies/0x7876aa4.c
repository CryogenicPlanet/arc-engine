// ==== 0x7876aa4  FUN_07876aa4  (144 bytes)

undefined8 FUN_07876aa4(long param_1,undefined8 param_2,int param_3,undefined8 param_4)

{
  long lVar1;
  
  FUN_0781da20();
  lVar1 = FUN_000805e0(param_1 + 8,&DAT_0c9aa668);
  *(int *)(lVar1 + 0x10) = param_3;
  if (param_3 == 1) {
    FUN_00359948(lVar1 + 0x18,param_4);
  }
  else if (*(char *)(lVar1 + 0x2f) < '\0') {
    **(undefined1 **)(lVar1 + 0x18) = 0;
    *(undefined8 *)(lVar1 + 0x20) = 0;
  }
  else {
    *(undefined1 *)(lVar1 + 0x18) = 0;
    *(undefined1 *)(lVar1 + 0x2f) = 0;
  }
  FUN_0678c5b8(param_1,param_2);
  return 1;
}


