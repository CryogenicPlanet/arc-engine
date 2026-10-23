// ==== 0x43e81d8  ArcBrowserApplication::isFeatureEnabled:  (28 bytes)

/* Function Stack Size: 0x18 bytes */

bool ArcBrowserApplication::isFeatureEnabled_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  bool bVar1;
  
  if ((int)param_3 != 0x2d) {
    bVar1 = FUN_020cf42c(param_3);
    return bVar1;
  }
  bVar1 = FUN_00014cc0(&PTR_s_ArcMacCoalescedResizing_0d41f79e_1_0e304a40);
  return bVar1;
}


