// ==== 0x1e930f8  FUN_01e930f8  (204 bytes)

long * FUN_01e930f8(long *param_1)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  lVar2 = *param_1;
  if (lVar2 != 0) {
    lVar3 = param_1[1] * 0x260;
    lVar4 = lVar2;
    if (lVar3 != 0) {
      lVar4 = param_1[1] * -0x260;
      lVar3 = lVar3 + lVar2;
      do {
        if (lVar3 + -0x260 == 0) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x1e931a0);
          (*pcVar1)();
        }
        if (*(char *)(lVar3 + -0x11) < '\0') {
          FUN_00005810(*(undefined8 *)(lVar3 + -0x28),*(ulong *)(lVar3 + -0x18) & 0x7fffffffffffffff
                      );
        }
        lVar4 = lVar4 + 0x260;
        lVar3 = lVar3 + -0x260;
      } while (lVar4 != 0);
      lVar4 = *param_1;
    }
    param_1[1] = (lVar2 - lVar4 >> 5) * -0x79435e50d79435e5;
    FUN_00005810(lVar4,param_1[2] * 0x260);
  }
  return param_1;
}


