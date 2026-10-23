// ==== 0x42e8148  FUN_042e8148  (264 bytes)

undefined8 FUN_042e8148(long param_1,undefined8 param_2,undefined8 *param_3)

{
  code *pcVar1;
  undefined8 uVar2;
  long lVar3;
  long lVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined8 local_48 [2];
  undefined7 local_38;
  char cStack_31;
  
  if (*(long *)(param_1 + 0x68) != 0) {
    uVar2 = FUN_014baf1c();
    lVar3 = FUN_022a58ec(uVar2,param_2,1);
    if (((lVar3 != 0) && (lVar4 = FUN_01ea9548(), (*(byte *)(lVar4 + 0x110) & 1) == 0)) &&
       ((lVar4 = FUN_01ea9548(lVar3), (*(byte *)(lVar4 + 0x20) & 1) != 0 ||
        (*(long *)(lVar4 + 0x88) != 0)))) {
      uVar6 = *(undefined8 *)(param_1 + 0x68);
      uVar2 = FUN_01145938();
      uVar2 = FUN_0009278c(uVar2,uVar6,1);
      lVar3 = FUN_06b1ab34(uVar2,*(undefined8 *)(lVar3 + 0x228));
      DAT_0e495710 = 1;
      if (*(long *)(lVar3 + 0x98) == 0) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x42e824c);
        (*pcVar1)();
      }
      lVar3 = *(long *)(*(long *)(lVar3 + 0x98) + 0x8d8);
      if (lVar3 == 0) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x42e8258);
        (*pcVar1)();
      }
      lVar4 = (long)*(char *)((long)param_3 + 0x17);
      puVar5 = param_3;
      if (lVar4 < 0) {
        puVar5 = (undefined8 *)*param_3;
        lVar4 = param_3[1];
      }
      FUN_007cf8e0(local_48,puVar5,lVar4);
      FUN_06677638(lVar3,local_48,0);
      if (cStack_31 < '\0') {
        FUN_00005810(local_48[0],CONCAT17(cStack_31,local_38) << 1);
      }
      DAT_0e495710 = 0;
      return 1;
    }
  }
  return 0;
}


