// ==== 0x4341ab0  FUN_04341ab0  (236 bytes)

undefined8 *
FUN_04341ab0(undefined8 *param_1,int param_2,undefined8 param_3,undefined8 param_4,
            undefined8 param_5,undefined8 param_6)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  
  plVar1 = (long *)FUN_00008790(8);
  *plVar1 = 0;
  *param_1 = plVar1;
  lVar2 = FUN_04461340(param_2 != 0,param_3,param_4,param_5,param_6);
  lVar3 = *plVar1;
  *plVar1 = lVar2;
  if (lVar3 != 0) {
    if (*(long *)(lVar3 + 0xa0) != 0) {
      plVar1 = (long *)(*(long *)(lVar3 + 0xa0) + -8);
      if (*plVar1 + 8U != 0) {
        _bzero(plVar1,*plVar1 + 8U);
      }
      _free(plVar1);
    }
    if (*(long *)(lVar3 + 0xb0) != 0) {
      plVar1 = (long *)(*(long *)(lVar3 + 0xb0) + -8);
      if (*plVar1 + 8U != 0) {
        _bzero(plVar1,*plVar1 + 8U);
      }
      _free(plVar1);
    }
    plVar1 = (long *)(lVar3 + -8);
    if (*plVar1 + 8U != 0) {
      _bzero(plVar1,*plVar1 + 8U);
    }
    _free(plVar1);
  }
  return param_1;
}


