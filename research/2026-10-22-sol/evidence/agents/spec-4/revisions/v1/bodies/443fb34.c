// ==== 0x443fb34  ArcMenuItemModel::type  (52 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcMenuItemModel::type(ID param_1,SEL param_2)

{
  long *plVar1;
  code *pcVar2;
  int iVar3;
  
  if ((*(byte *)(param_1 + 0x48) & 1) != 0) {
    plVar1 = *(long **)(*(long *)(param_1 + 0x38) + 0x10);
    iVar3 = (**(code **)(*plVar1 + 0x20))(plVar1,*(undefined8 *)(*(long *)(param_1 + 0x38) + 0x18));
    return (long)iVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x443fb68);
  (*pcVar2)();
}


