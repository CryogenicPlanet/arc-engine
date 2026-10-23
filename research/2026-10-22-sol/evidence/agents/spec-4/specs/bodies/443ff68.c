// ==== 0x443ff68  ArcMenuItemModel::activated  (32 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcMenuItemModel::activated(ID param_1,SEL param_2)

{
  long *plVar1;
  code *pcVar2;
  
  if ((*(byte *)(param_1 + 0x48) & 1) != 0) {
    plVar1 = *(long **)(*(long *)(param_1 + 0x38) + 0x10);
                    /* WARNING: Could not recover jumptable at 0x0443ff80. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*plVar1 + 0xf0))(plVar1,*(undefined8 *)(*(long *)(param_1 + 0x38) + 0x18));
    return;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x443ff88);
  (*pcVar2)();
}


