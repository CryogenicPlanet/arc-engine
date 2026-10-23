// ==== 0x42affa0  FUN_042affa0  (336 bytes)

long FUN_042affa0(long *param_1)

{
  ulong uVar1;
  bool bVar2;
  code *pcVar3;
  long *plVar4;
  long unaff_x20;
  long lVar5;
  undefined8 local_48;
  ulong local_40;
  ulong local_38;
  
  if ((((*param_1 == 0) || (*(char *)(*param_1 + 4) != '\0')) || (param_1[1] == 0)) ||
     ((char)param_1[3] == '\0')) {
    lVar5 = 0;
  }
  else {
    if (*(char *)((long)param_1 + 0x24) != '\0') {
      local_48 = 0;
      local_40 = 0;
      local_38 = 0;
      lVar5 = (long)*(char *)((long)param_1 + 0x47);
      if (lVar5 < 0) {
        plVar4 = (long *)param_1[6];
        lVar5 = param_1[7];
      }
      else {
        plVar4 = param_1 + 6;
      }
      bVar2 = true;
      FUN_01857fc8(&local_48,plVar4,lVar5,1);
      uVar1 = local_40;
      if ((local_38 & 0x8000000000000000) == 0) {
        uVar1 = local_38 >> 0x38;
      }
      if (uVar1 != 0) {
        if ((*param_1 == 0) || (*(char *)(*param_1 + 4) != '\0')) {
                    /* WARNING: Does not return */
          pcVar3 = (code *)SoftwareBreakpoint(0,0x42b00ec);
          (*pcVar3)();
        }
        if ((*(byte *)((long)param_1 + 0x24) & 1) == 0) goto LAB_042b00f0;
        unaff_x20 = FUN_02f57470(param_1[1],&local_48,(int)param_1[4]);
        if (unaff_x20 != 0) {
          if ((*(byte *)(param_1 + 3) & 1) == 0) goto LAB_042b00f0;
          if (*(long *)(unaff_x20 + 0x60) == param_1[2]) {
            bVar2 = false;
          }
        }
      }
      if ((long)local_38 < 0) {
        FUN_00005810(local_48,local_38 & 0x7fffffffffffffff);
      }
      if (!bVar2) {
        return unaff_x20;
      }
    }
    lVar5 = 0;
    if (*param_1 != 0) {
      if (*(char *)(*param_1 + 4) == '\0') {
        lVar5 = param_1[1];
      }
      else {
        lVar5 = 0;
      }
    }
    if ((*(byte *)(param_1 + 3) & 1) == 0) {
LAB_042b00f0:
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(1,0x42b00f4);
      (*pcVar3)();
    }
    lVar5 = FUN_09f56d44(*(undefined8 *)(lVar5 + 0x30),param_1[2]);
  }
  return lVar5;
}


