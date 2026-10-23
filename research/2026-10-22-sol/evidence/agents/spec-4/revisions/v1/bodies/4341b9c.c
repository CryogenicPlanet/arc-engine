// ==== 0x4341b9c  FUN_04341b9c  (176 bytes)

undefined8 * FUN_04341b9c(undefined8 *param_1)

{
  long *plVar1;
  long lVar2;
  long *plVar3;
  
  plVar1 = (long *)*param_1;
  *param_1 = 0;
  if (plVar1 != (long *)0x0) {
    lVar2 = *plVar1;
    *plVar1 = 0;
    if (lVar2 != 0) {
      if (*(long *)(lVar2 + 0xa0) != 0) {
        plVar3 = (long *)(*(long *)(lVar2 + 0xa0) + -8);
        if (*plVar3 + 8U != 0) {
          _bzero(plVar3,*plVar3 + 8U);
        }
        _free(plVar3);
      }
      if (*(long *)(lVar2 + 0xb0) != 0) {
        plVar3 = (long *)(*(long *)(lVar2 + 0xb0) + -8);
        if (*plVar3 + 8U != 0) {
          _bzero(plVar3,*plVar3 + 8U);
        }
        _free(plVar3);
      }
      plVar3 = (long *)(lVar2 + -8);
      if (*plVar3 + 8U != 0) {
        _bzero(plVar3,*plVar3 + 8U);
      }
      _free(plVar3);
    }
    FUN_00005810(plVar1,8);
  }
  return param_1;
}


