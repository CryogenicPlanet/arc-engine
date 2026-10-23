// ==== 0x4337780  FUN_04337780  (344 bytes)

undefined8 * FUN_04337780(undefined8 *param_1)

{
  long lVar1;
  code *pcVar2;
  undefined8 *puVar3;
  undefined8 uVar4;
  undefined1 auStack_248 [432];
  undefined1 local_98;
  undefined7 uStack_97;
  char local_90;
  undefined4 local_88;
  undefined4 uStack_84;
  undefined4 local_80;
  long local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  int *piStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined4 local_48;
  
  *param_1 = &PTR_FUN_0da7eeb0;
  FUN_00434000(auStack_248);
  local_98 = 0;
  local_90 = '\0';
  local_88 = 1;
  uStack_84 = 0;
  local_80 = 0;
  uStack_70 = 0;
  local_78 = 0;
  piStack_60 = (int *)0x0;
  local_68 = 0;
  uStack_50 = 0;
  local_58 = 0;
  local_48 = 0;
  puVar3 = (undefined8 *)FUN_00008790(0x220);
  puVar3[1] = 0;
  puVar3[2] = 0;
  *puVar3 = &PTR_viewWillStartLiveResize_0da7eef0;
  if (puVar3 + 3 != (undefined8 *)0x0) {
    uVar4 = FUN_00a35220(puVar3 + 3,auStack_248);
    *(undefined1 *)(puVar3 + 0x39) = 0;
    *(undefined1 *)(puVar3 + 0x3a) = 0;
    if (local_90 != '\0') {
      lVar1 = CONCAT71(uStack_97,local_98);
      if (lVar1 != 0) {
        *(int *)(lVar1 + 8) = *(int *)(lVar1 + 8) + 1;
      }
      puVar3[0x39] = lVar1;
      *(undefined1 *)(puVar3 + 0x3a) = 1;
    }
    *(undefined4 *)(puVar3 + 0x3b) = local_88;
    *(ulong *)((long)puVar3 + 0x1dc) = CONCAT44(local_80,uStack_84);
    if (local_78 != 0) {
      *(int *)(local_78 + 8) = *(int *)(local_78 + 8) + 1;
    }
    puVar3[0x3d] = local_78;
    puVar3[0x3f] = local_68;
    puVar3[0x3e] = uStack_70;
    if (piStack_60 != (int *)0x0) {
      *piStack_60 = *piStack_60 + 1;
    }
    puVar3[0x40] = piStack_60;
    puVar3[0x42] = uStack_50;
    puVar3[0x41] = local_58;
    *(undefined4 *)(puVar3 + 0x43) = local_48;
    param_1[1] = uVar4;
    param_1[2] = puVar3;
    FUN_01c8dcf8(auStack_248);
    return param_1;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x43378d8);
  (*pcVar2)();
}


