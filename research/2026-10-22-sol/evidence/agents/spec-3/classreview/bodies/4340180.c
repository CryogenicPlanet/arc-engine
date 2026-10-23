// ==== 0x4340180  FUN_04340180  (328 bytes)

long FUN_04340180(undefined8 param_1,long param_2)

{
  undefined8 ***pppuVar1;
  undefined1 **ppuVar2;
  long lVar3;
  undefined1 *puVar4;
  ulong uVar5;
  ulong uVar6;
  long lVar7;
  undefined1 *local_70;
  long lStack_68;
  undefined8 local_60;
  undefined8 **local_50;
  long lStack_48;
  undefined8 local_40;
  
  if (*(char *)(param_2 + 0x3cf) < '\0') {
    FUN_00054420(&local_50,*(undefined8 *)(param_2 + 0x3b8),*(undefined8 *)(param_2 + 0x3c0));
  }
  else {
    lStack_48 = *(long *)(param_2 + 0x3c0);
    local_50 = *(undefined8 ***)(param_2 + 0x3b8);
    local_40 = *(long *)(param_2 + 0x3c8);
  }
  pppuVar1 = (undefined8 ***)local_50;
  if (-1 < (long)local_40._7_1_) {
    pppuVar1 = &local_50;
  }
  lVar7 = lStack_48;
  if (-1 < local_40) {
    lVar7 = (long)local_40._7_1_;
  }
  uVar5 = FUN_00018f3c(pppuVar1,lVar7 << 1);
  if (*(char *)(param_2 + 999) < '\0') {
    FUN_00054420(&local_70,*(undefined8 *)(param_2 + 0x3d0),*(undefined8 *)(param_2 + 0x3d8));
  }
  else {
    lStack_68 = *(long *)(param_2 + 0x3d8);
    local_70 = *(undefined1 **)(param_2 + 0x3d0);
    local_60 = *(long *)(param_2 + 0x3e0);
  }
  puVar4 = local_70;
  lVar7 = (long)local_60._7_1_;
  ppuVar2 = (undefined1 **)local_70;
  if (-1 < lVar7) {
    ppuVar2 = &local_70;
  }
  lVar3 = lStack_68;
  if (-1 < local_60) {
    lVar3 = lVar7;
  }
  uVar6 = FUN_00018f3c(ppuVar2,lVar3 << 1);
  if (lVar7 < 0) {
    FUN_00005810(puVar4,local_60 << 1);
  }
  if (local_40 < 0) {
    FUN_00005810(local_50,local_40 << 1);
  }
  return uVar5 * 0x32348c9d + (uVar5 >> 0x20) * -0x32348c9cdb19d5fd +
         (uVar6 & 0xffffffff) * 0x37da1fb9 + (uVar6 >> 0x20) * 0xc54622fd;
}


