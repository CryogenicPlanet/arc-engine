// ==== 0x44124dc  objc::Categories::ArcWebContents_Extensions_::cast  (288 bytes)

/* Function Stack Size: 0x10 bytes */

bool objc::Categories::ArcWebContents_Extensions_::cast(ID param_1,SEL param_2)

{
  undefined8 uVar1;
  code *pcVar2;
  bool bVar3;
  long lVar4;
  long local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"web_contents",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x44125fc);
    (*pcVar2)();
  }
  lVar4 = *(long *)(local_48 + 0x130);
  bVar3 = 0;
  if (lVar4 != 0) {
    uVar1 = FUN_041c7080();
    lVar4 = FUN_04ad6f00(uVar1,lVar4);
    if (lVar4 == 0) {
      bVar3 = 0;
    }
    else {
      FUN_073554d4(lVar4,0);
      bVar3 = 1;
    }
  }
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar4 = local_40[1];
    local_40[1] = lVar4 + -1;
    LORelease();
    if (lVar4 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar4 = local_40[2];
        local_40[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) {
          return bVar3;
        }
      }
      pcVar2 = *(code **)(*local_40 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar2)(local_40);
      }
    }
  }
  return bVar3;
}


