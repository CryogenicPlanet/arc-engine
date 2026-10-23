// ==== 0x440ab88  ArcSessionID::initWithID:  (44 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcSessionID::initWithID_(ID param_1,SEL param_2,long_long param_3)

{
  ID IVar1;
  SessionID local_14;
  
  local_14.field0_0x0 = (int)param_3;
  IVar1 = initWithSessionId_(param_1,"initWithSessionId:",&local_14);
  return IVar1;
}


