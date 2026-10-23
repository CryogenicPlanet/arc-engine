// ==== 0x83694e8  FUN_083694e8  (108 bytes)

long FUN_083694e8(long param_1,long param_2)

{
  long lVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  
  lVar1 = FUN_05533b24();
  puVar2 = (undefined8 *)(lVar1 + 0x20);
  *(undefined1 *)puVar2 = 0;
  *(undefined1 *)(lVar1 + 0x38) = 0;
  if (*(char *)(param_2 + 0x38) != '\0') {
    if (*(char *)(param_2 + 0x37) < '\0') {
      thunk_FUN_00060a60(puVar2,*(undefined8 *)(param_2 + 0x20),*(undefined8 *)(param_2 + 0x28));
    }
    else {
      uVar4 = *(undefined8 *)(param_2 + 0x28);
      uVar3 = *(undefined8 *)(param_2 + 0x20);
      *(undefined8 *)(lVar1 + 0x30) = *(undefined8 *)(param_2 + 0x30);
      *(undefined8 *)(lVar1 + 0x28) = uVar4;
      *puVar2 = uVar3;
    }
    *(undefined1 *)(param_1 + 0x38) = 1;
  }
  *(undefined2 *)(param_1 + 0x40) = *(undefined2 *)(param_2 + 0x40);
  return param_1;
}


