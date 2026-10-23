// ==== 0x836ba70  FUN_0836ba70  (292 bytes)

void FUN_0836ba70(undefined8 *param_1,long *param_2,long param_3)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  long lVar5;
  long lVar6;
  undefined1 local_b0;
  undefined7 uStack_af;
  undefined7 local_a0;
  char cStack_99;
  undefined1 auStack_90 [80];
  undefined1 auStack_40 [12];
  undefined4 local_34;
  
  lVar6 = *param_2;
  lVar5 = *(long *)(param_3 + 0x18);
  local_b0 = 0;
  thunk_FUN_000f71cc(&local_b0,0,0x9f);
  lVar5 = FUN_02bf70d0(*(undefined8 *)(lVar5 + 0x18));
  if ((lVar5 == 0) || ((*(byte *)(lVar6 + 0x28) & 1) == 0)) {
    uVar2 = FUN_04b37104(0x39,"Download manager is not available",0x21,0x43,
                         "../../arc/browser/browser_context/downloads_impl.cc");
    *param_1 = uVar2;
  }
  else {
    local_34 = 0x5906c92;
    lVar1 = thunk_FUN_00008790(0x4d8);
    puVar3 = (undefined8 *)param_2[1];
    lVar6 = (long)*(char *)((long)puVar3 + 0x17);
    puVar4 = puVar3;
    if (lVar6 < 0) {
      puVar4 = (undefined8 *)*puVar3;
      lVar6 = puVar3[1];
    }
    FUN_00c28450(&local_b0,puVar4,lVar6);
    FUN_0929f2a0(lVar1,&local_b0,&local_34);
    thunk_FUN_000282bc(auStack_40,0);
    thunk_FUN_0002827c(auStack_90);
    if (cStack_99 < '\0') {
      thunk_FUN_00005810(CONCAT71(uStack_af,local_b0),
                         CONCAT17(cStack_99,local_a0) & 0x7fffffffffffffff);
    }
    *(undefined4 *)(lVar1 + 0x3f0) = 6;
    (**(code **)(*(long *)(lVar5 + 8) + 0x10))((long *)(lVar5 + 8),lVar1);
    *param_1 = 1;
  }
  return;
}


