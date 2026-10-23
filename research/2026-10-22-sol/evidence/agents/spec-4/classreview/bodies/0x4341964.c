// ==== 0x4341964  FUN_04341964  (224 bytes)

void FUN_04341964(long param_1,int param_2)

{
  char cVar1;
  undefined4 uVar2;
  
  if (param_2 < 2) {
    if (param_2 == 0) {
      if (*(char *)(param_1 + 0x18) != '\0') {
        cVar1 = *(char *)(param_1 + 0x14);
        *(undefined4 *)(param_1 + 0xc) = 0;
        goto joined_r0x043419dc;
      }
      *(undefined4 *)(param_1 + 0xc) = 0;
LAB_04341a24:
      *(undefined1 *)(param_1 + 0x14) = 0;
LAB_04341a2c:
      *(undefined1 *)(param_1 + 0x18) = 1;
      return;
    }
    if (param_2 != 1) {
      return;
    }
    if (*(char *)(param_1 + 0x18) == '\0') {
      *(undefined4 *)(param_1 + 0xc) = 1;
      *(undefined1 *)(param_1 + 0x14) = 0;
      goto LAB_04341a2c;
    }
    cVar1 = *(char *)(param_1 + 0x14);
    uVar2 = 1;
  }
  else if (param_2 == 2) {
    if (*(char *)(param_1 + 0x18) == '\0') {
      uVar2 = 2;
      goto LAB_04341a20;
    }
    cVar1 = *(char *)(param_1 + 0x14);
    uVar2 = 2;
  }
  else if (param_2 == 3) {
    if (*(char *)(param_1 + 0x18) == '\0') {
      uVar2 = 3;
      goto LAB_04341a20;
    }
    cVar1 = *(char *)(param_1 + 0x14);
    uVar2 = 3;
  }
  else {
    if (param_2 != 4) {
      return;
    }
    if (*(char *)(param_1 + 0x18) == '\0') {
      uVar2 = 4;
LAB_04341a20:
      *(undefined4 *)(param_1 + 0xc) = uVar2;
      goto LAB_04341a24;
    }
    cVar1 = *(char *)(param_1 + 0x14);
    uVar2 = 4;
  }
  *(undefined4 *)(param_1 + 0xc) = uVar2;
joined_r0x043419dc:
  if (cVar1 == '\0') {
    return;
  }
  *(undefined1 *)(param_1 + 0x14) = 0;
  return;
}


