// ==== 0x44123e0  objc::Categories::ArcWebContents_Extensions_::hash  (252 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long objc::Categories::ArcWebContents_Extensions_::hash(ID param_1,SEL param_2)

{
  long lVar1;
  unsigned_long_long uVar2;
  code *pcVar3;
  undefined8 local_50;
  long *local_48;
  byte local_40;
  undefined8 local_38;
  
  _objc_msgSend(param_1,"web_contents",&local_50);
  if ((local_40 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  local_38 = local_50;
  uVar2 = FUN_00018f3c(&local_38,8);
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return uVar2;
        }
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
  return uVar2;
}


