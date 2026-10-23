// ==== 0x430ddd0  FUN_0430ddd0  (288 bytes)

void FUN_0430ddd0(long *param_1,long param_2)

{
  ulong uVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  char *pcVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  long lVar8;
  
  lVar3 = (**(code **)(**(long **)(param_2 + 0x18) + 0xf8))();
  if (lVar3 == 0) {
    pcVar5 = "download item is null";
    uVar4 = 0x25;
    uVar6 = 0x15;
    uVar7 = 0x2b;
  }
  else {
    lVar3 = (**(code **)(**(long **)(param_2 + 0x18) + 0xf8))();
    if (((0 < *(long *)(lVar3 + 0x340)) && ((*(byte *)(lVar3 + 0x3a4) & 1) == 0)) &&
       (lVar8 = *(long *)(lVar3 + 0x430), lVar8 != 0)) {
      lVar2 = 0;
      if (lVar8 != 0) {
        lVar2 = (*(long *)(lVar3 + 0x340) - *(long *)(lVar3 + 0x3f8)) / lVar8;
      }
      uVar1 = lVar2 >> 0x3f ^ 0x7fffffffffffffff;
      if (SUB168(SEXT816(lVar2) * SEXT816(1000000),8) == lVar2 * 1000000 >> 0x3f) {
        uVar1 = lVar2 * 1000000;
      }
      if (1 < uVar1 + 0x8000000000000001) {
        uVar1 = (long)uVar1 / 1000000;
      }
      *param_1 = 1;
      param_1[1] = uVar1;
      return;
    }
    pcVar5 = "unable to get remaining time";
    uVar4 = 0x35;
    uVar6 = 0x1c;
    uVar7 = 0x32;
  }
  lVar3 = FUN_04b37104(uVar4,pcVar5,uVar6,uVar7,"../../arc/browser/download_item_impl.cc");
  *param_1 = lVar3;
  if (lVar3 != 1) {
    return;
  }
  FUN_04b37cec(param_1);
  return;
}


