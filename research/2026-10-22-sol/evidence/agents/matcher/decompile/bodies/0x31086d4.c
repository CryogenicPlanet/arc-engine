// ==== 0x31086d4  AppController::tryToTerminateApplication  (404 bytes)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Function Stack Size: 0x10 bytes */

void AppController::tryToTerminateApplication(ID param_1,SEL param_2)

{
  byte bVar1;
  int iVar2;
  bool bVar3;
  long_long lVar4;
  void *pvVar5;
  long lVar6;
  ulong uVar7;
  undefined1 local_31;
  
  lVar4 = confirmQuitIfNeeded(param_1,"confirmQuitIfNeeded");
  if ((lVar4 != 2) && ((DAT_0e49cf50 & 1) == 0)) {
    bVar1 = *(byte *)(param_1 + 0xe8);
    *(undefined1 *)(param_1 + 0xe8) = 0;
    pvVar5 = lastProfileIfLoaded(param_1,"lastProfileIfLoaded");
    if (pvVar5 != (void *)0x0) {
      if (((DAT_0e49eba0 & 1) == 0) && (iVar2 = thunk_FUN_0c44b610(&DAT_0e49eba0), iVar2 != 0)) {
        FUN_03108b30(&DAT_0e49eb50);
        thunk_FUN_0c44b760(&DAT_0e49eba0);
      }
      lVar6 = FUN_0009278c(&DAT_0e49eb50,pvVar5,0);
      if (lVar6 != 0) {
        FUN_07cc0310();
      }
    }
    bVar3 = shouldQuitWithInProgressDownloads(param_1,"shouldQuitWithInProgressDownloads");
    if (bVar3 != 0) {
      if ((((bVar1 & 1) == 0) && (*(long *)(param_1 + 0xd0) != 0)) &&
         (uVar7 = FUN_03108c50(), (uVar7 & 1) == 0)) {
        FUN_03afc45c(1);
        FUN_03108868();
        FUN_07da38d0(0);
        return;
      }
      local_31 = 1;
      if (((DAT_0e49cf98 & 1) == 0) && (iVar2 = thunk_FUN_0c44b610(&DAT_0e49cf98), iVar2 != 0)) {
        uRam000000000e49cf80 = 0;
        _DAT_0e49cf78 = 0;
        DAT_0e49cf90 = 0;
        DAT_0e49cf88 = 0;
        DAT_0e49cf60 = &DAT_0e49cf60;
        DAT_0e49cf68 = &DAT_0e49cf60;
        DAT_0e49cf70 = 0;
        FUN_013d0d14();
        thunk_FUN_0c44b760(&DAT_0e49cf98);
      }
      FUN_02651780(&DAT_0e49cf60,&local_31);
      DAT_0e49cf50 = 1;
      FUN_03108868();
      return;
    }
  }
  return;
}


