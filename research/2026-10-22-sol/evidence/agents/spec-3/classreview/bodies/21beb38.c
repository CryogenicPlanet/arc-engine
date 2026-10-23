// ==== 0x21beb38  FUN_021beb38  (164 bytes)

void FUN_021beb38(undefined8 *param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  long lVar2;
  byte bVar3;
  char cVar4;
  
  bVar3 = *(byte *)(param_2 + 3);
  if (bVar3 < 6) {
    if (bVar3 < 3) {
      if (bVar3 != 0) {
        if (bVar3 == 1) {
          *(undefined1 *)param_1 = *(undefined1 *)param_2;
        }
        else {
          *(undefined4 *)param_1 = *(undefined4 *)param_2;
        }
      }
      goto LAB_021beb70;
    }
    if (bVar3 == 4) {
      cVar4 = *(char *)((long)param_2 + 0x17);
      puVar1 = (undefined8 *)*param_2;
      if (-1 < (long)cVar4) {
        puVar1 = param_2;
      }
      lVar2 = param_2[1];
      if (-1 < cVar4) {
        lVar2 = (long)cVar4;
      }
      *param_1 = puVar1;
      param_1[1] = lVar2;
      goto LAB_021beb70;
    }
    if (bVar3 == 3) {
      *param_1 = *param_2;
      goto LAB_021beb70;
    }
  }
  else if ((bVar3 != 6) && (bVar3 != 7)) {
                    /* WARNING: Subroutine does not return */
    FUN_04b335b8("std::visit: variant is valueless");
  }
  *param_1 = param_2;
LAB_021beb70:
  *(byte *)(param_1 + 2) = bVar3;
  return;
}


