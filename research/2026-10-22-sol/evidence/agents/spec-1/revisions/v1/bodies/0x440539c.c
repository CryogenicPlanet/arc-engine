// ==== 0x440539c  ArcImageRep::init  (256 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcImageRep::init(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_58 [16];
  long *local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcImageRep;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    FUN_04337780(auStack_58);
    FUN_043381c0(IVar2 + 8,auStack_58);
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
            return IVar2;
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
  }
  return IVar2;
}


