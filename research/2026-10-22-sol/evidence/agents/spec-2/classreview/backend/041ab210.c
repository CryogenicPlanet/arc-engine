// ==== 0x41ab210  FUN_041ab210  (112 bytes)

undefined8 * FUN_041ab210(undefined8 *param_1)

{
  if ((*(char *)(param_1 + 9) != '\0') && (*(char *)((long)param_1 + 0x47) < '\0')) {
    FUN_00005810(param_1[6],param_1[8] & 0x7fffffffffffffff);
  }
  if (*(char *)((long)param_1 + 0x2f) < '\0') {
    FUN_00005810(param_1[3],param_1[5] & 0x7fffffffffffffff);
  }
  if (*(char *)((long)param_1 + 0x17) < '\0') {
    FUN_00005810(*param_1,param_1[2] & 0x7fffffffffffffff);
  }
  return param_1;
}


