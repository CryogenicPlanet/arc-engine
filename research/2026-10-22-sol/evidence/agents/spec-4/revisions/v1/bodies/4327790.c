// ==== 0x4327790  FUN_04327790  (216 bytes)

void FUN_04327790(undefined8 *param_1,undefined8 *param_2,undefined8 param_3)

{
  int iVar1;
  long lVar2;
  undefined1 *puVar3;
  undefined8 uVar4;
  int *piVar5;
  char *pcVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  
  lVar2 = FUN_005b4234(param_3,*param_2,param_2[1]);
  if (lVar2 == 0) {
    pcVar6 = "Preference not found";
    uVar4 = 0x15;
    uVar7 = 0x14;
    uVar8 = 0x36;
  }
  else {
    if (*(char *)(lVar2 + 0x18) == '\x01') {
      puVar3 = (undefined1 *)FUN_01860668();
      if (puVar3[0x18] == '\x01') {
        *(undefined1 *)(param_1 + 1) = *puVar3;
        *param_1 = 1;
        return;
      }
      if (puVar3[0x18] != -1) {
                    /* WARNING: Subroutine does not return */
        FUN_04b335b8("std::get: wrong alternative for variant");
      }
                    /* WARNING: Subroutine does not return */
      FUN_04b335b8("std::get: variant is valueless");
    }
    pcVar6 = "Wrong preference type";
    uVar4 = 0xd;
    uVar7 = 0x15;
    uVar8 = 0x39;
  }
  piVar5 = (int *)FUN_04b37104(uVar4,pcVar6,uVar7,uVar8,"../../arc/browser/local_state_impl.cc");
  *param_1 = piVar5;
  if (((ulong)piVar5 & 1) != 0) {
    return;
  }
  iVar1 = *piVar5;
  *piVar5 = iVar1 + 1;
  FUN_04b35898(iVar1);
  return;
}


