// ==== 0x440acf4  ArcSessionID::setSessionId:  (16 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcSessionID::setSessionId_(ID param_1,SEL param_2,optional<arc::SessionID> param_3)

{
  *(AnonymousUnion46 *)(param_1 + 8) = param_3.field0_0x0;
  *(char *)(param_1 + 0xc) = (char)param_3.field1_0x4;
  return;
}


