// ==== 0x43341e4  FUN_043341e4  (192 bytes)

void FUN_043341e4(long *param_1,undefined8 *param_2)

{
  long *plVar1;
  undefined8 uVar2;
  long lVar3;
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  
  lVar3 = *param_1;
  if (*(char *)((long)param_2 + 0x17) < '\0') {
    FUN_00060a60(&local_40,*param_2,param_2[1]);
  }
  else {
    uStack_38 = param_2[1];
    local_40 = *param_2;
    local_30 = param_2[2];
  }
  if (((*(long *)(lVar3 + 0xf0) != 0) && (*(char *)(*(long *)(lVar3 + 0xf0) + 4) == '\0')) &&
     (*(long *)(lVar3 + 0xf8) != 0)) {
    if ((*(long *)(lVar3 + 0xf0) == 0) || (*(char *)(*(long *)(lVar3 + 0xf0) + 4) != '\0')) {
      plVar1 = (long *)0x0;
    }
    else {
      plVar1 = *(long **)(lVar3 + 0xf8);
    }
    uVar2 = (**(code **)(*plVar1 + 0x18))();
    uVar2 = FUN_004969fc(uVar2,&PTR_s_WindowMetadataController_kDataK_0df404e8);
    FUN_079f3f00(uVar2,&local_40);
  }
  if ((long)local_30 < 0) {
    FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
  }
  return;
}


