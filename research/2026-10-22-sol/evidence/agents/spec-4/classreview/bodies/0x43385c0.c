// ==== 0x43385c0  FUN_043385c0  (224 bytes)

void FUN_043385c0(undefined1 *param_1,long param_2,int param_3)

{
  code *pcVar1;
  int iVar2;
  undefined8 uVar3;
  long lVar4;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  long local_50;
  undefined8 uStack_48;
  int *local_40;
  undefined8 local_38;
  undefined8 uStack_30;
  
  if (param_3 == 1) {
    uVar3 = FUN_022f4af4(*(undefined8 *)(param_2 + 8));
    FUN_044c0220(param_1,uVar3,0,0);
    return;
  }
  if (param_3 == 0) {
    lVar4 = FUN_022f4af4(*(undefined8 *)(param_2 + 8));
    if (*(long *)(lVar4 + 8) == 0) {
      *param_1 = 0;
      param_1[0x18] = 0;
    }
    else {
      uStack_48 = *(undefined8 *)(lVar4 + 0x10);
      local_50 = *(long *)(lVar4 + 8);
      local_40 = *(int **)(lVar4 + 0x18);
      if (local_40 != (int *)0x0) {
        *local_40 = *local_40 + 1;
      }
      uStack_30 = *(undefined8 *)(lVar4 + 0x28);
      local_38 = *(undefined8 *)(lVar4 + 0x20);
      local_68 = 0;
      uStack_60 = 0;
      local_58 = 0;
      FUN_032f5a00(param_1,&local_50,0,&local_68,1);
      if (local_40 != (int *)0x0) {
        LOAcquire();
        iVar2 = *local_40;
        *local_40 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          FUN_00005810(local_40,0x90);
        }
      }
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x4338694);
  (*pcVar1)();
}


