// ==== 0x43ecb04  MacScreenMonitor::isScreenLocked  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool MacScreenMonitor::isScreenLocked(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 9);
}


