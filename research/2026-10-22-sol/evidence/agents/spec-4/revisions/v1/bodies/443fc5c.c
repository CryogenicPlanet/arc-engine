// ==== 0x443fc5c  ArcMenuItemModel::isEnabled  (32 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcMenuItemModel::isEnabled(ID param_1,SEL param_2)

{
  long *plVar1;
  code *pcVar2;
  bool bVar3;
  
  if ((*(byte *)(param_1 + 0x48) & 1) != 0) {
    plVar1 = *(long **)(*(long *)(param_1 + 0x38) + 0x10);
                    /* WARNING: Could not recover jumptable at 0x0443fc74. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    bVar3 = (**(code **)(*plVar1 + 0xb8))(plVar1,*(undefined8 *)(*(long *)(param_1 + 0x38) + 0x18));
    return bVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x443fc7c);
  (*pcVar2)();
}


