// ==== 0x4461340  FUN_04461340  (364 bytes)

long * FUN_04461340(undefined4 param_1,undefined8 param_2,long param_3,undefined8 param_4,
                   long param_5)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long *plVar4;
  long *plVar5;
  
  plVar1 = _malloc(0xd8);
  if (plVar1 == (long *)0x0) {
    FUN_01790fe0(0xe,0,0x41,"../../third_party/boringssl/src/crypto/mem.cc",0xd7);
    return (long *)0x0;
  }
  plVar5 = plVar1 + 1;
  *plVar1 = 0xd0;
  if (plVar5 == (long *)0x0) {
    return (long *)0x0;
  }
  plVar1[0x18] = 0;
  plVar1[0x17] = 0;
  plVar1[0x1a] = 0;
  plVar1[0x19] = 0;
  plVar1[0x14] = 0;
  plVar1[0x13] = 0;
  plVar1[0x16] = 0;
  plVar1[0x15] = 0;
  plVar1[0x10] = 0;
  plVar1[0xf] = 0;
  plVar1[0x12] = 0;
  plVar1[0x11] = 0;
  plVar1[0xc] = 0;
  plVar1[0xb] = 0;
  plVar1[0xe] = 0;
  plVar1[0xd] = 0;
  plVar1[8] = 0;
  plVar1[7] = 0;
  plVar1[10] = 0;
  plVar1[9] = 0;
  plVar1[4] = 0;
  plVar1[3] = 0;
  plVar1[6] = 0;
  plVar1[5] = 0;
  plVar1[2] = 0;
  *plVar5 = 0;
  plVar1[0x15] = 0;
  *(undefined4 *)(plVar1 + 0x19) = param_1;
  plVar1[0x16] = 0;
  if (param_3 == 0) {
    lVar2 = 0;
  }
  else {
    lVar2 = FUN_01514750(param_2,param_3);
    plVar1[0x15] = lVar2;
    if (lVar2 == 0) goto LAB_04461450;
    plVar1[0x16] = param_3;
  }
  plVar4 = plVar1 + 0x17;
  *plVar4 = 0;
  plVar1[0x18] = 0;
  if (param_5 == 0) {
    return plVar5;
  }
  lVar3 = FUN_01514750(param_4,param_5);
  *plVar4 = lVar3;
  if (lVar3 != 0) {
    plVar1[0x18] = param_5;
    return plVar5;
  }
  if (lVar2 != 0) {
    plVar5 = (long *)(lVar2 + -8);
    if (*plVar5 + 8U != 0) {
      _bzero(plVar5,*plVar5 + 8U);
    }
    _free(plVar5);
    if (*plVar4 != 0) {
      plVar5 = (long *)(*plVar4 + -8);
      if (*plVar5 + 8U != 0) {
        _bzero(plVar5,*plVar5 + 8U);
      }
      _free(plVar5);
    }
  }
LAB_04461450:
  if (*plVar1 + 8U != 0) {
    _bzero(plVar1,*plVar1 + 8U);
  }
  _free(plVar1);
  return (long *)0x0;
}


