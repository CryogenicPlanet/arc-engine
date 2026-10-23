// ==== 0x43776b8  FUN_043776b8  (152 bytes)

void FUN_043776b8(float param_1,long param_2)

{
  long lVar1;
  long lVar2;
  double dVar3;
  undefined1 local_39;
  undefined8 local_38;
  
  if ((*(long *)(param_2 + 0x130) != 0) && (lVar1 = FUN_00882ba0(), lVar1 != 0)) {
    if (param_1 <= 0.0) {
      lVar2 = FUN_01064b4c(*(undefined8 *)(lVar1 + 0x18));
      local_39 = 0;
      FUN_000f71cc(&local_39,0,0xa0);
      dVar3 = *(double *)(lVar2 + 0x70);
    }
    else {
      dVar3 = (double)_log((double)param_1);
      dVar3 = dVar3 / 0.1823215567939546;
    }
    local_38 = 0;
    FUN_09fc1b54(dVar3,lVar1,&local_38);
  }
  return;
}


