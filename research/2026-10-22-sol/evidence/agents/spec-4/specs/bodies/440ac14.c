// ==== 0x440ac14  ArcSessionID::isEqual:  (196 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool ArcSessionID::isEqual_(ID param_1,SEL param_2,ID param_3)

{
  uint uVar1;
  code *pcVar2;
  ID IVar3;
  bool bVar4;
  
  if (param_1 == param_3) {
    bVar4 = 1;
  }
  else {
    if (param_3 != 0) {
      IVar3 = _objc_msgSend((ID)&objc::class_t::ArcSessionID,"class");
      IVar3 = _objc_msgSend(param_3,"isKindOfClass:",IVar3);
      if ((int)IVar3 != 0) {
        IVar3 = _objc_msgSend(param_3,"sessionId");
        uVar1 = (uint)(IVar3 >> 0x20);
        if ((uVar1 & 0xff & (uint)*(byte *)(param_1 + 0xc)) == 0) {
          return (uint)((uint)*(byte *)(param_1 + 0xc) == (uVar1 & 0xff));
        }
        if ((IVar3 >> 0x20 & 1) != 0) {
          return (uint)(*(int *)(param_1 + 8) == (int)IVar3);
        }
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x440acd8);
        (*pcVar2)();
      }
    }
    bVar4 = 0;
  }
  return bVar4;
}


