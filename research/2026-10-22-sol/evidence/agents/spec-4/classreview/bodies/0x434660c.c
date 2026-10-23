// ==== 0x434660c  FUN_0434660c  (188 bytes)

undefined8 * FUN_0434660c(undefined8 *param_1)

{
  long lVar1;
  long *plVar2;
  code *pcVar3;
  
  plVar2 = (long *)param_1[9];
  if (plVar2 != (long *)0x0) {
    if (plVar2[2] != 0) {
      LOAcquire();
      lVar1 = plVar2[2];
      plVar2[2] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) goto LAB_0434664c;
    }
    pcVar3 = *(code **)(*plVar2 + 0x20);
    if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
      FUN_00005810(plVar2,0x20);
    }
    else {
      (*pcVar3)();
    }
  }
LAB_0434664c:
  if (*(char *)((long)param_1 + 0x3f) < '\0') {
    FUN_00005810(param_1[5],param_1[7] << 1);
  }
  if (*(char *)((long)param_1 + 0x17) < '\0') {
    FUN_00005810(*param_1,param_1[2] << 1);
  }
  return param_1;
}


