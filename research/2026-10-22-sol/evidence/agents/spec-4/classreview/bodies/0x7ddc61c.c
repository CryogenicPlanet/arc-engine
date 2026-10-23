// ==== 0x7ddc61c  FUN_07ddc61c  (248 bytes)

void FUN_07ddc61c(long param_1)

{
  code *pcVar1;
  long lVar2;
  ulong uVar3;
  long lVar4;
  undefined8 uVar5;
  
  lVar2 = FUN_000805e0(param_1 + 8,&DAT_0c9c4684);
  if ((lVar2 == 0) || (*(long **)(lVar2 + 0x18) == (long *)0x0)) {
    lVar2 = FUN_000805e0(param_1 + 8,&DAT_0c9c4684);
    if (lVar2 != 0) {
      return;
    }
    uVar5 = FUN_00008790(0x40);
    FUN_07ddc71c(uVar5,param_1);
    FUN_011db6c0(param_1 + 8,&DAT_0c9c4684,uVar5);
    return;
  }
  (**(code **)(**(long **)(lVar2 + 0x18) + 0x68))();
  uVar3 = FUN_033bd41c();
  if ((uVar3 & 1) != 0) {
    return;
  }
  lVar4 = FUN_000805e0(param_1 + 8,&DAT_0cd0fcf4);
  lVar2 = 0;
  if (lVar4 != 0) {
    lVar2 = lVar4 + -0x28;
  }
  uVar3 = FUN_045aafe0(lVar2);
  if ((uVar3 & 1) != 0) {
    FUN_0a1135f8(lVar2);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0,0x7ddc6b4);
  (*pcVar1)();
}


