// ==== 0x443dcc4  ArcTabGroupProperties::setCollapsed:  (72 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcTabGroupProperties::setCollapsed_(ID param_1,SEL param_2,ID param_3)

{
  ushort uVar1;
  ID IVar2;
  
  if (param_3 == 0) {
    uVar1 = 0;
  }
  else {
    IVar2 = _objc_msgSend(param_3,"boolValue");
    uVar1 = (ushort)IVar2;
  }
  *(ushort *)(param_1 + 0x30) = uVar1 | (ushort)(param_3 != 0) << 8;
  return;
}


