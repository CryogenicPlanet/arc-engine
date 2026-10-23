// ==== 0x7d64a84  FUN_07d64a84  (136 bytes)

void FUN_07d64a84(long param_1,undefined8 param_2)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  undefined1 auStack_98 [120];
  
  if (param_1 != 0) {
    lVar3 = FUN_018d0a2c(param_1,1);
    if (lVar3 != 0) {
      LOAcquire();
      iVar2 = *(int *)(lVar3 + 8);
      *(int *)(lVar3 + 8) = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        FUN_02849320(lVar3);
      }
    }
    lVar3 = *(long *)(lVar3 + 0x18);
    if ((lVar3 != 0) && (*(char *)(lVar3 + 0x40) != '\0')) {
      FUN_00586268(auStack_98,param_2);
      FUN_06bef990(lVar3,auStack_98);
      FUN_0002822c(auStack_98);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0,0x7d64b0c);
  (*pcVar1)();
}


