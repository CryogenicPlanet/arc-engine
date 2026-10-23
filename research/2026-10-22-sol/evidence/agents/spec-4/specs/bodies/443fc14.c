// ==== 0x443fc14  ArcMenuItemModel::logicalType  (40 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcMenuItemModel::logicalType(ID param_1,SEL param_2)

{
  code *pcVar1;
  int iVar2;
  
  if ((*(byte *)(param_1 + 0x48) & 1) != 0) {
    iVar2 = FUN_0432bb60(*(undefined8 *)(param_1 + 0x38));
    return (long)iVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443fc3c);
  (*pcVar1)();
}


