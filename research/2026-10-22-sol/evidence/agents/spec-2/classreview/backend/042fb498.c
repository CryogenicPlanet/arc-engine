// ==== 0x42fb498  FUN_042fb498  (488 bytes)

void FUN_042fb498(undefined8 *param_1,long param_2)

{
  long *plVar1;
  long lVar2;
  ulong uVar3;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 local_50;
  
  if (((*(long *)(param_2 + 0xf0) != 0) && (*(char *)(*(long *)(param_2 + 0xf0) + 4) == '\0')) &&
     (*(long *)(param_2 + 0xf8) != 0)) {
    if ((*(long *)(param_2 + 0xf0) == 0) || (*(char *)(*(long *)(param_2 + 0xf0) + 4) != '\0')) {
      plVar1 = (long *)0x0;
    }
    else {
      plVar1 = *(long **)(param_2 + 0xf8);
    }
    lVar2 = (**(code **)(*plVar1 + 0x18))();
    for (plVar1 = *(long **)(lVar2 + 8); plVar1 != (long *)0x0; plVar1 = (long *)*plVar1) {
      while ((undefined **)plVar1[4] < (undefined **)0xdf404e9) {
        if ((undefined **)plVar1[4] == &PTR_s_WindowMetadataController_kDataK_0df404e8) {
          if (plVar1 == (long *)(lVar2 + 8)) goto LAB_042fb550;
          uVar3 = plVar1[5];
          if ((uVar3 & 0xfffffffc00000000) == DAT_0e2f2208) {
            FUN_0000ffa8(uVar3);
          }
          goto LAB_042fb57c;
        }
        plVar1 = (long *)plVar1[1];
        if (plVar1 == (long *)0x0) goto LAB_042fb550;
      }
    }
LAB_042fb550:
    uVar3 = 0;
LAB_042fb57c:
    if ((uVar3 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4(uVar3);
    }
    lVar2 = (long)*(char *)(uVar3 + 0x1f);
    if (lVar2 < 0) {
      lVar2 = *(long *)(uVar3 + 0x10);
    }
    if (lVar2 != 0) {
      if ((*(long *)(param_2 + 0xf0) == 0) || (*(char *)(*(long *)(param_2 + 0xf0) + 4) != '\0')) {
        plVar1 = (long *)0x0;
      }
      else {
        plVar1 = *(long **)(param_2 + 0xf8);
      }
      lVar2 = (**(code **)(*plVar1 + 0x18))();
      for (plVar1 = *(long **)(lVar2 + 8); plVar1 != (long *)0x0; plVar1 = (long *)*plVar1) {
        while ((undefined **)plVar1[4] < (undefined **)0xdf404e9) {
          if ((undefined **)plVar1[4] == &PTR_s_WindowMetadataController_kDataK_0df404e8) {
            if (plVar1 == (long *)(lVar2 + 8)) goto LAB_042fb5f8;
            uVar3 = plVar1[5];
            if ((uVar3 & 0xfffffffc00000000) == DAT_0e2f2208) {
              FUN_0000ffa8(uVar3);
            }
            goto LAB_042fb624;
          }
          plVar1 = (long *)plVar1[1];
          if (plVar1 == (long *)0x0) goto LAB_042fb5f8;
        }
      }
LAB_042fb5f8:
      uVar3 = 0;
LAB_042fb624:
      if ((uVar3 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0001fee4(uVar3);
      }
      if (*(char *)(uVar3 + 0x1f) < '\0') {
        FUN_00060a60(&local_60,*(undefined8 *)(uVar3 + 8),*(undefined8 *)(uVar3 + 0x10));
      }
      else {
        uStack_58 = *(undefined8 *)(uVar3 + 0x10);
        local_60 = *(undefined8 *)(uVar3 + 8);
        local_50 = *(undefined8 *)(uVar3 + 0x18);
      }
      param_1[1] = uStack_58;
      *param_1 = local_60;
      param_1[2] = local_50;
      *(undefined1 *)(param_1 + 3) = 1;
      return;
    }
  }
  *(undefined1 *)param_1 = 0;
  *(undefined1 *)(param_1 + 3) = 0;
  return;
}


