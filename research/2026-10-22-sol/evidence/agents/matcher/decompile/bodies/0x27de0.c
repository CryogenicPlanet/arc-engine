// ==== 0x27de0  FUN_00027de0  (184 bytes)

int FUN_00027de0(undefined8 *param_1,long param_2)

{
  int iVar1;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  long local_68;
  undefined8 local_60;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  long local_38;
  undefined8 local_30;
  
  local_30 = 0;
  uStack_48 = 0;
  local_50 = 0;
  local_38 = 0;
  uStack_40 = 0;
  FUN_00e266d4(&local_50,*param_1,0);
  local_60 = 0;
  uStack_78 = 0;
  local_80 = 0;
  local_68 = 0;
  uStack_70 = 0;
  FUN_00e266d4(&local_80,*param_1,1);
  if (local_38 != local_68) {
    iVar1 = 0;
    do {
      if (*(long *)(local_38 + 0x40) == param_2) goto LAB_00027e50;
      iVar1 = iVar1 + 1;
      FUN_00e268a4(&local_50);
    } while (local_38 != local_68);
  }
  iVar1 = -1;
LAB_00027e50:
  FUN_006e34e8(&local_80);
  FUN_006e34e8(&local_50);
  return iVar1;
}


