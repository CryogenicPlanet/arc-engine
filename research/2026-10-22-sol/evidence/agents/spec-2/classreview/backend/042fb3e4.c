// ==== 0x42fb3e4  FUN_042fb3e4  (172 bytes)

ulong FUN_042fb3e4(long param_1)

{
  code *pcVar1;
  ulong uVar2;
  long lVar3;
  
  if (((*(long *)(param_1 + 0xf0) != 0) && (*(char *)(*(long *)(param_1 + 0xf0) + 4) == '\0')) &&
     (*(long *)(param_1 + 0xf8) != 0)) {
    uVar2 = FUN_01428b9c(*(undefined8 *)(*(long *)(param_1 + 0x1e0) + 0x88));
    if ((int)uVar2 == 0) {
      return uVar2;
    }
    if ((*(long *)(param_1 + 0xf0) == 0) || (*(char *)(*(long *)(param_1 + 0xf0) + 4) != '\0')) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x42fb490);
      (*pcVar1)();
    }
    lVar3 = (**(code **)(**(long **)(param_1 + 0xf8) + 0x78))();
    if (((*(long *)(lVar3 + 0x90) != 0) &&
        (lVar3 = *(long *)(*(long *)(lVar3 + 0x90) + 0x40), lVar3 != 0)) &&
       (lVar3 = FUN_000805e0(lVar3 + 8,&DAT_0cd0fcf4), lVar3 != 0 && lVar3 != 0x28)) {
      return (ulong)(*(long *)(lVar3 + 0x28) == *(long *)(lVar3 + 0x30));
    }
  }
  return 0;
}


