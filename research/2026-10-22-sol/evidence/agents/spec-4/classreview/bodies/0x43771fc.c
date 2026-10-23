// ==== 0x43771fc  FUN_043771fc  (100 bytes)

void FUN_043771fc(long param_1)

{
  long lVar1;
  long lVar2;
  code *pcVar3;
  
  lVar1 = *(long *)(param_1 + 0x130);
  if (lVar1 != 0) {
    lVar2 = *(long *)(param_1 + 0x150);
    *(undefined8 *)(param_1 + 0x148) = 0;
    *(undefined8 **)(param_1 + 0x150) = &DAT_0c4b3500;
    pcVar3 = *(code **)(lVar2 + 8);
    if (pcVar3 != (code *)0x0) {
      (*pcVar3)(*(undefined8 *)(param_1 + 0x138));
      lVar1 = *(long *)(param_1 + 0x130);
    }
    lVar1 = FUN_000805e0(lVar1 + 8,&DAT_0cf208a0);
    if (lVar1 != 0) {
      FUN_0c1fde60(lVar1,0);
      return;
    }
  }
  return;
}


