// ==== 0x4346528  FUN_04346528  (228 bytes)

void FUN_04346528(undefined1 (*param_1) [16],undefined8 param_2,undefined8 *param_3)

{
  long *plVar1;
  long lVar2;
  long *plVar3;
  code *pcVar4;
  undefined1 auVar5 [16];
  undefined8 local_40;
  undefined1 uStack_38;
  undefined7 uStack_37;
  undefined1 uStack_30;
  undefined8 uStack_2f;
  
  local_40 = *param_3;
  uStack_38 = (undefined1)param_3[1];
  uStack_2f = *(undefined8 *)((long)param_3 + 0x11);
  uStack_37 = (undefined7)*(undefined8 *)((long)param_3 + 9);
  uStack_30 = (undefined1)((ulong)*(undefined8 *)((long)param_3 + 9) >> 0x38);
  auVar5 = FUN_04370ffc(param_2,&local_40);
  plVar3 = auVar5._8_8_;
  *param_1 = auVar5;
  if (plVar3 != (long *)0x0) {
    plVar1 = plVar3 + 1;
    *plVar1 = *plVar1 + 1;
    LOAcquire();
    lVar2 = *plVar1;
    *plVar1 = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar2 = plVar3[2];
        plVar3[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return;
        }
      }
      pcVar4 = *(code **)(*plVar3 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar4)(plVar3);
      }
    }
  }
  return;
}


