// ==== 0x2527d18  FUN_02527d18  (96 bytes)

void FUN_02527d18(long *param_1)

{
  long lVar1;
  code *UNRECOVERED_JUMPTABLE;
  
  if (param_1[2] != 0) {
    LOAcquire();
    lVar1 = param_1[2];
    param_1[2] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return;
    }
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*param_1 + 0x20);
  if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
     (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x02527d74. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)();
    return;
  }
  FUN_00005810(param_1,0x20);
  return;
}


