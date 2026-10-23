// ==== 0x836de40  FUN_0836de40  (136 bytes)

undefined8 * FUN_0836de40(undefined8 *param_1)

{
  FUN_04b8e340(param_1 + 0xc);
  if (*(char *)((long)param_1 + 0x5f) < '\0') {
    thunk_FUN_00005810(param_1[9],param_1[0xb] & 0x7fffffffffffffff);
  }
  if (*(char *)((long)param_1 + 0x47) < '\0') {
    thunk_FUN_00005810(param_1[6],param_1[8] & 0x7fffffffffffffff);
  }
  if (*(char *)((long)param_1 + 0x2f) < '\0') {
    thunk_FUN_00005810(param_1[3],param_1[5] & 0x7fffffffffffffff);
  }
  if (*(char *)((long)param_1 + 0x17) < '\0') {
    thunk_FUN_00005810(*param_1,param_1[2] & 0x7fffffffffffffff);
  }
  return param_1;
}


