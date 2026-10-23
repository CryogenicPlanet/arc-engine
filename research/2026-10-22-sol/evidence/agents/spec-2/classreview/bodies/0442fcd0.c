// ==== 0x442fcd0  ArcDownloadItem::remainingTimeInSeconds  (144 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcDownloadItem::remainingTimeInSeconds(ID param_1,SEL param_2)

{
  code *pcVar1;
  ulong local_20;
  long_long local_18;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fd54);
    (*pcVar1)();
  }
  local_20 = 0;
  local_18 = 0;
  if (*(long *)(param_1 + 0x10) == 0) {
    local_20 = FUN_04b37104(0x35,"impl is null",0xc,0x23,"../../arc/browser/public/download_item.cc"
                           );
    if (local_20 != 1) goto LAB_0442fd38;
    FUN_04b37cec(&local_20);
  }
  else {
    FUN_0430ddd0(&local_20);
  }
  if (local_20 == 1) {
    return local_18;
  }
LAB_0442fd38:
  if ((local_20 & 1) == 0) {
    FUN_04b35898();
  }
  return 0xffffffffffffffff;
}


