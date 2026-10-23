// ==== 0x443dc40  ArcTabGroupProperties::setColor:  (72 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcTabGroupProperties::setColor_(ID param_1,SEL param_2,ID param_3)

{
  undefined4 uVar1;
  ID IVar2;
  
  if (param_3 == 0) {
    uVar1 = 0;
  }
  else {
    IVar2 = _objc_msgSend(param_3,"integerValue");
    uVar1 = (undefined4)IVar2;
  }
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  *(bool *)(param_1 + 0x2c) = param_3 != 0;
  return;
}


