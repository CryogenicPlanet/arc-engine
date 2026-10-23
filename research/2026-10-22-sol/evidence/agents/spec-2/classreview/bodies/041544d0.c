// ==== 0x41544d0  ArcExtensionCommand::isGlobal  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcExtensionCommand::isGlobal(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0x70);
}


