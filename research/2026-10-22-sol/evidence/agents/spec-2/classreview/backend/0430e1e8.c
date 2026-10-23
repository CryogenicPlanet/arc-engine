// ==== 0x430e1e8  FUN_0430e1e8  (368 bytes)

void FUN_0430e1e8(undefined8 *param_1,long param_2)

{
  long lVar1;
  long lVar2;
  bool bVar3;
  long lVar4;
  long lVar5;
  code *pcVar6;
  long *plVar7;
  undefined8 local_30;
  undefined8 uStack_28;
  
  lVar4 = (**(code **)(**(long **)(param_2 + 0x18) + 0xf8))();
  if (lVar4 != 0) {
    (**(code **)(**(long **)(param_2 + 0x18) + 0xf8))();
    lVar4 = FUN_0647a03c();
    if (lVar4 != 0) {
      do {
        lVar5 = lVar4;
        lVar4 = *(long *)(lVar5 + 0x1e0);
      } while (lVar4 != 0);
      lVar4 = FUN_000805e0(lVar5 + 8,&DAT_0e4a2f38);
      if ((lVar4 != 0) && (plVar7 = *(long **)(lVar4 + 0x10), plVar7 != (long *)0x0)) {
        lVar5 = plVar7[1];
        while (lVar5 != -1) {
          lVar1 = lVar5 + 1;
          lVar2 = plVar7[1];
          bVar3 = lVar2 == lVar5;
          lVar5 = lVar2;
          if (bVar3) {
            plVar7[1] = lVar1;
            if (*(long *)(lVar4 + 8) == 0) {
              *(undefined1 *)param_1 = 0;
              *(undefined1 *)(param_1 + 2) = 0;
            }
            else {
              FUN_042c8400(&local_30);
              param_1[1] = uStack_28;
              *param_1 = local_30;
              *(undefined1 *)(param_1 + 2) = 1;
            }
            LOAcquire();
            lVar4 = plVar7[1];
            plVar7[1] = lVar4 + -1;
            LORelease();
            if (lVar4 != 0) {
              return;
            }
            (**(code **)(*plVar7 + 0x10))(plVar7);
            if (plVar7[2] != 0) {
              LOAcquire();
              lVar4 = plVar7[2];
              plVar7[2] = lVar4 + -1;
              LORelease();
              if (lVar4 != 0) {
                return;
              }
            }
            pcVar6 = *(code **)(*plVar7 + 0x20);
            if (((pcVar6 != FUN_00f01bc0) && (pcVar6 != FUN_00f01bc0)) && (pcVar6 != FUN_013efb40))
            {
              (*pcVar6)(plVar7);
              return;
            }
            FUN_00005810(plVar7,0x20);
            return;
          }
        }
      }
    }
  }
  *(undefined1 *)param_1 = 0;
  *(undefined1 *)(param_1 + 2) = 0;
  return;
}


