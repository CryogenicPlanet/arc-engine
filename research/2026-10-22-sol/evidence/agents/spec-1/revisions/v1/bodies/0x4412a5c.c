// ==== 0x4412a5c  objc::Categories::ArcWebContents_Extensions_::muted  (288 bytes)

/* Function Stack Size: 0x10 bytes */

bool objc::Categories::ArcWebContents_Extensions_::muted(ID param_1,SEL param_2)

{
  byte bVar1;
  uint uVar2;
  code *pcVar3;
  long lVar4;
  uint uVar5;
  long local_50;
  long *local_48;
  byte local_40;
  undefined1 local_31;
  
  _objc_msgSend(param_1,"web_contents",&local_50);
  if ((local_40 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4412b7c);
    (*pcVar3)();
  }
  if ((*(long *)(local_50 + 0x130) == 0) ||
     (lVar4 = *(long *)(*(long *)(local_50 + 0x130) + 0xf50), lVar4 == 0)) {
    uVar2 = 0;
  }
  else {
    local_31 = 0;
    FUN_000f71cc(&local_31,0,0xa0);
    bVar1 = *(byte *)(lVar4 + 0x28);
    uVar5 = (uint)bVar1;
    uVar2 = (uint)bVar1;
    if (local_40 == 0) goto LAB_04412adc;
  }
  uVar5 = uVar2;
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar4 = local_48[1];
    local_48[1] = lVar4 + -1;
    LORelease();
    if (lVar4 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar4 = local_48[2];
        local_48[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) goto LAB_04412adc;
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
LAB_04412adc:
  return uVar5 & 1;
}


