// ==== 0x443bce8  ArcExtensionToolbarAction::icon  (32 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionToolbarAction::icon(ID param_1,SEL param_2)

{
  ID IVar1;
  
  if ((**(long **)(param_1 + 0x58) != 0) && (*(long *)(**(long **)(param_1 + 0x58) + 0x18) != 0)) {
    IVar1 = FUN_02a02ccc();
    return IVar1;
  }
  return 0;
}


