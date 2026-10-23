// ==== 0x443fc7c  ArcMenuItemModel::isVisible  (32 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcMenuItemModel::isVisible(ID param_1,SEL param_2)

{
  long *plVar1;
  code *pcVar2;
  bool bVar3;
  
  if ((*(byte *)(param_1 + 0x48) & 1) != 0) {
    plVar1 = *(long **)(*(long *)(param_1 + 0x38) + 0x10);
                    /* WARNING: Could not recover jumptable at 0x0443fc94. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    bVar3 = (**(code **)(*plVar1 + 0xc0))(plVar1,*(undefined8 *)(*(long *)(param_1 + 0x38) + 0x18));
    return bVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x443fc9c);
  (*pcVar2)();
}


