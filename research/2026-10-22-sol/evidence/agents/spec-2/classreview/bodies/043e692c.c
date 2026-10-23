// ==== 0x43e692c  ArcBrowserApplication::setNotificationCenter:  (124 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setNotificationCenter_(ID param_1,SEL param_2,ID param_3)

{
  int iVar1;
  undefined8 uVar2;
  
  if (param_3 == 0) {
    uVar2 = 0;
  }
  else {
    uVar2 = FUN_0440a6e8(param_3);
  }
  if ((DAT_0e4a2f10 & 1) == 0) {
    iVar1 = thunk_FUN_0c44b610(&DAT_0e4a2f10);
    if (iVar1 != 0) {
      DAT_0e4a2f08 = 0;
      thunk_FUN_0c44b760(&DAT_0e4a2f10,uVar2);
    }
  }
  FUN_0432cd94(&DAT_0e4a2f08,uVar2);
  return;
}


