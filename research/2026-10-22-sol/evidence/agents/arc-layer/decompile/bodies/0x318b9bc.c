// ==== 0x318b9bc  FUN_0318b9bc  (496 bytes)

void FUN_0318b9bc(long param_1)

{
  int *piVar1;
  int iVar2;
  undefined8 uVar3;
  long lVar4;
  undefined1 auStack_68 [8];
  undefined1 auStack_60 [8];
  int *local_58 [4];
  undefined8 local_38;
  
  if ((*(byte *)(param_1 + 8) & 1) == 0) {
    FUN_0837a430(local_58,"../../content/browser/browser_main_runner_impl.cc",0x9f);
    FUN_08375df4(auStack_60,"initialization_started_",0x9f,local_58);
    FUN_0837612c(auStack_60);
  }
  if (*(char *)(param_1 + 9) != '\0') {
    FUN_0837a430(local_58,"../../content/browser/browser_main_runner_impl.cc",0xa0);
    FUN_08375df4(auStack_68,"!is_shutdown_",0x9f,local_58);
    FUN_0837612c(auStack_68);
  }
  lVar4 = *(long *)(param_1 + 0x18);
  local_58[0] = (int *)0x0;
  FUN_01469964(lVar4 + 0x150,local_58);
  piVar1 = local_58[0];
  if (local_58[0] != (int *)0x0) {
    local_58[0] = (int *)0x0;
    (**(code **)(piVar1 + 2))(piVar1);
    LOAcquire();
    iVar2 = *piVar1;
    *piVar1 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      (**(code **)(piVar1 + 4))(piVar1);
    }
    if (local_58[0] != (int *)0x0) {
      LOAcquire();
      iVar2 = *local_58[0];
      *local_58[0] = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        (**(code **)(local_58[0] + 4))();
      }
    }
  }
  FUN_02f34f84();
  if (*(long *)(lVar4 + 0x128) != 0) {
    FUN_02f350c4();
  }
  local_38 = 0x95;
  if (DAT_0e4a44e5 != '\0') {
    local_58[0] = (int *)0xd197ee4;
    FUN_0000a280(DAT_0e4a44e5,&local_38,local_58,1);
  }
  if (((DAT_0e495350 & 1) == 0) && (iVar2 = thunk_FUN_0c44b610(&DAT_0e495350), iVar2 != 0)) {
    DAT_0e495348 = 0;
    thunk_FUN_0c44b760(&DAT_0e495350);
  }
  DAT_0e495348 = 1;
  FUN_03015550(*(undefined8 *)(param_1 + 0x18));
  lVar4 = *(long *)(param_1 + 0x18);
  *(undefined8 *)(param_1 + 0x18) = 0;
  if (lVar4 != 0) {
    uVar3 = FUN_02f0961c();
    FUN_00005810(uVar3,0x160);
  }
  *(undefined1 *)(param_1 + 9) = 1;
  local_38 = 0x95;
  if (DAT_0e4a44e5 != '\0') {
    local_58[0] = (int *)0x0;
    FUN_0000a280(DAT_0e4a44e5,&local_38,local_58,2);
  }
  return;
}


