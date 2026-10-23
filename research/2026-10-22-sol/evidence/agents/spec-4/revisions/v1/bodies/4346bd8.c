// ==== 0x4346bd8  FUN_04346bd8  (480 bytes)

void FUN_04346bd8(undefined8 *param_1,long param_2,long param_3)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  undefined8 *puVar4;
  ulong uVar5;
  long lVar6;
  undefined8 uVar7;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  ulong uStack_68;
  ulong local_60;
  ulong local_58;
  
  if (((*(long **)(param_2 + 0x68) != (long *)0x0) &&
      (plVar1 = (long *)(**(code **)(**(long **)(param_2 + 0x68) + 0x78))(), plVar1 != (long *)0x0))
     && (lVar2 = *plVar1, 0 < *(int *)(lVar2 + 0x18))) {
    lVar6 = 0;
    do {
      lVar2 = FUN_003abe30(lVar2,lVar6);
      lVar2 = *(long *)(lVar2 + 0x40);
      if (((lVar2 != 0) &&
          (lVar3 = FUN_000805e0(lVar2 + 8,&DAT_0cd0f780), lVar3 != 0 && lVar3 != 0x20)) &&
         (param_3 == *(int *)(lVar3 + 0x50))) {
        if (((*(long *)(param_2 + 0x68) != 0) && (lVar6 = FUN_0031dab4(), lVar6 != 0)) &&
           (*(long *)(lVar6 + 0x278) != 0)) {
          uStack_68 = 0;
          local_70 = 0;
          local_58 = 0;
          local_60 = 0;
          uStack_78 = 0;
          local_80 = 0;
          FUN_043c4054(&local_80,*(long *)(lVar6 + 0x278),lVar2);
          param_1[3] = 0;
          param_1[2] = 0;
          param_1[5] = 0;
          param_1[4] = 0;
          param_1[1] = 0;
          *param_1 = 0;
          *(undefined1 *)param_1 = (undefined1)local_80;
          if (param_1 != &local_80) {
            if ((long)uStack_68 < 0) {
              FUN_00359998(param_1 + 1,uStack_78,local_70);
            }
            else {
              puVar4 = (undefined8 *)((ulong)&local_80 | 8);
              uVar7 = *puVar4;
              param_1[2] = puVar4[1];
              param_1[1] = uVar7;
              param_1[3] = puVar4[2];
            }
          }
          *(undefined1 *)(param_1 + 4) = (undefined1)local_60;
          if (((local_60 & 1) != 0) && (uVar5 = *(ulong *)(param_2 + 0x60), uVar5 != 0)) {
            if ((uVar5 & 0xfffffffc00000000) == DAT_0e2f2208) {
              FUN_0000ffa8(uVar5);
            }
            if ((param_1[5] & 0xfffffffc00000000) == DAT_0e2f2208) {
              FUN_0001fee4();
            }
            param_1[5] = uVar5;
          }
          if ((local_58 & 0xfffffffc00000000) == DAT_0e2f2208) {
            FUN_0001fee4();
          }
          local_58 = 0;
          if (-1 < (long)uStack_68) {
            return;
          }
          FUN_00005810(uStack_78,uStack_68 & 0x7fffffffffffffff);
          return;
        }
        break;
      }
      lVar6 = lVar6 + 1;
      lVar2 = *plVar1;
    } while (lVar6 < *(int *)(lVar2 + 0x18));
  }
  *(undefined1 *)param_1 = 0;
  param_1[5] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[1] = 0;
  *(undefined1 *)(param_1 + 4) = 0;
  return;
}


